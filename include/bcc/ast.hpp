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

class UnaryOperator;
class BinaryOperator;
using Expr = std::variant<UnaryOperator, BinaryOperator, Constant>;

class UnaryOperator {
public:
  enum class Kind {
    BitwiseNot,
    LogicalNot,
    Negate,
  };

public:
  UnaryOperator(Kind kind, std::unique_ptr<Expr> expr);
  
public:
  bool operator==(const UnaryOperator &other) const;
  bool operator!=(const UnaryOperator &other) const;
  void print(std::ostream &out, size_t indent) const;

public:
  Kind kind;
  std::unique_ptr<Expr> expr;
};

class BinaryOperator {
public:
  enum class Kind {
    Addition,
    Subtraction,
    Multiplication,
    Division
  };

public:
  BinaryOperator(Kind kind, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs);

public:
  bool operator==(const BinaryOperator &other) const;
  bool operator!=(const BinaryOperator &other) const;
  void print(std::ostream &out, size_t indent) const;

public:
  Kind kind;
  std::unique_ptr<Expr> lhs;
  std::unique_ptr<Expr> rhs;
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
