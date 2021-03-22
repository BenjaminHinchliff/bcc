#include "bcc/parser.hpp"

#include <iostream>
#include <unordered_map>
#include <unordered_set>

namespace bcc {
namespace parser {
using namespace ast;
namespace exceptions {

const char *ParserException::what() const noexcept { return message.c_str(); }

UnexpectedToken::UnexpectedToken(const tokens::Token &given,
                                 const tokens::Token &expected) {
  std::stringstream msgStream;
  msgStream << "unexpected token - given: " << given
            << " expected: " << expected;
  message = msgStream.str();
}

UnexpectedRule::UnexpectedRule(const tokens::Token &given,
                               const std::string &rule) {
  std::stringstream msgStream;
  msgStream << "unexpected rule - given: " << given << " expected: " << rule;
  message = msgStream.str();
}

UnexpectedEOF::UnexpectedEOF(const tokens::Token &expected) {
  std::stringstream msgStream;
  msgStream << "unexpected EOF - expected: " << expected;
  message = msgStream.str();
}

} // namespace exceptions

using namespace exceptions;

void ensureToken(Tokens::const_iterator &it, const Tokens::const_iterator &end,
                 const tokens::Token &target) {
  if (it == end) {
    throw UnexpectedEOF(target);
  }
  if (*it != target) {
    throw UnexpectedToken(*it, target);
  }
  ++it;
}

const std::unordered_map<tokens::Operators, UnaryOperator::Kind>
    UN_OP_TOK_TO_KIND{
        {tokens::Operators::BitwiseNot, UnaryOperator::Kind::BitwiseNot},
        {tokens::Operators::LogicalNot, UnaryOperator::Kind::LogicalNot},
        {tokens::Operators::Minus, UnaryOperator::Kind::Negate}};

const std::unordered_map<tokens::Operators, BinaryOperator::Kind>
    BIN_OP_TOK_TO_KIND{
        {tokens::Operators::Add, BinaryOperator::Kind::Addition},
        {tokens::Operators::Minus, BinaryOperator::Kind::Subtraction},
        {tokens::Operators::Multiply, BinaryOperator::Kind::Multiplication},
        {tokens::Operators::Divide, BinaryOperator::Kind::Division},
        {tokens::Operators::Modulo, BinaryOperator::Kind::Modulo},
        {tokens::Operators::LogicalAnd, BinaryOperator::Kind::LogicalAnd},
        {tokens::Operators::LogicalOr, BinaryOperator::Kind::LogicalOr},
        {tokens::Operators::BitwiseAnd, BinaryOperator::Kind::BitwiseAnd},
        {tokens::Operators::BitwiseOr, BinaryOperator::Kind::BitwiseOr},
        {tokens::Operators::BitwiseXor, BinaryOperator::Kind::BitwiseXor},
        {tokens::Operators::BitwiseLeftShift,
         BinaryOperator::Kind::BitwiseLeftShift},
        {tokens::Operators::BitwiseRightShift,
         BinaryOperator::Kind::BitwiseRightShift},
        {tokens::Operators::Equal, BinaryOperator::Kind::Equal},
        {tokens::Operators::NotEqual, BinaryOperator::Kind::NotEqual},
        {tokens::Operators::LessThan, BinaryOperator::Kind::LessThan},
        {tokens::Operators::LessThanOrEqual,
         BinaryOperator::Kind::LessThanOrEqual},
        {tokens::Operators::GreaterThan, BinaryOperator::Kind::GreaterThan},
        {tokens::Operators::GreaterThanOrEqual,
         BinaryOperator::Kind::GreaterThanOrEqual},
    };

bool canBeUnary(const tokens::Operators &op) {
  return UN_OP_TOK_TO_KIND.count(op);
}

const std::unordered_map<tokens::Operators, int> BINARY_OP_PREC{
    {tokens::Operators::LogicalOr, 0},
    {tokens::Operators::LogicalAnd, 1},
    {tokens::Operators::BitwiseOr, 2},
    {tokens::Operators::BitwiseXor, 3},
    {tokens::Operators::BitwiseAnd, 4},
    {tokens::Operators::Equal, 5},
    {tokens::Operators::NotEqual, 5},
    {tokens::Operators::LessThan, 6},
    {tokens::Operators::LessThanOrEqual, 6},
    {tokens::Operators::GreaterThan, 6},
    {tokens::Operators::GreaterThanOrEqual, 6},
    {tokens::Operators::Add, 7},
    {tokens::Operators::Minus, 7},
    {tokens::Operators::Multiply, 8},
    {tokens::Operators::Divide, 8},
    {tokens::Operators::Modulo, 8}};

std::unique_ptr<Expr> parseFactor(Tokens::const_iterator &it);

std::unique_ptr<Expr> parseUnary(Tokens::const_iterator &it,
                                 tokens::Operators op) {
  auto kind_it = UN_OP_TOK_TO_KIND.find(op);
  if (kind_it == UN_OP_TOK_TO_KIND.end()) {
    throw UnexpectedRule(*it, "expression");
  }
  UnaryOperator::Kind kind = kind_it->second;
  auto expr = parseFactor(++it);
  return std::make_unique<Expr>(UnaryOperator(kind, std::move(expr)));
}

std::unique_ptr<Expr> parseExpr(Tokens::const_iterator &it, int min_prec);

std::unique_ptr<Expr> parseFactor(Tokens::const_iterator &it) {
  if (std::holds_alternative<tokens::OpenParen>(*it)) {
    ++it;
    std::unique_ptr<Expr> expr = parseExpr(it, 0);
    if (!std::holds_alternative<tokens::CloseParen>(*it)) {
      throw UnexpectedToken(*it, tokens::CloseParen{});
    }
    ++it;
    return expr;
  } else if (auto op = std::get_if<tokens::Operators>(&*it);
             op && canBeUnary(*op)) {
    return parseUnary(it, *op);
  } else if (auto lit = std::get_if<tokens::Int>(&*it)) {
    int val = lit->value;
    ++it;
    return std::make_unique<Expr>(Constant{val});
  } else {
    throw UnexpectedRule(*it, "expression");
  }
}

std::unique_ptr<Expr> parseExpr(Tokens::const_iterator &it, int min_prec) {
  std::unique_ptr<Expr> result = parseFactor(it);

  while (std::holds_alternative<tokens::Operators>(*it)) {
    auto op = std::get<tokens::Operators>(*it);
    auto prec_it = BINARY_OP_PREC.find(op);

    // break out if doesn't have precedence (isn't binop)
    if (prec_it == BINARY_OP_PREC.end())
      break;

    int prec = prec_it->second;
    // break if precedence is too low
    if (prec < min_prec)
      break;

    auto rhs = parseExpr(++it, prec + 1);
    BinaryOperator::Kind kind = BIN_OP_TOK_TO_KIND.at(op);
    result = std::make_unique<Expr>(
        BinaryOperator(kind, std::move(result), std::move(rhs)));
  }
  return result;
}

Stmt parseStmt(Tokens::const_iterator &it, const Tokens::const_iterator &end) {
  ensureToken(it, end, tokens::Token(tokens::Keyword::Return));
  std::unique_ptr<Expr> expr = parseExpr(it, 0);
  // consume semicolon
  ensureToken(it, end, tokens::Semicolon{});
  return Return{std::move(expr)};
}

Function parseFunction(Tokens::const_iterator &it,
                       const Tokens::const_iterator &end) {
  // type keyword
  if (!std::holds_alternative<tokens::TypeKeyword>(*it)) {
    throw UnexpectedToken(*it, tokens::Token(tokens::TypeKeyword::Int));
  }
  ++it;
  const std::string &name = std::get<tokens::Identifier>(*it++).name;
  // args
  ensureToken(it, end, tokens::OpenParen{});
  ensureToken(it, end, tokens::CloseParen{});
  // body
  ensureToken(it, end, tokens::OpenBrace{});
  auto body = std::make_unique<Stmt>(parseStmt(it, end));
  ensureToken(it, end, tokens::CloseBrace{});
  return Function(name, std::move(body));
}

Program parseTokens(const Tokens &tokens) {
  auto it = tokens.cbegin();
  auto end = tokens.cend();
  Function func = parseFunction(it, end);
  return func;
}

ast::Program parse(const std::string &source) {
  const Tokens tokens = lexer::lex(source);
  return parseTokens(tokens);
}
} // namespace parser
} // namespace bcc
