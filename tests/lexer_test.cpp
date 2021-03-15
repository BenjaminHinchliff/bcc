#include <bcc/bcc.hpp>
#include <catch2/catch.hpp>

#include <iostream>

TEST_CASE("return 2 lexes", "[lexer]") {
  std::string source{R"#(
int main() {
    return 2;
}
)#"};
  auto tokens = bcc::lexer::lex(source);
  using namespace bcc::tokens;
  std::vector<Token> target{
      TypeKeyword::INT, Identifier{"main"}, OpenParen{},
      CloseParen{},     OpenBrace{},        Keyword::RETURN,
      literals::Int{2}, Semicolon{},        CloseBrace{},
  };
  REQUIRE(tokens == target);
}

TEST_CASE("negate lexes", "[lexer]") {
  std::string source{R"#(
int main() {
    return -5;
}
)#"};
  auto tokens = bcc::lexer::lex(source);
  using namespace bcc::tokens;
  std::vector<Token> target{
      TypeKeyword::INT, Identifier{"main"}, OpenParen{}, CloseParen{},
      OpenBrace{},      Keyword::RETURN,    Minus{},     literals::Int{5},
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
      TypeKeyword::INT,    Identifier{"main"}, OpenParen{},
      CloseParen{},        OpenBrace{},        Keyword::RETURN,
      BitwiseComplement{}, literals::Int{12},  Semicolon{},
      CloseBrace{},
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
      TypeKeyword::INT, Identifier{"main"}, OpenParen{},       CloseParen{},
      OpenBrace{},      Keyword::RETURN,    LogicalNegation{}, literals::Int{0},
      Semicolon{},      CloseBrace{},
  };
  REQUIRE(tokens == target);
}
