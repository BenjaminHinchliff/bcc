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

using Expr = std::variant<Constant>;

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
