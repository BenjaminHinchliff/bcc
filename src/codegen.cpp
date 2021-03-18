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
          [&](const UnaryOperator &unOp) {
            switch (unOp.getKind()) {
            case UnaryOperator::Kind::Negate:
              codegenExpr(out, *unOp.getExpr());
              out << "\tneg\t%rax\n";
              break;
            case UnaryOperator::Kind::BitwiseNot:
              codegenExpr(out, *unOp.getExpr());
              out << "\tnot\t%rax\n";
              break;
            case UnaryOperator::Kind::LogicalNot:
              codegenExpr(out, *unOp.getExpr());
              out << "\tcmp\t$0, %rax\n";
              out << "\tmov\t$0, %rax\n";
              out << "\tsete\t%al\n";
              break;
            default:
              throw std::runtime_error("unknown operator kind");
            }
          },
          [&](const BinaryOperator &binOp) {
            switch (binOp.getKind()) {
            case BinaryOperator::Kind::Addition:
              codegenExpr(out, *binOp.getLhs());
              out << "\tpush\t%rax\n";
              codegenExpr(out, *binOp.getRhs());
              out << "\tpop\t%rcx\n";
              out << "\tadd\t%rcx, %rax\n";
              break;
            case BinaryOperator::Kind::Subtraction:
              codegenExpr(out, *binOp.getLhs());
              out << "\tpush\t%rax\n";
              codegenExpr(out, *binOp.getRhs());
              out << "\tmov\t%rax, %rcx\n";
              out << "\tpop\t%rax\n";
              out << "\tsub\t%rcx, %rax\n";
              break;
            case BinaryOperator::Kind::Multiplication:
              codegenExpr(out, *binOp.getLhs());
              out << "\tpush\t%rax\n";
              codegenExpr(out, *binOp.getRhs());
              out << "\tpop\t%rcx\n";
              out << "\timul\t%rcx, %rax\n";
              break;
            case BinaryOperator::Kind::Division:
              codegenExpr(out, *binOp.getLhs());
              out << "\tpush\t%rax\n";
              codegenExpr(out, *binOp.getRhs());
              out << "\tmov\t%rax, %rcx\n";
              out << "\tpop\t%rax\n";
              out << "\tcqo\n";
              out << "\tidiv\t%rcx\n";
              break;
            default:
              throw std::runtime_error("unknown operator kind");
            }
          },
          [&](const auto &) { throw std::runtime_error("unimplemented"); }},
      expr);
}

void codegenStmt(std::ostream &out, const Stmt &stmt) {
  std::visit(
      [&](const Return &ret) {
        codegenExpr(out, *ret.getExpr());
        out << "\tret\n";
      },
      stmt);
}

void codegenFunction(std::ostream &out, const Function &function) {
  const std::string &name = function.getName();
  // declare function
#ifdef _WIN32
  out << "\t.def\t" << name << ";\n";
#endif // _WIN32
  out << "\t.globl\t" << name << '\n';
  out << name << ":\n";
  codegenStmt(out, *function.getBody());
}

void codegen(std::ostream &out, const Program &program) {
  codegenFunction(out, program);
}
} // namespace codegen
} // namespace bcc
