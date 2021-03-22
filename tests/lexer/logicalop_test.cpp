#include <catch2/catch.hpp>

#include <bcc/bcc.hpp>

TEST_CASE("and lexes", "[lexer]") {
  std::string source{R"(
int main() {
    return 1 && 0;
}
)"};
  auto tokens = bcc::lexer::lex(source);
  using namespace bcc::tokens;
  std::vector<Token> target{
      TypeKeyword::Int, Identifier{"main"}, OpenParen{},  CloseParen{},
      OpenBrace{},      Keyword::Return,    Int{1},       Operators::LogicalAnd,
      Int{0},           Semicolon{},        CloseBrace{},
  };
  REQUIRE(tokens == target);
}

TEST_CASE("or lexes", "[lexer]") {
  std::string source{R"(
int main() {
    return 1 || 0;
}
)"};
  auto tokens = bcc::lexer::lex(source);
  using namespace bcc::tokens;
  std::vector<Token> target{
      TypeKeyword::Int, Identifier{"main"}, OpenParen{},  CloseParen{},
      OpenBrace{},      Keyword::Return,    Int{1},       Operators::LogicalOr,
      Int{0},           Semicolon{},        CloseBrace{},
  };
  REQUIRE(tokens == target);
}
