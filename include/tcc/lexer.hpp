#ifndef TCC_LEXER_HPP
#define TCC_LEXER_HPP

#include <string>
#include <variant>
#include <vector>

#include "tokens.hpp"

namespace tcc {
namespace lexer {

using tokens::Token;
using Tokens = std::vector<Token>;

Tokens lex(const std::string &source);
} // namespace lexer
} // namespace tcc

#endif // !TCC_LEXER_HPP
