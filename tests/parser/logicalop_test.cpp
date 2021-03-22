#include <catch2/catch.hpp>

#include <bcc/bcc.hpp>

TEST_CASE("and parses", "[parser]") {
  std::string source{R"(
int main() {
    return 1 && 0;
}
)"};
  auto ast = bcc::parser::parse(source);
  using namespace bcc::ast;
  Program target(
      "main",
      std::make_unique<Stmt>(Return(std::make_unique<Expr>(BinaryOperator(
          BinaryOperator::Kind::LogicalAnd, std::make_unique<Expr>(Constant(1)),
          std::make_unique<Expr>(Constant(0)))))));
  REQUIRE(ast == target);
}

TEST_CASE("or parses", "[parser]") {
  std::string source{R"(
int main() {
    return 1 || 0;
}
)"};
  auto ast = bcc::parser::parse(source);
  using namespace bcc::ast;
  Program target(
      "main",
      std::make_unique<Stmt>(Return(std::make_unique<Expr>(BinaryOperator(
          BinaryOperator::Kind::LogicalOr, std::make_unique<Expr>(Constant(1)),
          std::make_unique<Expr>(Constant(0)))))));
  REQUIRE(ast == target);
}
