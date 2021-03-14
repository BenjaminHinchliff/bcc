#include "bcc/ast.hpp"

namespace bcc {
namespace ast {
// overloaded lambda helper
template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

void printHelper(const Expr &expr, std::ostream &out, size_t indent) {
  std::visit(overloaded{[&](const UnaryOperator &op) {
                          std::visit(
                              [&](const auto &ex) { ex.print(out, indent); },
                              op);
                        },
                        [&](const auto &ex) { ex.print(out, indent); }},
             expr);
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

UnaryOperatorBase::UnaryOperatorBase(std::unique_ptr<Expr> expr)
    : expr(std::move(expr)) {}
bool UnaryOperatorBase::operator==(const UnaryOperatorBase &other) const {
  return *expr == *other.expr;
}
bool UnaryOperatorBase::operator!=(const UnaryOperatorBase &other) const {
  return !(*this == other);
}

void Negation::print(std::ostream &out, size_t indent) const {
  out << "Negation{\n";
  out << createIndent(indent + 4);
  printHelper(*expr, out, indent + 4);
  out << createIndent(indent) << "}\n";
}

void BitwiseComplement::print(std::ostream &out, size_t indent) const {
  out << "BitwiseComplement{\n";
  out << createIndent(indent + 4);
  printHelper(*expr, out, indent + 4);
  out << createIndent(indent) << "}\n";
}

void Not::print(std::ostream &out, size_t indent) const {
  out << "Not{\n";
  out << createIndent(indent + 4);
  printHelper(*expr, out, indent + 4);
  out << createIndent(indent) << "}\n";
}

bool Return::operator==(const Return &other) const {
  return expr == other.expr;
}
bool Return::operator!=(const Return &other) const { return !(*this == other); }
void Return::print(std::ostream &out, size_t indent) const {
  out << "Return{\n";
  out << createIndent(indent + 4);
  printHelper(expr, out, indent + 4);
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
