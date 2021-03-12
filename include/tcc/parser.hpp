#ifndef TCC_PARSER_HPP
#define TCC_PARSER_HPP

#include <cassert>
#include <ostream>
#include <stdexcept>
#include <string>
#include <variant>

#include "ast.hpp"
#include "lexer.hpp"

namespace tcc {
namespace parser {
namespace exceptions {
struct UnexpectedToken : public std::exception {
public:
  UnexpectedToken(const lexer::Token &given, const lexer::Token &expected);

  const char *what() const noexcept;

private:
  lexer::Token given;
  lexer::Token expected;
  std::string message;
};
} // namespace exceptions

ast::Program parse(const lexer::Tokens &tokens);
} // namespace parser
} // namespace tcc

#endif // !TCC_PARSER_HPP
