#ifndef TCC_PARSER_HPP
#define TCC_PARSER_HPP

#include <cassert>
#include <string>
#include <variant>
#include <ostream>

#include "lexer.hpp"
#include "ast.hpp"

namespace tcc {
namespace parser {
ast::Program parse(const lexer::Tokens &tokens);
} // namespace parser
} // namespace tcc

#endif // !TCC_PARSER_HPP
