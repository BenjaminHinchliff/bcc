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
            << " expected: " << this->expected;
  message = msgStream.str();
}

const char *UnexpectedToken::what() const noexcept { return message.c_str(); }
} // namespace exceptions

void ensureToken(const Token &cur, const Token &target) {
  if (cur != target) {
    throw UnexpectedToken(cur, target);
  }
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

Stmt parseStmt(Tokens::const_iterator &it) {
  ensureToken(*it++, Token(Keyword::RETURN));
  Expr expr = parseExpr(it);
  // consume semicolon
  ensureToken(*it++, Semicolon{});
  return Return{expr};
}

Function parseFunction(Tokens::const_iterator &it) {
  // type keyword
  if (!std::holds_alternative<TypeKeyword>(*it)) {
    throw UnexpectedToken(*it, Token(TypeKeyword::INT));
  }
  ++it;
  const std::string &name = std::get<Identifier>(*it++).name;
  // args
  ensureToken(*it++, OpenParen{}); 
  ensureToken(*it++, CloseParen{}); 
  // body
  ensureToken(*it++, OpenBrace{});
  Stmt body = parseStmt(it);
  ensureToken(*it++, CloseBrace{});
  return Function{name, body};
}

Program parseTokens(const Tokens &tokens) {
  Tokens::const_iterator it = tokens.cbegin();
  Function func = parseFunction(it);
  return func;
}

ast::Program parse(const std::string& source) {
  const Tokens tokens = lexer::lex(source);
  return parseTokens(tokens);
}
} // namespace parser
} // namespace tcc
