#ifndef BCC_LEXER_HPP
#define BCC_LEXER_HPP

#include <string>
#include <variant>
#include <vector>

#include "tokens.hpp"

namespace bcc {
namespace lexer {
Tokens lex(const std::string &source);
} // namespace lexer
} // namespace bcc

#endif // !BCC_LEXER_HPP
