#include "bcc/codegen.hpp"

namespace bcc {
namespace codegen {
using namespace ast;

// overloaded lambda helper
template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

void codegenExpr(std::ostream &out, const Expr &expr, size_t &labelCounter);

void baseMathOpCodegen(std::ostream &out, const BinaryOperator &binOp,
                       size_t &labelCounter, bool swapOperands = false) {
  codegenExpr(out, *binOp.getLhs(), labelCounter);
  out << "\tpush\t%rax\n";
  codegenExpr(out, *binOp.getRhs(), labelCounter);
  if (swapOperands) {
    out << "\tmov\t%rax, %rcx\n";
    out << "\tpop\t%rax\n";
  } else {
    out << "\tpop\t%rcx\n";
  }
}

void baseCmpOpCodegen(std::ostream &out, const BinaryOperator &binOp,
                      size_t &labelCounter) {
  baseMathOpCodegen(out, binOp, labelCounter);
  out << "\tcmp\t%rax, %rcx\n";
  out << "\tmov\t$0, %rax\n";
}

std::string makeLabel(size_t labelCounter) {
  return "_clause" + std::to_string(labelCounter);
}

void codegenExpr(std::ostream &out, const Expr &expr, size_t &labelCounter) {
  std::visit(
      overloaded{
          [&](const Constant &c) { out << "\tmov\t$" << c.val << ", %rax\n"; },
          [&](const UnaryOperator &unOp) {
            switch (unOp.getKind()) {
            case UnaryOperator::Kind::Negate:
              codegenExpr(out, *unOp.getExpr(), labelCounter);
              out << "\tneg\t%rax\n";
              break;
            case UnaryOperator::Kind::BitwiseNot:
              codegenExpr(out, *unOp.getExpr(), labelCounter);
              out << "\tnot\t%rax\n";
              break;
            case UnaryOperator::Kind::LogicalNot:
              codegenExpr(out, *unOp.getExpr(), labelCounter);
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
              baseMathOpCodegen(out, binOp, labelCounter);
              out << "\tadd\t%rcx, %rax\n";
              break;
            case BinaryOperator::Kind::Subtraction:
              baseMathOpCodegen(out, binOp, labelCounter, true);
              out << "\tsub\t%rcx, %rax\n";
              break;
            case BinaryOperator::Kind::Multiplication:
              baseMathOpCodegen(out, binOp, labelCounter);
              out << "\timul\t%rcx, %rax\n";
              break;
            case BinaryOperator::Kind::Division:
              baseMathOpCodegen(out, binOp, labelCounter, true);
              out << "\tcqo\n";
              out << "\tidiv\t%rcx\n";
              break;
            case BinaryOperator::Kind::Modulo:
              baseMathOpCodegen(out, binOp, labelCounter, true);
              out << "\tcqo\n";
              out << "\tidiv\t%rcx\n";
              out << "\tmov\t%rdx, %rax\n";
              break;
            case BinaryOperator::Kind::Equal:
              baseMathOpCodegen(out, binOp, labelCounter);
              out << "\tcmp\t%rax, %rcx\n";
              out << "\tmov\t$0, %rax\n";
              out << "\tsete\t%al\n";
              break;
            case BinaryOperator::Kind::NotEqual:
              baseCmpOpCodegen(out, binOp, labelCounter);
              out << "\tsetne\t%al\n";
              break;
            case BinaryOperator::Kind::GreaterThan:
              baseCmpOpCodegen(out, binOp, labelCounter);
              out << "\tsetg\t%al\n";
              break;
            case BinaryOperator::Kind::GreaterThanOrEqual:
              baseCmpOpCodegen(out, binOp, labelCounter);
              out << "\tsetge\t%al\n";
              break;
            case BinaryOperator::Kind::LessThan:
              baseCmpOpCodegen(out, binOp, labelCounter);
              out << "\tsetl\t%al\n";
              break;
            case BinaryOperator::Kind::LessThanOrEqual:
              baseCmpOpCodegen(out, binOp, labelCounter);
              out << "\tsetle\t%al\n";
              break;
            case BinaryOperator::Kind::LogicalOr: {
              codegenExpr(out, *binOp.getLhs(), labelCounter);
              out << "\tcmp\t$0, %rax\n";
              const std::string rightClause{makeLabel(labelCounter++)};
              out << "\tje\t" << rightClause << '\n';
              out << "\tmov\t$1, %rax\n";
              const std::string endClause{makeLabel(labelCounter++)};
              out << "\tjmp\t" << endClause << '\n';
              out << rightClause << ":\n";
              codegenExpr(out, *binOp.getRhs(), labelCounter);
              out << "\tcmp\t$0, %rax\n";
              out << "\tmov\t$0, %rax\n";
              out << "\tsetne\t%al\n";
              out << endClause << ":\n";
            } break;
            case BinaryOperator::Kind::LogicalAnd: {
              codegenExpr(out, *binOp.getLhs(), labelCounter);
              out << "\tcmp\t$0, %rax\n";
              const std::string rightClause{makeLabel(labelCounter++)};
              out << "\tjne\t" << rightClause << '\n';
              const std::string endClause{makeLabel(labelCounter++)};
              out << "\tjmp\t" << endClause << '\n';
              out << rightClause << ":\n";
              codegenExpr(out, *binOp.getRhs(), labelCounter);
              out << "\tcmp\t$0, %rax\n";
              out << "\tmov\t$0, %rax\n";
              out << "\tsetne\t%al\n";
              out << endClause << ":\n";
            } break;
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
        size_t labelCounter = 0;
        codegenExpr(out, *ret.getExpr(), labelCounter);
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
