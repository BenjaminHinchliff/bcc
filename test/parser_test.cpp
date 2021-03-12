#include <catch2/catch.hpp>
#include <tcc/tcc.hpp>
#include <iostream>

using namespace tcc::ast;

TEST_CASE("parser tests", "[parser]") {
  SECTION("return 2") {
    std::string source{R"#(
int main() {
    return 2;
}
)#"};
    auto tokens = tcc::lexer::lex(source);
    auto ast = tcc::parser::parse(tokens);
    std::cout << ast << '\n';
    Program target{"main", Stmt{Return{Constant{2}}}};
    REQUIRE(ast == target);
  }
}
