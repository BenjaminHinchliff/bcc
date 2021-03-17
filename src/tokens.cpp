#include "bcc/tokens.hpp"

namespace bcc {
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
  case Keyword::Return:
    out << "Return";
    break;
  default:
    throw std::runtime_error("unknown keyword");
  }
  return out;
}

std::ostream &operator<<(std::ostream &out, const TypeKeyword &tok) {
  switch (tok) {
  case TypeKeyword::Int:
    out << "Int";
    break;
  default:
    throw std::runtime_error("unknown type keyword");
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const Operators& tok) {
  switch (tok) {
  case Operators::BitwiseNot:
    out << "BitwiseNot";
    break;
  case Operators::LogicalNot:
    out << "LogicalNot";
    break;
  case Operators::Minus:
    out << "Minus";
    break;
  case Operators::Add:
    out << "Add";
    break;
  case Operators::Multiply:
    out << "Multiply";
    break;
  case Operators::Divide:
    out << "Divide";
    break;
  default:
    throw std::runtime_error("unknown operators");
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

bool Int::operator==(const Int &other) const { return value == other.value; }
bool Int::operator!=(const Int &other) const { return !(*this == other); }
std::ostream &operator<<(std::ostream &out, const Int &tok) {
  out << "Int(" << tok.value << ')';
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
} // namespace bcc
