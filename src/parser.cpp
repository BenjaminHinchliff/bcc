#include "tcc/parser.hpp"

namespace tcc {
namespace parser {
using namespace tokens;
using namespace ast;
using namespace exceptions;

namespace exceptions {

const char *ParserException::what() const noexcept { return message.c_str(); }

UnexpectedToken::UnexpectedToken(const Token &given, const Token &expected) {
  std::stringstream msgStream;
  msgStream << "unexpected token given: " << given << " expected: " << expected;
  message = msgStream.str();
}

UnexpectedEOF::UnexpectedEOF(const Token &expected) {
  std::stringstream msgStream;
  msgStream << "unexpected EOF - expected: " << expected;
  message = msgStream.str();
}

} // namespace exceptions

void ensureToken(Tokens::const_iterator &it, const Tokens::const_iterator &end, const Token &target) {
  if (it == end) {
    throw UnexpectedEOF(target);
  }
  if (*it != target) {
    throw UnexpectedToken(*it, target);
  }
  ++it;
}

Expr parseExpr(Tokens::const_iterator &it) {
  if (!std::holds_alternative<Literal>(*it)) {
    throw UnexpectedToken(*it, Literal{});
  }
  const Literal &lit = std::get<Literal>(*it++);
  if (!std::holds_alternative<literals::Int>(lit)) {
    throw UnexpectedToken(lit, literals::Int{});
  }
  int val = std::get<literals::Int>(lit).value;
  return Constant{val};
}

Stmt parseStmt(Tokens::const_iterator &it, const Tokens::const_iterator &end) {
  ensureToken(it, end, Token(Keyword::RETURN));
  Expr expr = parseExpr(it);
  // consume semicolon
  ensureToken(it, end, Semicolon{});
  return Return{expr};
}

Function parseFunction(Tokens::const_iterator &it, const Tokens::const_iterator &end) {
  // type keyword
  if (!std::holds_alternative<TypeKeyword>(*it)) {
    throw UnexpectedToken(*it, Token(TypeKeyword::INT));
  }
  ++it;
  const std::string &name = std::get<Identifier>(*it++).name;
  // args
  ensureToken(it, end, OpenParen{});
  ensureToken(it, end, CloseParen{});
  // body
  ensureToken(it, end, OpenBrace{});
  Stmt body = parseStmt(it, end);
  ensureToken(it, end, CloseBrace{});
  return Function{name, body};
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
} // namespace tcc
