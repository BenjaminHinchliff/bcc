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
    Program target{"main", Stmt{Return{Constant{2}}}};
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
  Program target{"main", Return{Negation(std::make_unique<Expr>(Constant{5}))}};
  REQUIRE(ast == target);
}

TEST_CASE("bitwise complement parses", "[parser]") {
  std::string source{R"#(
int main() {
    return ~12;
}
)#"};
  auto ast = bcc::parser::parse(source);
  Program target{
      "main", Return{BitwiseComplement(std::make_unique<Expr>(Constant{12}))}};
  REQUIRE(ast == target);
}

TEST_CASE("logical not parses", "[parser]") {
  std::string source{R"#(
int main() {
    return !0;
}
)#"};
  auto ast = bcc::parser::parse(source);
  Program target{"main", Return{Not(std::make_unique<Expr>(Constant{0}))}};
  REQUIRE(ast == target);
}

TEST_CASE("nested ops parse", "[parser]") {
  std::string source{R"#(
int main() {
    return !-~0;
}
)#"};
  auto ast = bcc::parser::parse(source);
  Program target{
      "main", Return{Not(std::make_unique<Expr>(Negation(std::make_unique<Expr>(
                  BitwiseComplement(std::make_unique<Expr>(Constant{0}))))))}};
  REQUIRE(ast == target);
}
