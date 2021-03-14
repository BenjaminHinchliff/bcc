#include "bcc/codegen.hpp"

namespace bcc {
namespace codegen {
using namespace ast;

// overloaded lambda helper
template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

void codegenExpr(std::ostream &out, const Expr &expr);

void codegenUnary(std::ostream &out, const UnaryOperator &oper) {
  std::visit(overloaded{[&](const Negation &neg) {
                          codegenExpr(out, *neg.expr);
                          out << "\tneg\t%eax\n";
                        },
                        [&](const BitwiseComplement &bits) {
                          codegenExpr(out, *bits.expr);
                          out << "\tnot\t%eax\n";
                        },
                        [&](const Not &uNot) {
                          codegenExpr(out, *uNot.expr);
                          out << "\tcmpl\t$0, %eax\n";
                          out << "\txor\t%eax, %eax\n";
                          out << "\tsete\t%al\n";
                        }},
             oper);
}

void codegenExpr(std::ostream &out, const Expr &expr) {
  std::visit(
      overloaded{
          [&](const Constant &c) { out << "\tmovl\t$" << c.val << ", %eax\n"; },
          [&](const UnaryOperator &oper) { codegenUnary(out, oper); }},
      expr);
}

void codegenStmt(std::ostream &out, const Stmt &stmt) {
  std::visit(
      [&](const Return &ret) {
        codegenExpr(out, ret.expr);
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
} // namespace bcc
