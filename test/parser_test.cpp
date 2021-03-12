#include <catch2/catch.hpp>
#include <tcc/tcc.hpp>

using namespace tcc::parser::ast;

TEST_CASE("parser tests", "[parser]") {
  SECTION("return 2") {
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
}
