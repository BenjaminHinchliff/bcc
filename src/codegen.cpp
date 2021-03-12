#include "tcc/codegen.hpp"

namespace tcc {
namespace codegen {
using namespace ast;

void codegenExpr(std::ostream &out, const Expr &expr) {
  std::visit([&](const Constant &c) { out << '$' << c.val; }, expr);
}

void codegenStmt(std::ostream &out, const Stmt &stmt) {
  std::visit(
      [&](const Return &ret) {
        out << "\tmovl\t";
        codegenExpr(out, ret.expr);
        out << ", %eax\n";
        out << "\tretq\n";
      },
      stmt);
}

void codegenFunction(std::ostream &out, const Function &function) {
  const std::string &name = function.name;
  // declare function
#ifdef _WIN32
  out << "\t.def\t" << name << ";\n";
#endif // _WIN32
  out << "\t.globl\t" << name << '\n';
  out << name << ":\n";
  codegenStmt(out, function.body);
}

void codegen(std::ostream &out, const Program &program) {
  codegenFunction(out, program);
}
} // namespace codegen
} // namespace tcc
