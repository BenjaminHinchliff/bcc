#include <catch2/catch.hpp>

#include <bcc/bcc.hpp>

TEST_CASE("negate lexes", "[lexer]") {
  std::string source{R"#(
int main() {
    return -5;
}
)#"};
  auto tokens = bcc::lexer::lex(source);
  using namespace bcc::tokens;
  std::vector<Token> target{
      TypeKeyword::Int, Identifier{"main"}, OpenParen{},      CloseParen{},
      OpenBrace{},      Keyword::Return,    Operators::Minus, Int{5},
      Semicolon{},      CloseBrace{},
  };
  REQUIRE(tokens == target);
}

TEST_CASE("bitwise complement lexes", "[lexer]") {
  std::string source{R"#(
int main() {
    return ~12;
}
)#"};
  auto tokens = bcc::lexer::lex(source);
  using namespace bcc::tokens;
  std::vector<Token> target{
      TypeKeyword::Int, Identifier{"main"}, OpenParen{},           CloseParen{},
      OpenBrace{},      Keyword::Return,    Operators::BitwiseNot, Int{12},
      Semicolon{},      CloseBrace{},
  };
  REQUIRE(tokens == target);
}

TEST_CASE("logical not lexes", "[lexer]") {
  std::string source{R"#(
int main() {
    return !0;
}
)#"};
  auto tokens = bcc::lexer::lex(source);
  using namespace bcc::tokens;
  std::vector<Token> target{
      TypeKeyword::Int, Identifier{"main"}, OpenParen{},           CloseParen{},
      OpenBrace{},      Keyword::Return,    Operators::LogicalNot, Int{0},
      Semicolon{},      CloseBrace{},
  };
  REQUIRE(tokens == target);
}
