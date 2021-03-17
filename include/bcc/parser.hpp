#ifndef BCC_PARSER_HPP
#define BCC_PARSER_HPP

#include <cassert>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>
#include <array>
#include <algorithm>

#include "ast.hpp"
#include "lexer.hpp"

namespace bcc {
namespace parser {
namespace exceptions {
class ParserException : public std::exception {
public:
  const char *what() const noexcept;

protected:
  std::string message;
};

class UnexpectedToken : public ParserException {
public:
  UnexpectedToken(const tokens::Token &given, const tokens::Token &expected);
};

class UnexpectedRule : public ParserException {
public:
  UnexpectedRule(const tokens::Token &given, const std::string &rule);
};

class UnexpectedEOF : public ParserException {
public:
  UnexpectedEOF(const tokens::Token &expected);
};
} // namespace exceptions

using Exception = exceptions::ParserException;

ast::Program parseTokens(const Tokens &tokens);

ast::Program parse(const std::string &source);
} // namespace parser
} // namespace bcc

#endif // !BCC_PARSER_HPP
