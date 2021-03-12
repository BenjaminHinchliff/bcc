#include "tcc/ast.hpp"

namespace tcc {
namespace ast {
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

bool Return::operator==(const Return &other) const {
  return expr == other.expr;
}
bool Return::operator!=(const Return &other) const { return !(*this == other); }
void Return::print(std::ostream &out, size_t indent) const {
  out << "Return{\n";
  out << createIndent(indent + 4);
  std::visit([&](const auto &e) { e.print(out, indent + 4); }, expr);
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
} // namespace tcc
