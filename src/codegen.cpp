#include "bcc/codegen.hpp"

namespace bcc {
namespace codegen {
using namespace ast;

// overloaded lambda helper
template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

void codegenExpr(std::ostream &out, const Expr &expr);

void baseBinOpCodegen(std::ostream &out, const BinaryOperator &binOp,
                      bool swapOperands = false) {
  codegenExpr(out, *binOp.getLhs());
  out << "\tpush\t%rax\n";
  codegenExpr(out, *binOp.getRhs());
  if (swapOperands) {
    out << "\tmov\t%rax, %rcx\n";
    out << "\tpop\t%rax\n";
  } else {
    out << "\tpop\t%rcx\n";
  }
}

void baseCmpOpCodegen(std::ostream &out, const BinaryOperator &binOp) {
  baseBinOpCodegen(out, binOp);
  out << "\tcmp\t%rax, %rcx\n";
  out << "\tmov\t$0, %rax\n";
}

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
              baseBinOpCodegen(out, binOp);
              out << "\tadd\t%rcx, %rax\n";
              break;
            case BinaryOperator::Kind::Subtraction:
              baseBinOpCodegen(out, binOp, true);
              out << "\tsub\t%rcx, %rax\n";
              break;
            case BinaryOperator::Kind::Multiplication:
              baseBinOpCodegen(out, binOp);
              out << "\timul\t%rcx, %rax\n";
              break;
            case BinaryOperator::Kind::Division:
              baseBinOpCodegen(out, binOp, true);
              out << "\tcqo\n";
              out << "\tidiv\t%rcx\n";
              break;
            case BinaryOperator::Kind::Equal:
              baseBinOpCodegen(out, binOp);
              out << "\tcmp\t%rax, %rcx\n";
              out << "\tmov\t$0, %rax\n";
              out << "\tsete\t%al\n";
              break;
            case BinaryOperator::Kind::NotEqual:
              baseCmpOpCodegen(out, binOp);
              out << "\tsetne\t%al\n";
              break;
            case BinaryOperator::Kind::GreaterThan:
              baseCmpOpCodegen(out, binOp);
              out << "\tsetg\t%al\n";
              break;
            case BinaryOperator::Kind::GreaterThanOrEqual:
              baseCmpOpCodegen(out, binOp);
              out << "\tsetge\t%al\n";
              break;
            case BinaryOperator::Kind::LessThan:
              baseCmpOpCodegen(out, binOp);
              out << "\tsetl\t%al\n";
              break;
            case BinaryOperator::Kind::LessThanOrEqual:
              baseCmpOpCodegen(out, binOp);
              out << "\tsetle\t%al\n";
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
