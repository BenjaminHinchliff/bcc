#ifndef BCC_AST_HPP
#define BCC_AST_HPP

#include <memory>
#include <ostream>
#include <variant>

namespace bcc {
namespace ast {
class Constant {
public:
  Constant(int val);

public:
  bool operator==(const Constant &other) const;
  bool operator!=(const Constant &other) const;
  void print(std::ostream &out, size_t indent) const;

public:
  int val;
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

  Kind getKind() const;
  const std::unique_ptr<Expr> &getExpr() const;

private:
  Kind kind;
  std::unique_ptr<Expr> expr;
};

class BinaryOperator {
public:
  enum class Kind {
    Addition,
    Subtraction,
    Multiplication,
    Division,
    Modulo,
    LogicalAnd,
    LogicalOr,
    BitwiseAnd,
    BitwiseOr,
    BitwiseXor,
    BitwiseLeftShift,
    BitwiseRightShift,
    Equal,
    NotEqual,
    LessThan,
    LessThanOrEqual,
    GreaterThan,
    GreaterThanOrEqual,
  };

public:
  BinaryOperator(Kind kind, std::unique_ptr<Expr> lhs,
                 std::unique_ptr<Expr> rhs);

public:
  bool operator==(const BinaryOperator &other) const;
  bool operator!=(const BinaryOperator &other) const;
  void print(std::ostream &out, size_t indent) const;

  Kind getKind() const;
  const std::unique_ptr<Expr> &getLhs() const;
  const std::unique_ptr<Expr> &getRhs() const;

private:
  Kind kind;
  std::unique_ptr<Expr> lhs;
  std::unique_ptr<Expr> rhs;
};

struct Return {
public:
  Return(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

public:
  bool operator==(const Return &other) const;
  bool operator!=(const Return &other) const;
  void print(std::ostream &out, size_t indent) const;
  const std::unique_ptr<Expr> &getExpr() const;

private:
  std::unique_ptr<Expr> expr;
};

using Stmt = std::variant<Return>;

struct Function {
public:
  Function(const std::string &name, std::unique_ptr<Stmt> body);

public:
  bool operator==(const Function &other) const;
  bool operator!=(const Function &other) const;
  void print(std::ostream &out, size_t indent) const;

  const std::string &getName() const;
  const std::unique_ptr<Stmt> &getBody() const;

private:
  std::string name;
  std::unique_ptr<Stmt> body;
};

std::ostream &operator<<(std::ostream &out, const Function &func);

using Program = Function;
} // namespace ast
} // namespace bcc

#endif // !BCC_AST_HPP
