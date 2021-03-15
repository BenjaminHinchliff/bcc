#include "bcc/parser.hpp"

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

Expr parseExpr(Tokens::const_iterator &it) {
  if (auto lit = std::get_if<tokens::Literal>(&*it)) {
    if (!std::holds_alternative<tokens::literals::Int>(*lit)) {
      throw UnexpectedToken(*lit, tokens::literals::Int{});
    }
    int val = std::get<tokens::literals::Int>(*lit).value;
    ++it;
    return Constant{val};
  } else if (std::holds_alternative<tokens::Minus>(*it)) {
    auto expr = std::make_unique<Expr>(parseExpr(++it));
    return Negation(std::move(expr));
  } else if (std::holds_alternative<tokens::BitwiseComplement>(*it)) {
    auto expr = std::make_unique<Expr>(parseExpr(++it));
    return BitwiseComplement(std::move(expr));
  } else if (std::holds_alternative<tokens::LogicalNegation>(*it)) {
    auto expr = std::make_unique<Expr>(parseExpr(++it));
    return Not(std::move(expr));
  }  else {
    throw UnexpectedRule(*it, "expression");
  }
}

Stmt parseStmt(Tokens::const_iterator &it, const Tokens::const_iterator &end) {
  ensureToken(it, end, tokens::Token(tokens::Keyword::RETURN));
  Expr expr = parseExpr(it);
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
