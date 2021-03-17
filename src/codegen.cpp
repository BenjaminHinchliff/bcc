#include "bcc/codegen.hpp"

namespace bcc {
namespace codegen {
using namespace ast;

// overloaded lambda helper
template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

void codegenExpr(std::ostream &out, const Expr &expr) {
  std::visit(
      overloaded{
          [&](const Constant &c) { out << "\tmov\t$" << c.val << ", %rax\n"; },
          [&](const Negation &neg) {
            codegenExpr(out, *neg.expr);
            out << "\tneg\t%rax\n";
          },
          [&](const BitwiseComplement &bits) {
            codegenExpr(out, *bits.expr);
            out << "\tnot\t%rax\n";
          },
          [&](const Not &uNot) {
            codegenExpr(out, *uNot.expr);
            out << "\tcmp\t$0, %rax\n";
            out << "\tmov\t$0, %rax\n";
            out << "\tsete\t%al\n";
          },
          [&](const Addition &add) {
            codegenExpr(out, *add.lhs);
            out << "\tpush\t%rax\n";
            codegenExpr(out, *add.rhs);
            out << "\tpop\t%rcx\n";
            out << "\tadd\t%rcx, %rax\n";
          },
          [&](const Subtraction &sub) {
            codegenExpr(out, *sub.lhs);
            out << "\tpush\t%rax\n";
            codegenExpr(out, *sub.rhs);
            out << "\tmov\t%rax, %rcx\n";
            out << "\tpop\t%rax\n";
            out << "\tsub\t%rcx, %rax\n";
          },
          [&](const Multiplication &mult) {
            codegenExpr(out, *mult.lhs);
            out << "\tpush\t%rax\n";
            codegenExpr(out, *mult.rhs);
            out << "\tpop\t%rcx\n";
            out << "\timul\t%rcx, %rax\n";
          },
          [&](const auto &) { throw std::runtime_error("unimplemented"); }},
      expr);
}

void codegenStmt(std::ostream &out, const Stmt &stmt) {
  std::visit(
      [&](const Return &ret) {
        codegenExpr(out, *ret.expr);
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
