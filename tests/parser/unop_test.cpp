#include <catch2/catch.hpp>

#include <bcc/bcc.hpp>

using namespace bcc::ast;

TEST_CASE("negate parses", "[parser]") {
  std::string source{R"#(
int main() {
    return -5;
}
)#"};
  auto ast = bcc::parser::parse(source);
  Program target(
      "main",
      std::make_unique<Stmt>(Return(std::make_unique<Expr>(UnaryOperator(
          UnaryOperator::Kind::Negate, std::make_unique<Expr>(Constant(5)))))));
  REQUIRE(ast == target);
}

TEST_CASE("bitwise complement parses", "[parser]") {
  std::string source{R"#(
int main() {
    return ~12;
}
)#"};
  auto ast = bcc::parser::parse(source);
  Program target("main",
                 std::make_unique<Stmt>(Return(std::make_unique<Expr>(
                     UnaryOperator(UnaryOperator::Kind::BitwiseNot,
                                   std::make_unique<Expr>(Constant(12)))))));
  REQUIRE(ast == target);
}

TEST_CASE("logical not parses", "[parser]") {
  std::string source{R"#(
int main() {
    return !0;
}
)#"};
  auto ast = bcc::parser::parse(source);
  Program target("main",
                 std::make_unique<Stmt>(Return(std::make_unique<Expr>(
                     UnaryOperator(UnaryOperator::Kind::LogicalNot,
                                   std::make_unique<Expr>(Constant(0)))))));
  REQUIRE(ast == target);
}

TEST_CASE("nested ops parse", "[parser]") {
  std::string source{R"#(
int main() {
    return !-~0;
}
)#"};
  auto ast = bcc::parser::parse(source);
  Program target(
      "main",
      std::make_unique<Stmt>(Return(std::make_unique<Expr>(
          UnaryOperator(UnaryOperator::Kind::LogicalNot,
                        std::make_unique<Expr>(UnaryOperator(
                            UnaryOperator::Kind::Negate,
                            std::make_unique<Expr>(UnaryOperator(
                                UnaryOperator::Kind::BitwiseNot,
                                std::make_unique<Expr>(Constant(0)))))))))));
  REQUIRE(ast == target);
}