#ifndef TCC_AST_HPP
#define TCC_AST_HPP

#include <ostream>
#include <variant>

namespace tcc {
namespace ast {
struct Constant {
  int val;

  bool operator==(const Constant &other) const;
  bool operator!=(const Constant &other) const;

  void print(std::ostream &out, size_t indent) const;
};

struct Negation;
struct BitwiseComplement;
struct Not;
using UnaryOperator = std::variant<Negation, BitwiseComplement, Not>;
using Expr = std::variant<Constant, UnaryOperator>;

struct UnaryOperatorBase {
  UnaryOperatorBase(std::unique_ptr<Expr> &expr);

  bool operator==(const UnaryOperatorBase &other) const;
  bool operator!=(const UnaryOperatorBase &other) const;

  std::unique_ptr<Expr> expr;
};

struct Negation : public UnaryOperatorBase {
  using UnaryOperatorBase::UnaryOperatorBase;
  void print(std::ostream &out, size_t indent) const;
};

struct BitwiseComplement : public UnaryOperatorBase {
  using UnaryOperatorBase::UnaryOperatorBase;
  void print(std::ostream &out, size_t indent) const;
};

struct Not : public UnaryOperatorBase {
  using UnaryOperatorBase::UnaryOperatorBase;
  void print(std::ostream &out, size_t indent) const;
};


struct Return {
  Expr expr;

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
} // namespace tcc

#endif // !TCC_AST_HPP
