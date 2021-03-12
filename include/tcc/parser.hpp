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
  const char *what() const noexcept { return "Unexpected Token"; }
};
} // namespace exceptions

ast::Program parse(const lexer::Tokens &tokens);
} // namespace parser
} // namespace tcc

#endif // !TCC_PARSER_HPP
