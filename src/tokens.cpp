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
  case Operators::LogicalAnd:
    out << "And";
    break;
  case Operators::LogicalOr:
    out << "Or";
    break;
  case Operators::Equal:
    out << "Equal";
    break;
  case Operators::NotEqual:
    out << "NotEqual";
    break;
  case Operators::LessThan:
    out << "LessThan";
    break;
  case Operators::LessThanOrEqual:
    out << "LessThanOrEqual";
    break;
  case Operators::GreaterThan:
    out << "GreaterThan";
    break;
  case Operators::GreaterThanOrEqual:
    out << "GreaterThanOrEqual";
    break;
  default:
    throw std::runtime_error("unknown operator");
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
