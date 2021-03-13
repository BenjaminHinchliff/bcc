#include <sstream>

#include <catch2/catch.hpp>
#include <tcc/tcc.hpp>

TEST_CASE("return 2 codegens", "[codegen]") {
  std::string source{R"#(
int main() {
    return 2;
}
)#"};
  auto ast = tcc::parser::parse(source);
  std::stringstream ss;
  tcc::codegen::codegen(ss, ast);
  std::string target(
      "\n\t.def\tmain;\n\t.globl\tmain\nmain:\n\tmovl\t$2, %eax\n\tretq\n");
  REQUIRE(("\n" + ss.str()) == target);
}
