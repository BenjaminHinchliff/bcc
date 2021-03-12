#ifndef TCC_TOKENS_HPP
#define TCC_TOKENS_HPP

#include <string>
#include <variant>

namespace tcc {
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

  bool operator==(const Identifier &other) const;
  bool operator!=(const Identifier &other) const;
};

namespace literals {
struct Int {
  int value;

  bool operator==(const Int &other) const;
  bool operator!=(const Int &other) const;
};

} // namespace literals

using Literal = std::variant<literals::Int>;

using Token = std::variant<OpenBrace, CloseBrace, OpenParen, CloseParen,
                           Semicolon, Keyword, Identifier, Literal>;

} // namespace tokens
} // namespace tcc

#endif // !TCC_TOKENS_HPP
