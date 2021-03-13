#include "tcc/parser.hpp"

namespace tcc {
namespace parser {
using namespace tokens;
using namespace ast;
using namespace exceptions;

namespace exceptions {
UnexpectedToken::UnexpectedToken(const Token &given, const Token &expected)
    : given(given), expected(expected) {
  std::stringstream msgStream;
  msgStream << "unexpected token given: " << this->given
            << "expected: " << this->expected;
  message = msgStream.str();
}

const char *UnexpectedToken::what() const noexcept { return message.c_str(); }
} // namespace exceptions

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

Stmt parseStmt(Tokens::const_iterator &it) {
  if (*it != Token(Keyword::RETURN)) {
    throw UnexpectedToken(*it, Keyword::RETURN);
  }
  ++it;
  Expr expr = parseExpr(it);
  // consume semicolon
  if (*it != Token(Semicolon{})) {
    throw UnexpectedToken(*it, Semicolon{});
  }
  ++it;
  return Return{expr};
}

Function parseFunction(Tokens::const_iterator &it) {
  // type keyword
  ++it;
  const std::string &name = std::get<Identifier>(*it++).name;
  // args
  // open paren
  if (*it != Token(OpenParen{})) {
    throw UnexpectedToken(*it, OpenParen{});
  }
  ++it;
  // close paren
  if (*it != Token(CloseParen{})) {
    throw UnexpectedToken(*it, CloseParen{});
  }
  ++it;
  // open brace
  if (*it != Token(OpenBrace{})) {
    throw UnexpectedToken(*it, OpenBrace{});
  }
  ++it;
  Stmt body = parseStmt(it);
  // close brace
  if (*it != Token(CloseBrace{})) {
    throw UnexpectedToken(*it, CloseBrace{});
  }
  ++it;

  return Function{name, body};
}

// TODO: error handling
Program parse(const Tokens &tokens) {
  Tokens::const_iterator it = tokens.cbegin();
  Function func = parseFunction(it);
  assert(it == tokens.cend() && "unexpected tokens at end of source");
  return func;
}
} // namespace parser
} // namespace tcc
