#include <sstream>
#include <iostream>

#include <catch2/catch.hpp>
#include <tcc/tcc.hpp>

TEST_CASE("codegen tests", "[codegen]") {
  SECTION("return 2") {
    std::string source{R"#(
int main() {
    return 2;
}
)#"};
    auto tokens = tcc::lexer::lex(source);
    auto ast = tcc::parser::parse(tokens);
    std::stringstream ss;
    tcc::codegen::codegen(ss, ast);
    std::string target("\n\t.def\tmain;\n\t.globl\tmain\nmain:\n\tmovl\t$2, %eax\n\tretq\n");
    REQUIRE(("\n" + ss.str()) == target);
  }
}
