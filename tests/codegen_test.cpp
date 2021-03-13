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
      "\t.globl\tmain\nmain:\n\tmovl\t$2, %eax\n\tretq\n");
#ifdef _WIN32
  target.insert(0, "\t.def\tmain;\n");
#endif // _WIN32
  REQUIRE(ss.str() == target);
}
