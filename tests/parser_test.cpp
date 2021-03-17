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
    Program target{"main", Stmt{Return{std::make_unique<Expr>(Constant{2})}}};
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
  Program target{"main", Return{std::make_unique<Expr>(UnaryOperator(
                             UnaryOperator::Kind::Negate,
                             std::make_unique<Expr>(Constant{5})))}};
  REQUIRE(ast == target);
}

TEST_CASE("bitwise complement parses", "[parser]") {
  std::string source{R"#(
int main() {
    return ~12;
}
)#"};
  auto ast = bcc::parser::parse(source);
  Program target{"main", Return{std::make_unique<Expr>(UnaryOperator(
                             UnaryOperator::Kind::BitwiseNot,
                             std::make_unique<Expr>(Constant{12})))}};
  REQUIRE(ast == target);
}

TEST_CASE("logical not parses", "[parser]") {
  std::string source{R"#(
int main() {
    return !0;
}
)#"};
  auto ast = bcc::parser::parse(source);
  Program target{"main", Return{std::make_unique<Expr>(UnaryOperator(
                             UnaryOperator::Kind::LogicalNot,
                             std::make_unique<Expr>(Constant{0})))}};
  REQUIRE(ast == target);
}

TEST_CASE("nested ops parse", "[parser]") {
  std::string source{R"#(
int main() {
    return !-~0;
}
)#"};
  auto ast = bcc::parser::parse(source);
  Program target{"main",
                 Return{std::make_unique<Expr>(UnaryOperator(
                     UnaryOperator::Kind::LogicalNot,
                     std::make_unique<Expr>(UnaryOperator(
                         UnaryOperator::Kind::Negate,
                         std::make_unique<Expr>(UnaryOperator(
                             UnaryOperator::Kind::BitwiseNot,
                             std::make_unique<Expr>(Constant{0})))))))}};
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
  Program target{"main", Return{std::make_unique<Expr>(BinaryOperator(
                             BinaryOperator::Kind::Subtraction,
                             std::make_unique<Expr>(BinaryOperator(
                                 BinaryOperator::Kind::Addition,
                                 std::make_unique<Expr>(Constant{2}),
                                 std::make_unique<Expr>(BinaryOperator(
                                     BinaryOperator::Kind::Multiplication,
                                     std::make_unique<Expr>(Constant{3}),
                                     std::make_unique<Expr>(Constant{4}))))),
                             std::make_unique<Expr>(BinaryOperator(
                                 BinaryOperator::Kind::Division,
                                 std::make_unique<Expr>(Constant{2}),
                                 std::make_unique<Expr>(Constant{2})))))}};
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
  Program target{"main", Return{std::make_unique<Expr>(BinaryOperator(
                             BinaryOperator::Kind::Addition,
                             std::make_unique<Expr>(UnaryOperator(
                                 UnaryOperator::Kind::BitwiseNot,
                                 std::make_unique<Expr>(Constant{2}))),
                             std::make_unique<Expr>(Constant{3})))}};
  REQUIRE(ast == target);
}
