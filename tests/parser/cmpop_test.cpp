#include <catch2/catch.hpp>

#include <bcc/bcc.hpp>

TEST_CASE("equal parses", "[parser]") {
  std::string source{R"(
int main() {
    return 1 == 0;
}
)"};
  auto ast = bcc::parser::parse(source);
  using namespace bcc::ast;
  Program target(
      "main",
      std::make_unique<Stmt>(Return(std::make_unique<Expr>(BinaryOperator(
          BinaryOperator::Kind::Equal, std::make_unique<Expr>(Constant(1)),
          std::make_unique<Expr>(Constant(0)))))));
  REQUIRE(ast == target);
}

TEST_CASE("not equal parses", "[parser]") {
  std::string source{R"(
int main() {
    return 1 != 0;
}
)"};
  auto ast = bcc::parser::parse(source);
  using namespace bcc::ast;
  Program target(
      "main",
      std::make_unique<Stmt>(Return(std::make_unique<Expr>(BinaryOperator(
          BinaryOperator::Kind::NotEqual, std::make_unique<Expr>(Constant(1)),
          std::make_unique<Expr>(Constant(0)))))));
  REQUIRE(ast == target);
}

TEST_CASE("less than parses", "[parser]") {
  std::string source{R"(
int main() {
    return 1 < 0;
}
)"};
  auto ast = bcc::parser::parse(source);
  using namespace bcc::ast;
  Program target(
      "main",
      std::make_unique<Stmt>(Return(std::make_unique<Expr>(BinaryOperator(
          BinaryOperator::Kind::LessThan, std::make_unique<Expr>(Constant(1)),
          std::make_unique<Expr>(Constant(0)))))));
  REQUIRE(ast == target);
}

TEST_CASE("less than or equal parses", "[parser]") {
  std::string source{R"(
int main() {
    return 1 <= 0;
}
)"};
  auto ast = bcc::parser::parse(source);
  using namespace bcc::ast;
  Program target("main",
                 std::make_unique<Stmt>(Return(std::make_unique<Expr>(
                     BinaryOperator(BinaryOperator::Kind::LessThanOrEqual,
                                    std::make_unique<Expr>(Constant(1)),
                                    std::make_unique<Expr>(Constant(0)))))));
  REQUIRE(ast == target);
}

TEST_CASE("greater than parses", "[parser]") {
  std::string source{R"(
int main() {
    return 1 > 0;
}
)"};
  auto ast = bcc::parser::parse(source);
  using namespace bcc::ast;
  Program target("main",
                 std::make_unique<Stmt>(Return(std::make_unique<Expr>(
                     BinaryOperator(BinaryOperator::Kind::GreaterThan,
                                    std::make_unique<Expr>(Constant(1)),
                                    std::make_unique<Expr>(Constant(0)))))));
  REQUIRE(ast == target);
}

TEST_CASE("greater than or equal parses", "[parser]") {
  std::string source{R"(
int main() {
    return 1 >= 0;
}
)"};
  auto ast = bcc::parser::parse(source);
  using namespace bcc::ast;
  Program target("main",
                 std::make_unique<Stmt>(Return(std::make_unique<Expr>(
                     BinaryOperator(BinaryOperator::Kind::GreaterThanOrEqual,
                                    std::make_unique<Expr>(Constant(1)),
                                    std::make_unique<Expr>(Constant(0)))))));
  REQUIRE(ast == target);
}
