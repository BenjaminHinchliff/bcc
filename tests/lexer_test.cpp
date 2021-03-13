#include <catch2/catch.hpp>
#include <tcc/tcc.hpp>

#include <iostream>

TEST_CASE("return 2 lexes", "[lexer]") {
  std::string source{R"#(
int main() {
    return 2;
}
)#"};
  auto tokens = tcc::lexer::lex(source);
  using namespace tcc::tokens;
  std::vector<Token> target{
      TypeKeyword::INT,     Identifier{"main"}, OpenParen{},
      CloseParen{},     OpenBrace{},        Keyword::RETURN,
      literals::Int{2}, Semicolon{},        CloseBrace{},
  };
  REQUIRE(tokens == target);
}
