#include <sstream>

#include <bcc/bcc.hpp>
#include <catch2/catch.hpp>

TEST_CASE("logical or codegens", "[codegen]") {
  std::string source{R"#(
int main() {
    return 0 || 1;
}
)#"};
  auto ast = bcc::parser::parse(source);
  std::stringstream ss;
  bcc::codegen::codegen(ss, ast);
  std::string target("\t.globl\tmain\n"
                     "main:\n"
                     "\tmov\t$0, %rax\n"
                     "\tcmp\t$0, %rax\n"
                     "\tje\t_clause0\n"
                     "\tmov\t$1, %rax\n"
                     "\tjmp\t_clause1\n"
                     "_clause0:\n"
                     "\tmov\t$1, %rax\n"
                     "\tcmp\t$0, %rax\n"
                     "\tmov\t$0, %rax\n"
                     "\tsetne\t%al\n"
                     "_clause1:\n"
                     "\tret\n");
#ifdef _WIN32
  target.insert(0, "\t.def\tmain;\n");
#endif // _WIN32
  REQUIRE(ss.str() == target);
}

TEST_CASE("logical and codegens", "[codegen]") {
  std::string source{R"#(
int main() {
    return 0 && 1;
}
)#"};
  auto ast = bcc::parser::parse(source);
  std::stringstream ss;
  bcc::codegen::codegen(ss, ast);
  std::string target("\t.globl\tmain\n"
                     "main:\n"
                     "\tmov\t$0, %rax\n"
                     "\tcmp\t$0, %rax\n"
                     "\tjne\t_clause0\n"
                     "\tjmp\t_clause1\n"
                     "_clause0:\n"
                     "\tmov\t$1, %rax\n"
                     "\tcmp\t$0, %rax\n"
                     "\tmov\t$0, %rax\n"
                     "\tsetne\t%al\n"
                     "_clause1:\n"
                     "\tret\n");
#ifdef _WIN32
  target.insert(0, "\t.def\tmain;\n");
#endif // _WIN32
  REQUIRE(ss.str() == target);
}