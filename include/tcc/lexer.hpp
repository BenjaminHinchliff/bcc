#ifndef TCC_LEXER_HPP
#define TCC_LEXER_HPP

#include <string>
#include <variant>
#include <vector>

namespace tcc {
namespace lexer {
namespace tokens {
template <typename T> struct InheritedEquality {
  constexpr bool operator==(const T &) const { return true; }
  constexpr bool operator!=(const T &other) const { return !(*this == other); }
};

struct OpenBrace : InheritedEquality<OpenBrace> {};
struct CloseBrace : InheritedEquality<CloseBrace> {};
struct OpenParen : InheritedEquality<OpenParen> {};
struct CloseParen : InheritedEquality<CloseParen> {};
struct Semicolon : InheritedEquality<Semicolon> {};

enum class Keyword { INT, RETURN };

struct Identifier {
  std::string name;

  bool operator==(const Identifier &other) const { return name == other.name; }
  bool operator!=(const Identifier &other) const { return !(*this == other); }
};

namespace literals {
struct Int {
  int value;

  bool operator==(const Int &other) const { return value == other.value; }
  bool operator!=(const Int &other) const { return !(*this == other); }
};

} // namespace literals

using Literal = std::variant<literals::Int>;

using Token = std::variant<OpenBrace, CloseBrace, OpenParen, CloseParen,
                           Semicolon, Keyword, Identifier, Literal>;

} // namespace tokens

using tokens::Token;
using Tokens = std::vector<Token>;

Tokens lex(const std::string &source);
} // namespace lexer
} // namespace tcc

#endif // !TCC_LEXER_HPP
