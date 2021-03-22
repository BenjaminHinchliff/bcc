#include <bcc/bcc.hpp>
#include <catch2/catch.hpp>
#include <iostream>

using namespace bcc::ast;

TEST_CASE("return 2 parses", "[parser]") {
  SECTION("valid program") {
    std::string source{R"#(
int main() {
    return 2;
}
)#"};
    auto ast = bcc::parser::parse(source);
    Program target("main", std::make_unique<Stmt>(
                               Return(std::make_unique<Expr>(Constant(2)))));
    REQUIRE(ast == target);
  }

  SECTION("invalid programs") {
    std::vector<std::string> sources = {
        R"(
int main( {
    return 0;
}
)",
        R"(
int main() {
    return;
}
)",
        R"(
int main() {
    return 0;
)",
        R"(
int main() {
    return 0
}
)",
        R"(
int main() {
    return0;
}
)", R"(
int main() {
    RETURN 0;
}
)"};
    for (const std::string &source : sources) {
      REQUIRE_THROWS_AS(bcc::parser::parse(source), bcc::parser::Exception);
    }
  }
}

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

TEST_CASE("operators parse", "[parser]") {
  std::string source{R"(
int main() {
    return 2 + 3 * 4 - 2 / 2;
}
)"};
  auto ast = bcc::parser::parse(source);
  using namespace bcc::ast;
  Program target(
      "main",
      std::make_unique<Stmt>(Return(std::make_unique<Expr>(
          BinaryOperator(BinaryOperator::Kind::Subtraction,
                         std::make_unique<Expr>(BinaryOperator(
                             BinaryOperator::Kind::Addition,
                             std::make_unique<Expr>(Constant(2)),
                             std::make_unique<Expr>(BinaryOperator(
                                 BinaryOperator::Kind::Multiplication,
                                 std::make_unique<Expr>(Constant(3)),
                                 std::make_unique<Expr>(Constant(4)))))),
                         std::make_unique<Expr>(BinaryOperator(
                             BinaryOperator::Kind::Division,
                             std::make_unique<Expr>(Constant(2)),
                             std::make_unique<Expr>(Constant(2)))))))));
  REQUIRE(ast == target);
}

TEST_CASE("unary and binary ops parse", "[parser]") {
  std::string source{R"(
int main() {
    return ~2 + 3;
}
)"};
  auto ast = bcc::parser::parse(source);
  using namespace bcc::ast;
  Program target("main",
                 std::make_unique<Stmt>(Return(std::make_unique<Expr>(
                     BinaryOperator(BinaryOperator::Kind::Addition,
                                    std::make_unique<Expr>(UnaryOperator(
                                        UnaryOperator::Kind::BitwiseNot,
                                        std::make_unique<Expr>(Constant(2)))),
                                    std::make_unique<Expr>(Constant(3)))))));
  REQUIRE(ast == target);
}

TEST_CASE("associativity works", "[parser]") {
  std::string source{R"(
int main() {
    return 6 / 3 / 2;
}
)"};
  auto ast = bcc::parser::parse(source);
  using namespace bcc::ast;
  Program target("main",
                 std::make_unique<Stmt>(Return(std::make_unique<Expr>(
                     BinaryOperator(BinaryOperator::Kind::Division,
                                    std::make_unique<Expr>(BinaryOperator(
                                        BinaryOperator::Kind::Division,
                                        std::make_unique<Expr>(Constant(6)),
                                        std::make_unique<Expr>(Constant(3)))),
                                    std::make_unique<Expr>(Constant(2)))))));
  REQUIRE(ast == target);
}

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
