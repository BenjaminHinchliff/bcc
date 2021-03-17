#include "bcc/parser.hpp"

#include <iostream>

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

const std::array<tokens::Token, 4> unaryOps{
    tokens::Minus{}, tokens::BitwiseComplement{}, tokens::LogicalNegation{}};

bool canBeUnary(const tokens::Token &token) {
  return std::find(unaryOps.begin(), unaryOps.end(), token) != unaryOps.end();
}

std::unique_ptr<Expr> parseExpr(Tokens::const_iterator &it);

std::unique_ptr<Expr> parseUnary(Tokens::const_iterator &it) {
  if (std::holds_alternative<tokens::Minus>(*it)) {
    auto expr = parseExpr(++it);
    return std::make_unique<Expr>(Negation(std::move(expr)));
  } else if (std::holds_alternative<tokens::BitwiseComplement>(*it)) {
    auto expr = parseExpr(++it);
    return std::make_unique<Expr>(BitwiseComplement(std::move(expr)));
  } else if (std::holds_alternative<tokens::LogicalNegation>(*it)) {
    auto expr = parseExpr(++it);
    return std::make_unique<Expr>(Not(std::move(expr)));
  } else {
    throw UnexpectedRule(*it, "expression");
  }
}

std::unique_ptr<Expr> parseFactor(Tokens::const_iterator &it) {
  if (std::holds_alternative<tokens::OpenParen>(*it)) {
    ++it;
    std::unique_ptr<Expr> expr = parseExpr(it);
    if (!std::holds_alternative<tokens::CloseParen>(*it)) {
      throw UnexpectedToken(*it, tokens::CloseParen{});
    }
    ++it;
    return expr;
  } else if (canBeUnary(*it)) {
    return parseUnary(it);
  } else if (auto lit = std::get_if<tokens::Int>(&*it)) {
    int val = lit->value;
    ++it;
    return std::make_unique<Expr>(Constant{val});
  } else {
    throw UnexpectedRule(*it, "expression");
  }
}

std::unique_ptr<Expr> parseTerm(Tokens::const_iterator &it) {
  std::unique_ptr<Expr> factor = parseFactor(it);
  while (std::holds_alternative<tokens::Multiplication>(*it) ||
         std::holds_alternative<tokens::Division>(*it)) {
    if (std::holds_alternative<tokens::Multiplication>(*it)) {
      std::unique_ptr<Expr> nextFactor = parseFactor(++it);
      factor = std::make_unique<Expr>(
          Multiplication(std::move(factor), std::move(nextFactor)));
    } else if (std::holds_alternative<tokens::Division>(*it)) {
      std::unique_ptr<Expr> nextFactor = parseFactor(++it);
      factor = std::make_unique<Expr>(
          Division(std::move(factor), std::move(nextFactor)));
    }
  }
  return factor;
}

std::unique_ptr<Expr> parseExpr(Tokens::const_iterator &it) {
  std::unique_ptr<Expr> term = parseTerm(it);
  while (std::holds_alternative<tokens::Addition>(*it) ||
         std::holds_alternative<tokens::Minus>(*it)) {
    if (std::holds_alternative<tokens::Addition>(*it)) {
      std::unique_ptr<Expr> nextTerm = parseTerm(++it);
      term = std::make_unique<Expr>(
          Addition(std::move(term), std::move(nextTerm)));
    } else if (std::holds_alternative<tokens::Minus>(*it)) {
      std::unique_ptr<Expr> nextTerm = parseTerm(++it);
      term = std::make_unique<Expr>(
          Subtraction(std::move(term), std::move(nextTerm)));
    }
  }
  return term;
}

Stmt parseStmt(Tokens::const_iterator &it, const Tokens::const_iterator &end) {
  ensureToken(it, end, tokens::Token(tokens::Keyword::RETURN));
  std::unique_ptr<Expr> expr = parseExpr(it);
  // consume semicolon
  ensureToken(it, end, tokens::Semicolon{});
  return Return{std::move(expr)};
}

Function parseFunction(Tokens::const_iterator &it,
                       const Tokens::const_iterator &end) {
  // type keyword
  if (!std::holds_alternative<tokens::TypeKeyword>(*it)) {
    throw UnexpectedToken(*it, tokens::Token(tokens::TypeKeyword::INT));
  }
  ++it;
  const std::string &name = std::get<tokens::Identifier>(*it++).name;
  // args
  ensureToken(it, end, tokens::OpenParen{});
  ensureToken(it, end, tokens::CloseParen{});
  // body
  ensureToken(it, end, tokens::OpenBrace{});
  Stmt body = parseStmt(it, end);
  ensureToken(it, end, tokens::CloseBrace{});
  return Function{name, std::move(body)};
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
