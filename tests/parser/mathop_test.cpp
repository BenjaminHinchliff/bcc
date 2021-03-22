#include <catch2/catch.hpp>

#include <bcc/bcc.hpp>

using namespace bcc::ast;

TEST_CASE("add parses", "[parser]") {
  std::string source{R"(
int main() {
    return 1 + 2;
}
)"};
  auto ast = bcc::parser::parse(source);
  using namespace bcc::ast;
  Program target(
      "main",
      std::make_unique<Stmt>(Return(std::make_unique<Expr>(BinaryOperator(
          BinaryOperator::Kind::Addition, std::make_unique<Expr>(Constant(1)),
          std::make_unique<Expr>(Constant(2)))))));
}

TEST_CASE("subtraction parses", "[parser]") {
  std::string source{R"(
int main() {
    return 2 - 1;
}
)"};
  auto ast = bcc::parser::parse(source);
  using namespace bcc::ast;
  Program target("main",
                 std::make_unique<Stmt>(Return(std::make_unique<Expr>(
                     BinaryOperator(BinaryOperator::Kind::Subtraction,
                                    std::make_unique<Expr>(Constant(2)),
                                    std::make_unique<Expr>(Constant(1)))))));
}

TEST_CASE("multiplication parses", "[parser]") {
  std::string source{R"(
int main() {
    return 2 * 3;
}
)"};
  auto ast = bcc::parser::parse(source);
  using namespace bcc::ast;
  Program target("main",
                 std::make_unique<Stmt>(Return(std::make_unique<Expr>(
                     BinaryOperator(BinaryOperator::Kind::Multiplication,
                                    std::make_unique<Expr>(Constant(2)),
                                    std::make_unique<Expr>(Constant(3)))))));
}

TEST_CASE("division parses", "[parser]") {
  std::string source{R"(
int main() {
    return 3 / 2;
}
)"};
  auto ast = bcc::parser::parse(source);
  using namespace bcc::ast;
  Program target(
      "main",
      std::make_unique<Stmt>(Return(std::make_unique<Expr>(BinaryOperator(
          BinaryOperator::Kind::Division, std::make_unique<Expr>(Constant(3)),
          std::make_unique<Expr>(Constant(2)))))));
}

TEST_CASE("modulo parses", "[parser]") {
  std::string source{R"(
int main() {
    return 3 % 2;
}
)"};
  auto ast = bcc::parser::parse(source);
  using namespace bcc::ast;
  Program target(
      "main",
      std::make_unique<Stmt>(Return(std::make_unique<Expr>(BinaryOperator(
          BinaryOperator::Kind::Modulo, std::make_unique<Expr>(Constant(3)),
          std::make_unique<Expr>(Constant(2)))))));
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

TEST_CASE("math op precidence parsing", "[parser]") {
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
