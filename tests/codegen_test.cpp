#include <sstream>

#include <bcc/bcc.hpp>
#include <catch2/catch.hpp>

TEST_CASE("return 2 codegens", "[codegen]") {
  std::string source{R"#(
int main() {
    return 2;
}
)#"};
  auto ast = bcc::parser::parse(source);
  std::stringstream ss;
  bcc::codegen::codegen(ss, ast);
  std::string target("\t.globl\tmain\nmain:\n\tmov\t$2, %rax\n\tret\n");
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
  auto ast = bcc::parser::parse(source);
  std::stringstream ss;
  bcc::codegen::codegen(ss, ast);
  std::string target(
      "\t.globl\tmain\nmain:\n\tmov\t$5, %rax\n\tneg\t%rax\n\tret\n");
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
      "\t.globl\tmain\nmain:\n\tmov\t$12, %rax\n\tnot\t%rax\n\tret\n");
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

TEST_CASE("math operators codegen", "[codegen]") {
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
