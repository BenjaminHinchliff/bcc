#ifndef TCC_PARSER_HPP
#define TCC_PARSER_HPP

#include <cassert>
#include <string>
#include <variant>

#include "lexer.hpp"

namespace tcc {
namespace parser {
namespace ast {
struct Constant {
  int val;

  bool operator==(const Constant &other) const { return val == other.val; }
};

using Expr = std::variant<Constant>;

struct Return {
  Expr expr;

  bool operator==(const Return &other) const { return expr == other.expr; }
};

using Stmt = std::variant<Return>;

struct Function {
  std::string name;
  Stmt body;

  bool operator==(const Function &other) const {
    return name == other.name && body == other.body;
  }
};

using Program = Function;
} // namespace ast

using ast::Program;

Program parse(const lexer::Tokens &tokens);
} // namespace parser
} // namespace tcc

#endif // !TCC_PARSER_HPP
