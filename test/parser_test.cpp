#include <catch2/catch.hpp>
#include <iostream>
#include <tcc/tcc.hpp>

using namespace tcc::ast;

TEST_CASE("return 2 parses", "[parser]") {
  SECTION("valid program") {
    std::string source{R"#(
int main() {
    return 2;
}
)#"};
    auto ast = tcc::parser::parse(source);
    Program target{"main", Stmt{Return{Constant{2}}}};
    REQUIRE(ast == target);
  }

  SECTION("invalid programs") {
    std::vector<std::string> sources = {
        R"(
int main( {
    return 0;
}
)",
        R"(
int main() {
    return;
}
)",
        R"(
int main() {
    return 0;
)",
        R"(
int main() {
    return 0
}
)",
        R"(
int main() {
    return0;
}
)", R"(
int main() {
    RETURN 0;
}
)"};
    for (const std::string &source : sources) {
      REQUIRE_THROWS_AS(tcc::parser::parse(source), tcc::parser::Exception);
    }
  }
}
