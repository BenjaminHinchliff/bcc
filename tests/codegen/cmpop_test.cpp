#include <sstream>

#include <bcc/bcc.hpp>
#include <catch2/catch.hpp>

TEST_CASE("equality codegens", "[codegen]") {
  std::string source{R"#(
int main() {
    return 0 == 1;
}
)#"};
  auto ast = bcc::parser::parse(source);
  std::stringstream ss;
  bcc::codegen::codegen(ss, ast);
  std::string target("\t.globl\tmain\n"
                     "main:\n"
                     "\tmov\t$0, %rax\n"
                     "\tpush\t%rax\n"
                     "\tmov\t$1, %rax\n"
                     "\tpop\t%rcx\n"
                     "\tcmp\t%rax, %rcx\n"
                     "\tmov\t$0, %rax\n"
                     "\tsete\t%al\n"
                     "\tret\n");
#ifdef _WIN32
  target.insert(0, "\t.def\tmain;\n");
#endif // _WIN32
  REQUIRE(ss.str() == target);
}

TEST_CASE("inequality codegens", "[codegen]") {
  std::string source{R"#(
int main() {
    return 0 != 1;
}
)#"};
  auto ast = bcc::parser::parse(source);
  std::stringstream ss;
  bcc::codegen::codegen(ss, ast);
  std::string target("\t.globl\tmain\n"
                     "main:\n"
                     "\tmov\t$0, %rax\n"
                     "\tpush\t%rax\n"
                     "\tmov\t$1, %rax\n"
                     "\tpop\t%rcx\n"
                     "\tcmp\t%rax, %rcx\n"
                     "\tmov\t$0, %rax\n"
                     "\tsetne\t%al\n"
                     "\tret\n");
#ifdef _WIN32
  target.insert(0, "\t.def\tmain;\n");
#endif // _WIN32
  REQUIRE(ss.str() == target);
}

TEST_CASE("greater than codegens", "[codegen]") {
  std::string source{R"#(
int main() {
    return 0 > 1;
}
)#"};
  auto ast = bcc::parser::parse(source);
  std::stringstream ss;
  bcc::codegen::codegen(ss, ast);
  std::string target("\t.globl\tmain\n"
                     "main:\n"
                     "\tmov\t$0, %rax\n"
                     "\tpush\t%rax\n"
                     "\tmov\t$1, %rax\n"
                     "\tpop\t%rcx\n"
                     "\tcmp\t%rax, %rcx\n"
                     "\tmov\t$0, %rax\n"
                     "\tsetg\t%al\n"
                     "\tret\n");
#ifdef _WIN32
  target.insert(0, "\t.def\tmain;\n");
#endif // _WIN32
  REQUIRE(ss.str() == target);
}

TEST_CASE("less than codegens", "[codegen]") {
  std::string source{R"#(
int main() {
    return 0 < 1;
}
)#"};
  auto ast = bcc::parser::parse(source);
  std::stringstream ss;
  bcc::codegen::codegen(ss, ast);
  std::string target("\t.globl\tmain\n"
                     "main:\n"
                     "\tmov\t$0, %rax\n"
                     "\tpush\t%rax\n"
                     "\tmov\t$1, %rax\n"
                     "\tpop\t%rcx\n"
                     "\tcmp\t%rax, %rcx\n"
                     "\tmov\t$0, %rax\n"
                     "\tsetl\t%al\n"
                     "\tret\n");
#ifdef _WIN32
  target.insert(0, "\t.def\tmain;\n");
#endif // _WIN32
  REQUIRE(ss.str() == target);
}

TEST_CASE("greater than or equal codegens", "[codegen]") {
  std::string source{R"#(
int main() {
    return 0 >= 1;
}
)#"};
  auto ast = bcc::parser::parse(source);
  std::stringstream ss;
  bcc::codegen::codegen(ss, ast);
  std::string target("\t.globl\tmain\n"
                     "main:\n"
                     "\tmov\t$0, %rax\n"
                     "\tpush\t%rax\n"
                     "\tmov\t$1, %rax\n"
                     "\tpop\t%rcx\n"
                     "\tcmp\t%rax, %rcx\n"
                     "\tmov\t$0, %rax\n"
                     "\tsetge\t%al\n"
                     "\tret\n");
#ifdef _WIN32
  target.insert(0, "\t.def\tmain;\n");
#endif // _WIN32
  REQUIRE(ss.str() == target);
}

TEST_CASE("less than or equal codegens", "[codegen]") {
  std::string source{R"#(
int main() {
    return 0 <= 1;
}
)#"};
  auto ast = bcc::parser::parse(source);
  std::stringstream ss;
  bcc::codegen::codegen(ss, ast);
  std::string target("\t.globl\tmain\n"
                     "main:\n"
                     "\tmov\t$0, %rax\n"
                     "\tpush\t%rax\n"
                     "\tmov\t$1, %rax\n"
                     "\tpop\t%rcx\n"
                     "\tcmp\t%rax, %rcx\n"
                     "\tmov\t$0, %rax\n"
                     "\tsetle\t%al\n"
                     "\tret\n");
#ifdef _WIN32
  target.insert(0, "\t.def\tmain;\n");
#endif // _WIN32
  REQUIRE(ss.str() == target);
}
