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
  UnexpectedToken(const tokens::Token &given, const tokens::Token &expected);

  const char *what() const noexcept;

private:
  tokens::Token given;
  tokens::Token expected;
  std::string message;
};
} // namespace exceptions

ast::Program parse(const Tokens &tokens);
} // namespace parser
} // namespace tcc

#endif // !TCC_PARSER_HPP
