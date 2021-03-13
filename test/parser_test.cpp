#include <catch2/catch.hpp>
#include <iostream>
#include <tcc/tcc.hpp>

using namespace tcc::ast;

TEST_CASE("return 2 parses", "[parser]") {
  std::string source{R"#(
int main() {
    return 2;
}
)#"};
  auto tokens = tcc::lexer::lex(source);
  auto ast = tcc::parser::parse(tokens);
  Program target{"main", Stmt{Return{Constant{2}}}};
  REQUIRE(ast == target);
}
