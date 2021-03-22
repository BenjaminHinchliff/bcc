#include <sstream>

#include <bcc/bcc.hpp>
#include <catch2/catch.hpp>

TEST_CASE("negate codegens", "[codegen]") {
  std::string source{R"#(
int main() {
    return -5;
}
)#"};
  auto ast = bcc::parser::parse(source);
  std::stringstream ss;
  bcc::codegen::codegen(ss, ast);
  std::string target(
      "\t.globl\tmain\n"
      "main:\n"
      "\tmov\t$5, %rax\n"
      "\tneg\t%rax\n"
      "\tret\n");
#ifdef _WIN32
  target.insert(0, "\t.def\tmain;\n");
#endif // _WIN32
  REQUIRE(ss.str() == target);
}

TEST_CASE("bitwise complement codegens", "[codegen]") {
  std::string source{R"#(
int main() {
    return ~12;
}
)#"};
  auto ast = bcc::parser::parse(source);
  std::stringstream ss;
  bcc::codegen::codegen(ss, ast);
  std::string target(
      "\t.globl\tmain\n"
      "main:\n"
      "\tmov\t$12, %rax\n"
      "\tnot\t%rax\n"
      "\tret\n");
#ifdef _WIN32
  target.insert(0, "\t.def\tmain;\n");
#endif // _WIN32
  REQUIRE(ss.str() == target);
}

TEST_CASE("logical not codegens", "[codegen]") {
  std::string source{R"#(
int main() {
    return !1;
}
)#"};
  auto ast = bcc::parser::parse(source);
  std::stringstream ss;
  bcc::codegen::codegen(ss, ast);
  std::string target("\t.globl\tmain\n"
                     "main:\n"
                     "\tmov\t$1, %rax\n"
                     "\tcmp\t$0, %rax\n"
                     "\tmov\t$0, %rax\n"
                     "\tsete\t%al\n"
                     "\tret\n");
#ifdef _WIN32
  target.insert(0, "\t.def\tmain;\n");
#endif // _WIN32
  REQUIRE(ss.str() == target);
}
