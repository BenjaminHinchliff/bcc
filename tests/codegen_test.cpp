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

TEST_CASE("negate codegens", "[codegen]") {
  std::string source{R"#(
int main() {
    return -5;
}
)#"};
  auto ast = tcc::parser::parse(source);
  std::stringstream ss;
  tcc::codegen::codegen(ss, ast);
  std::string target(
      "\t.globl\tmain\nmain:\n\tmovl\t$5, %eax\n\tneg\t%eax\n\tretq\n");
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
  auto ast = tcc::parser::parse(source);
  std::stringstream ss;
  tcc::codegen::codegen(ss, ast);
  std::string target(
      "\t.globl\tmain\nmain:\n\tmovl\t$12, %eax\n\tnot\t%eax\n\tretq\n");
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
  auto ast = tcc::parser::parse(source);
  std::stringstream ss;
  tcc::codegen::codegen(ss, ast);
  std::string target(
      "\t.globl\tmain\nmain:\n\tmovl\t$1, %eax\n\tcmpl\t$0, %eax\n\tmovl\t$0, %eax\n\tsete\t%al\n\tretq\n");
#ifdef _WIN32
  target.insert(0, "\t.def\tmain;\n");
#endif // _WIN32
  REQUIRE(ss.str() == target);
}

