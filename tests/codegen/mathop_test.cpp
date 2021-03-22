#include <sstream>

#include <bcc/bcc.hpp>
#include <catch2/catch.hpp>

TEST_CASE("addition codegens", "[codegen]") {
  std::string source{R"(
int main() {
    return 1 + 2;
}
)"};
  auto ast = bcc::parser::parse(source);
  std::stringstream ss;
  bcc::codegen::codegen(ss, ast);
  std::string target("\t.globl\tmain\n"
                     "main:\n"
                     "\tmov\t$1, %rax\n"
                     "\tpush\t%rax\n"
                     "\tmov\t$2, %rax\n"
                     "\tpop\t%rcx\n"
                     "\tadd\t%rcx, %rax\n"
                     "\tret\n");
#ifdef _WIN32
  target.insert(0, "\t.def\tmain;\n");
#endif // _WIN32
  REQUIRE(ss.str() == target);
}

TEST_CASE("subtraction codegens", "[codegen]") {
  std::string source{R"(
int main() {
    return 1 - 2;
}
)"};
  auto ast = bcc::parser::parse(source);
  std::stringstream ss;
  bcc::codegen::codegen(ss, ast);
  std::string target("\t.globl\tmain\n"
                     "main:\n"
                     "\tmov\t$1, %rax\n"
                     "\tpush\t%rax\n"
                     "\tmov\t$2, %rax\n"
                     "\tmov\t%rax, %rcx\n"
                     "\tpop\t%rax\n"
                     "\tsub\t%rcx, %rax\n"
                     "\tret\n");
#ifdef _WIN32
  target.insert(0, "\t.def\tmain;\n");
#endif // _WIN32
  REQUIRE(ss.str() == target);
}

TEST_CASE("multiply codegens", "[codegen]") {
  std::string source{R"(
int main() {
    return 1 * 2;
}
)"};
  auto ast = bcc::parser::parse(source);
  std::stringstream ss;
  bcc::codegen::codegen(ss, ast);
  std::string target("\t.globl\tmain\n"
                     "main:\n"
                     "\tmov\t$1, %rax\n"
                     "\tpush\t%rax\n"
                     "\tmov\t$2, %rax\n"
                     "\tpop\t%rcx\n"
                     "\timul\t%rcx, %rax\n"
                     "\tret\n");
#ifdef _WIN32
  target.insert(0, "\t.def\tmain;\n");
#endif // _WIN32
  REQUIRE(ss.str() == target);
}

TEST_CASE("division codegens", "[codegen]") {
  std::string source{R"(
int main() {
    return 4 / 2;
}
)"};
  auto ast = bcc::parser::parse(source);
  std::stringstream ss;
  bcc::codegen::codegen(ss, ast);
  std::string target("\t.globl\tmain\n"
                     "main:\n"
                     "\tmov\t$4, %rax\n"
                     "\tpush\t%rax\n"
                     "\tmov\t$2, %rax\n"
                     "\tmov\t%rax, %rcx\n"
                     "\tpop\t%rax\n"
                     "\tcqo\n"
                     "\tidiv\t%rcx\n"
                     "\tret\n");
#ifdef _WIN32
  target.insert(0, "\t.def\tmain;\n");
#endif // _WIN32
  REQUIRE(ss.str() == target);
}

TEST_CASE("modulo codegens", "[codegen]") {
  std::string source{R"(
int main() {
    return 5 % 2;
}
)"};
  auto ast = bcc::parser::parse(source);
  std::stringstream ss;
  bcc::codegen::codegen(ss, ast);
  std::string target("\t.globl\tmain\n"
                     "main:\n"
                     "\tmov\t$5, %rax\n"
                     "\tpush\t%rax\n"
                     "\tmov\t$2, %rax\n"
                     "\tmov\t%rax, %rcx\n"
                     "\tpop\t%rax\n"
                     "\tcqo\n"
                     "\tidiv\t%rcx\n"
                     "\tret\n");
#ifdef _WIN32
  target.insert(0, "\t.def\tmain;\n");
#endif // _WIN32
  REQUIRE(ss.str() == target);
}

TEST_CASE("math precedece codegen", "[codegen]") {
  std::string source{R"(
int main() {
    return 2 + 3 * 4 - 2 / 2;
}
)"};
  auto ast = bcc::parser::parse(source);
  std::stringstream ss;
  bcc::codegen::codegen(ss, ast);
  std::string target("\t.globl\tmain\n"
                     "main:\n"
                     "\tmov\t$2, %rax\n"
                     "\tpush\t%rax\n"
                     "\tmov\t$3, %rax\n"
                     "\tpush\t%rax\n"
                     "\tmov\t$4, %rax\n"
                     "\tpop\t%rcx\n"
                     "\timul\t%rcx, %rax\n"
                     "\tpop\t%rcx\n"
                     "\tadd\t%rcx, %rax\n"
                     "\tpush\t%rax\n"
                     "\tmov\t$2, %rax\n"
                     "\tpush\t%rax\n"
                     "\tmov\t$2, %rax\n"
                     "\tmov\t%rax, %rcx\n"
                     "\tpop\t%rax\n"
                     "\tcqo\n"
                     "\tidiv\t%rcx\n"
                     "\tmov\t%rax, %rcx\n"
                     "\tpop\t%rax\n"
                     "\tsub\t%rcx, %rax\n"
                     "\tret\n");
#ifdef _WIN32
  target.insert(0, "\t.def\tmain;\n");
#endif // _WIN32
  REQUIRE(ss.str() == target);
}