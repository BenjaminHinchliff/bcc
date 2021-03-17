#ifndef BCC_AST_HPP
#define BCC_AST_HPP

#include <memory>
#include <ostream>
#include <variant>

namespace bcc {
namespace ast {
template <typename T, typename... Args> struct variant_extend;

template <typename... Args0, typename... Args1>
struct variant_extend<std::variant<Args0...>, Args1...> {
    using type = std::variant<Args0..., Args1...>;
};

template <typename T, typename... Args> struct variant_concat;

template <typename... Args0, typename... Args1>
struct variant_concat<std::variant<Args0...>, std::variant<Args1...>> {
    using type = std::variant<Args0..., Args1...>;
};


struct Constant {
  int val;

  bool operator==(const Constant &other) const;
  bool operator!=(const Constant &other) const;

  void print(std::ostream &out, size_t indent) const;
};

struct Negation;
struct BitwiseComplement;
struct Not;
struct Addition;
struct Subtraction;
struct Multiplication;
struct Division;
using UnaryOperator = std::variant<Negation, BitwiseComplement, Not>;
using BinaryOperator = std::variant<Addition, Subtraction, Multiplication, Division>;
using Expr = variant_extend<variant_concat<BinaryOperator, UnaryOperator>::type, Constant>::type;

template<typename T>
struct UnaryOperatorBase {
  UnaryOperatorBase(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

  bool operator==(const T& other) const { return *expr == *other.expr; }
  bool operator!=(const T &other) const { return !(*this == other); }

  std::unique_ptr<Expr> expr;
};

struct Negation : public UnaryOperatorBase<Negation> {
  using UnaryOperatorBase::UnaryOperatorBase;
  void print(std::ostream &out, size_t indent) const;
};

struct BitwiseComplement : public UnaryOperatorBase<BitwiseComplement> {
  using UnaryOperatorBase::UnaryOperatorBase;
  void print(std::ostream &out, size_t indent) const;
};

struct Not : public UnaryOperatorBase<Not> {
  using UnaryOperatorBase::UnaryOperatorBase;
  void print(std::ostream &out, size_t indent) const;
};

template<typename T>
struct BinaryOperatorBase {
  BinaryOperatorBase(std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {}

  bool operator==(const T& other) const {
    return *lhs == *other.lhs && *rhs == *other.rhs;
  }
  bool operator!=(const T&other) const;

  std::unique_ptr<Expr> lhs;
  std::unique_ptr<Expr> rhs;
};

struct Addition : public BinaryOperatorBase<Addition> {
  using BinaryOperatorBase::BinaryOperatorBase;
  void print(std::ostream &out, size_t indent) const;
};

struct Subtraction : public BinaryOperatorBase<Subtraction> {
  using BinaryOperatorBase::BinaryOperatorBase;
  void print(std::ostream &out, size_t indent) const;
};

struct Multiplication : public BinaryOperatorBase<Multiplication> {
  using BinaryOperatorBase::BinaryOperatorBase;
  void print(std::ostream &out, size_t indent) const;
};

struct Division : public BinaryOperatorBase<Division> {
  using BinaryOperatorBase::BinaryOperatorBase;
  void print(std::ostream &out, size_t indent) const;
};

struct Return {
  std::unique_ptr<Expr> expr;

  bool operator==(const Return &other) const;
  bool operator!=(const Return &other) const;

  void print(std::ostream &out, size_t indent) const;
};

using Stmt = std::variant<Return>;

struct Function {
  std::string name;
  Stmt body;

  bool operator==(const Function &other) const;
  bool operator!=(const Function &other) const;

  void print(std::ostream &out, size_t indent) const;
};

std::ostream &operator<<(std::ostream &out, const Function &func);

using Program = Function;
} // namespace ast
} // namespace bcc

#endif // !BCC_AST_HPP
