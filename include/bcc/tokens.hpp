#ifndef BCC_TOKENS_HPP
#define BCC_TOKENS_HPP

#include <ostream>
#include <string>
#include <variant>
#include <vector>

namespace bcc {
namespace tokens {
template <typename T> struct InheritedEquality {
  constexpr bool operator==(const T &) const { return true; }
  constexpr bool operator!=(const T &other) const { return !(*this == other); }
};

struct OpenBrace : InheritedEquality<OpenBrace> {};
std::ostream &operator<<(std::ostream &out, const OpenBrace &);

struct CloseBrace : InheritedEquality<CloseBrace> {};
std::ostream &operator<<(std::ostream &out, const CloseBrace &);

struct OpenParen : InheritedEquality<OpenParen> {};
std::ostream &operator<<(std::ostream &out, const OpenParen &);

struct CloseParen : InheritedEquality<CloseParen> {};
std::ostream &operator<<(std::ostream &out, const CloseParen &);

struct Semicolon : InheritedEquality<Semicolon> {};
std::ostream &operator<<(std::ostream &out, const Semicolon &);

enum class Keyword { Return };
std::ostream &operator<<(std::ostream &out, const Keyword &tok);

enum class TypeKeyword { Int };
std::ostream &operator<<(std::ostream &out, const TypeKeyword &tok);

enum class Operators {
  BitwiseNot,
  LogicalNot,
  Minus,
  Add,
  Multiply,
  Divide,
  And,
  Or,
  Equal,
  NotEqual,
  LessThan,
  LessThanOrEqual,
  GreaterThan,
  GreaterThanOrEqual
};

struct Identifier {
  std::string name;

  bool operator==(const Identifier &other) const;
  bool operator!=(const Identifier &other) const;
};
std::ostream &operator<<(std::ostream &out, const Identifier &tok);

struct Int {
  int value;

  bool operator==(const Int &other) const;
  bool operator!=(const Int &other) const;
};
std::ostream &operator<<(std::ostream &out, const Int &tok);

using Token = std::variant<OpenBrace, CloseBrace, OpenParen, CloseParen,
                           Semicolon, Keyword, TypeKeyword, Identifier, Int, Operators>;

std::ostream &operator<<(std::ostream &out, const Token &tok);

using Tokens = std::vector<Token>;

std::ostream &operator<<(std::ostream &out, const Tokens &tok);
} // namespace tokens

using tokens::Tokens;
} // namespace bcc

#endif // !BCC_TOKENS_HPP
