#include "tcc/tokens.hpp"

namespace tcc {
namespace tokens {
std::ostream &operator<<(std::ostream &out, const OpenBrace &) {
  out << "OpenBrace";
  return out;
}

std::ostream &operator<<(std::ostream &out, const CloseBrace &) {
  out << "CloseBrace";
  return out;
}

std::ostream &operator<<(std::ostream &out, const OpenParen &) {
  out << "OpenParen";
  return out;
}

std::ostream &operator<<(std::ostream &out, const CloseParen &) {
  out << "CloseParen";
  return out;
}

std::ostream &operator<<(std::ostream &out, const Semicolon &) {
  out << "Semicolon";
  return out;
}

std::ostream &operator<<(std::ostream &out, const Keyword &tok) {
  switch (tok) {
  case Keyword::RETURN:
    out << "RETURN";
    break;
  default:
    throw std::runtime_error("unknown keyword");
  }
  return out;
}

std::ostream &operator<<(std::ostream &out, const TypeKeyword &tok) {
  switch (tok) {
  case TypeKeyword::INT:
    out << "INT";
    break;
  default:
    throw std::runtime_error("unknown keyword");
  }
  return out;
}

std::ostream &operator<<(std::ostream &out, const UnaryOperator &tok) {
  switch (tok) {
  case UnaryOperator::NEGATION:
    out << "NEGATION";
    break;
  case UnaryOperator::BITWISE_COMPLEMENT:
    out << "BITWISE_COMPLEMENT";
    break;
  case UnaryOperator::LOGICAL_NEGATION:
    out << "LOGICAL_NEGATION";
    break;
  default:
    throw std::runtime_error("unknown keyword");
  }
  return out;
}

bool Identifier::operator==(const Identifier &other) const {
  return name == other.name;
}
bool Identifier::operator!=(const Identifier &other) const {
  return !(*this == other);
}
std::ostream &operator<<(std::ostream &out, const Identifier &tok) {
  out << "Ident(" << tok.name << ')';
  return out;
}

namespace literals {
bool Int::operator==(const Int &other) const { return value == other.value; }
bool Int::operator!=(const Int &other) const { return !(*this == other); }
std::ostream &operator<<(std::ostream &out, const Int &tok) {
  out << "Int(" << tok.value << ')';
  return out;
}
} // namespace literals

std::ostream &operator<<(std::ostream &out, const Literal &lit) {
  std::visit([&](const auto &l) { out << l; }, lit);
  return out;
}

std::ostream &operator<<(std::ostream &out, const Token &tok) {
  std::visit([&](const auto &t) { out << t; }, tok);
  return out;
}

std::ostream &operator<<(std::ostream &out, const Tokens &toks) {
  for (const Token &tok : toks) {
    out << tok << ' ';
  }
  return out;
}
} // namespace tokens
} // namespace tcc
