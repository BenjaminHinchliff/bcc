#ifndef TCC_LEXER_HPP
#define TCC_LEXER_HPP

#include <string>
#include <variant>
#include <vector>

#include "tokens.hpp"

namespace tcc {
namespace lexer {
Tokens lex(const std::string &source);
} // namespace lexer
} // namespace tcc

#endif // !TCC_LEXER_HPP
