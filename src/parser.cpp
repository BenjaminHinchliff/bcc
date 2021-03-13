#include "tcc/parser.hpp"

namespace tcc {
namespace parser {
using namespace tokens;
using namespace ast;
using namespace exceptions;

namespace exceptions {
UnexpectedToken::UnexpectedToken(const Token &given, const Token &expected)
    : given(given), expected(expected), message("Unexpected Token") {}

const char *UnexpectedToken::what() const noexcept { return "asdf"; }
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
  ++it;
  return Return{expr};
}

Function parseFunction(Tokens::const_iterator &it) {
  // type keyword
  ++it;
  const std::string &name = std::get<Identifier>(*it++).name;
  // args
  it += 2;
  // open brace
  ++it;
  Stmt body = parseStmt(it);
  // close brace
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
