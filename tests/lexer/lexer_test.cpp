#include <bcc/bcc.hpp>
#include <catch2/catch.hpp>

TEST_CASE("return 2 lexes", "[lexer]") {
  std::string source{R"#(
int main() {
    return 2;
}
)#"};
  auto tokens = bcc::lexer::lex(source);
  using namespace bcc::tokens;
  std::vector<Token> target{
      TypeKeyword::Int, Identifier{"main"}, OpenParen{},
      CloseParen{},     OpenBrace{},        Keyword::Return,
      Int{2},           Semicolon{},        CloseBrace{},
  };
  REQUIRE(tokens == target);
}
