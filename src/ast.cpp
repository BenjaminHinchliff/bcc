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

void Addition::print(std::ostream &out, size_t indent) const {
  out << "Addition{\n";
  out << createIndent(indent + 4) << "lhs: ";
  printHelper(*lhs, out, indent + 4);
  out << createIndent(indent + 4) << "rhs: ";
  printHelper(*rhs, out, indent + 4);
  out << createIndent(indent) << "}\n";
}

void Subtraction::print(std::ostream &out, size_t indent) const {
  out << "Subtraction{\n";
  out << createIndent(indent + 4) << "lhs: ";
  printHelper(*lhs, out, indent + 4);
  out << createIndent(indent + 4) << "rhs: ";
  printHelper(*rhs, out, indent + 4);
  out << createIndent(indent) << "}\n";
}

void Multiplication::print(std::ostream &out, size_t indent) const {
  out << "Multiplication{\n";
  out << createIndent(indent + 4) << "lhs: ";
  printHelper(*lhs, out, indent + 4);
  out << createIndent(indent + 4) << "rhs: ";
  printHelper(*rhs, out, indent + 4);
  out << createIndent(indent) << "}\n";
}

void Division::print(std::ostream &out, size_t indent) const {
  out << "Division{\n";
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
