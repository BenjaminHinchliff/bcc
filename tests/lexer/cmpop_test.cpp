#include <catch2/catch.hpp>

#include <bcc/bcc.hpp>

TEST_CASE("equal lexes", "[lexer]") {
  std::string source{R"(
int main() {
    return 1 == 0;
}
)"};
  auto tokens = bcc::lexer::lex(source);
  using namespace bcc::tokens;
  std::vector<Token> target{
      TypeKeyword::Int, Identifier{"main"}, OpenParen{},  CloseParen{},
      OpenBrace{},      Keyword::Return,    Int{1},       Operators::Equal,
      Int{0},           Semicolon{},        CloseBrace{},
  };
  REQUIRE(tokens == target);
}

TEST_CASE("not equal lexes", "[lexer]") {
  std::string source{R"(
int main() {
    return 1 != 0;
}
)"};
  auto tokens = bcc::lexer::lex(source);
  using namespace bcc::tokens;
  std::vector<Token> target{
      TypeKeyword::Int, Identifier{"main"}, OpenParen{},  CloseParen{},
      OpenBrace{},      Keyword::Return,    Int{1},       Operators::NotEqual,
      Int{0},           Semicolon{},        CloseBrace{},
  };
  REQUIRE(tokens == target);
}

TEST_CASE("less than lexes", "[lexer]") {
  std::string source{R"(
int main() {
    return 1 < 0;
}
)"};
  auto tokens = bcc::lexer::lex(source);
  using namespace bcc::tokens;
  std::vector<Token> target{
      TypeKeyword::Int, Identifier{"main"}, OpenParen{},  CloseParen{},
      OpenBrace{},      Keyword::Return,    Int{1},       Operators::LessThan,
      Int{0},           Semicolon{},        CloseBrace{},
  };
  REQUIRE(tokens == target);
}

TEST_CASE("less than or equal lexes", "[lexer]") {
  std::string source{R"(
int main() {
    return 1 <= 0;
}
)"};
  auto tokens = bcc::lexer::lex(source);
  using namespace bcc::tokens;
  std::vector<Token> target{
      TypeKeyword::Int, Identifier{"main"},
      OpenParen{},      CloseParen{},
      OpenBrace{},      Keyword::Return,
      Int{1},           Operators::LessThanOrEqual,
      Int{0},           Semicolon{},
      CloseBrace{},
  };
  REQUIRE(tokens == target);
}

TEST_CASE("greater than lexes", "[lexer]") {
  std::string source{R"(
int main() {
    return 1 > 0;
}
)"};
  auto tokens = bcc::lexer::lex(source);
  using namespace bcc::tokens;
  std::vector<Token> target{
      TypeKeyword::Int, Identifier{"main"},
      OpenParen{},      CloseParen{},
      OpenBrace{},      Keyword::Return,
      Int{1},           Operators::GreaterThan,
      Int{0},           Semicolon{},
      CloseBrace{},
  };
  REQUIRE(tokens == target);
}

TEST_CASE("greater than or equal lexes", "[lexer]") {
  std::string source{R"(
int main() {
    return 1 >= 0;
}
)"};
  auto tokens = bcc::lexer::lex(source);
  using namespace bcc::tokens;
  std::vector<Token> target{
      TypeKeyword::Int, Identifier{"main"},
      OpenParen{},      CloseParen{},
      OpenBrace{},      Keyword::Return,
      Int{1},           Operators::GreaterThanOrEqual,
      Int{0},           Semicolon{},
      CloseBrace{},
  };
  REQUIRE(tokens == target);
}
