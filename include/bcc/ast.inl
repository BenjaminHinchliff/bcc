// implementation for templates in ast.hpp

namespace bcc {
namespace ast {
template <typename T>
bool UnaryOperatorBase<T>::operator==(const T &other) const {
  return *expr == *other.expr;
}

template <typename T>
bool UnaryOperatorBase<T>::operator!=(const T &other) const {
  return !(*this == other);
}

template <typename T>
bool BinaryOperatorBase<T>::operator==(const T &other) const {
  return *lhs == *other.lhs && *rhs == *other.rhs;
}

template <typename T>
bool BinaryOperatorBase<T>::operator!=(const T &other) const {
  return !(*this == other);
}
} // namespace ast
} // namespace bcc
