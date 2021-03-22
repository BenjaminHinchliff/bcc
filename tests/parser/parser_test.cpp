#include <bcc/bcc.hpp>
#include <catch2/catch.hpp>

using namespace bcc::ast;

TEST_CASE("return 2 parses", "[parser]") {
  std::string source{R"#(
int main() {
    return 2;
}
)#"};
  auto ast = bcc::parser::parse(source);
  Program target("main", std::make_unique<Stmt>(
                             Return(std::make_unique<Expr>(Constant(2)))));
  REQUIRE(ast == target);
}
