#include "tcc/tokens.hpp"

namespace tcc {
namespace tokens {
bool Identifier::operator==(const Identifier &other) const {
  return name == other.name;
}
bool Identifier::operator!=(const Identifier &other) const {
  return !(*this == other);
}

namespace literals {
bool Int::operator==(const Int &other) const { return value == other.value; }
bool Int::operator!=(const Int &other) const { return !(*this == other); }
} // namespace literals
} // namespace tokens
} // namespace tcc
