#include "bcc/ast.hpp"

namespace bcc {
namespace ast {
// overloaded lambda helper
template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

void printHelper(const Expr &expr, std::ostream &out, size_t indent) {
  std::visit(overloaded{[&](const auto &ex) { ex.print(out, indent); }}, expr);
}

std::string createIndent(size_t indent) { return std::string(indent, ' '); }

bool Constant::operator==(const Constant &other) const {
  return val == other.val;
}
bool Constant::operator!=(const Constant &other) const {
  return !(*this == other);
}
void Constant::print(std::ostream &out, size_t indent) const {
  out << "Constant{" << val << "}\n";
}

UnaryOperator::UnaryOperator(Kind kind, std::unique_ptr<Expr> expr)
    : kind(kind), expr(std::move(expr)) {}

bool UnaryOperator::operator==(const UnaryOperator &other) const {
  return kind == other.kind && *expr == *other.expr;
}

bool UnaryOperator::operator!=(const UnaryOperator &other) const {
  return !(*this == other);
}

void UnaryOperator::print(std::ostream &out, size_t indent) const {
  switch (kind) {
  case Kind::BitwiseNot:
    out << "BitwiseNot";
    break;
  case Kind::LogicalNot:
    out << "LogicalNot";
    break;
  case Kind::Negate:
    out << "Negate";
    break;
  default:
    throw std::runtime_error("unknown binary operator kind");
  }
  out << "{\n";
  out << createIndent(indent + 4);
  printHelper(*expr, out, indent + 4);
  out << createIndent(indent) << "}\n";
}

BinaryOperator::BinaryOperator(Kind kind, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs)
    : kind(kind), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

bool BinaryOperator::operator==(const BinaryOperator &other) const {
  return kind == other.kind && *lhs == *other.lhs && *rhs == *other.rhs;
}

bool BinaryOperator::operator!=(const BinaryOperator &other) const {
  return !(*this == other);
}

void BinaryOperator::print(std::ostream &out, size_t indent) const {
  switch (kind) {
  case Kind::Addition:
    out << "Addition";
    break;
  case Kind::Subtraction:
    out << "Subtraction";
    break;
  case Kind::Multiplication:
    out << "Multiplication";
    break;
  case Kind::Division:
    out << "Division";
    break;
  default:
    throw std::runtime_error("unknown binary operator kind");
  }
  out << "{\n";
  out << createIndent(indent + 4) << "lhs: ";
  printHelper(*lhs, out, indent + 4);
  out << createIndent(indent + 4) << "rhs: ";
  printHelper(*rhs, out, indent + 4);
  out << createIndent(indent) << "}\n";
}

bool Return::operator==(const Return &other) const {
  return *expr == *other.expr;
}
bool Return::operator!=(const Return &other) const { return !(*this == other); }
void Return::print(std::ostream &out, size_t indent) const {
  out << "Return{\n";
  out << createIndent(indent + 4);
  printHelper(*expr, out, indent + 4);
  out << createIndent(indent) << "}\n";
}

bool Function::operator==(const Function &other) const {
  return name == other.name && body == other.body;
}
bool Function::operator!=(const Function &other) const {
  return !(*this == other);
}
void Function::print(std::ostream &out, size_t indent) const {
  out << createIndent(indent) << "Function{\n";
  out << createIndent(indent + 4) << "name: " << name << '\n';
  out << createIndent(indent + 4) << "body: ";
  std::visit([&](const auto &e) { e.print(out, indent + 4); }, body);
  out << createIndent(indent) << "}\n";
}

std::ostream &operator<<(std::ostream &out, const Function &func) {
  func.print(out, 0);
  return out;
}
} // namespace ast
} // namespace bcc
