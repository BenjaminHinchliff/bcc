#include <catch2/catch.hpp>

#include <bcc/bcc.hpp>

TEST_CASE("add lexes", "[lexer]") {
  std::string source{R"(
int main() {
    return 2 + 3;
}
)"};
  auto tokens = bcc::lexer::lex(source);
  using namespace bcc::tokens;
  std::vector<Token> target{
      TypeKeyword::Int, Identifier{"main"}, OpenParen{},  CloseParen{},
      OpenBrace{},      Keyword::Return,    Int{2},       Operators::Add,
      Int{3},           Semicolon{},        CloseBrace{},
  };
  REQUIRE(tokens == target);
}

TEST_CASE("subtract lexes", "[lexer]") {
  std::string source{R"(
int main() {
    return 2 - 3;
}
)"};
  auto tokens = bcc::lexer::lex(source);
  using namespace bcc::tokens;
  std::vector<Token> target{
      TypeKeyword::Int, Identifier{"main"}, OpenParen{},  CloseParen{},
      OpenBrace{},      Keyword::Return,    Int{2},       Operators::Minus,
      Int{3},           Semicolon{},        CloseBrace{},
  };
  REQUIRE(tokens == target);
}

TEST_CASE("multiply lexes", "[lexer]") {
  std::string source{R"(
int main() {
    return 2 * 3;
}
)"};
  auto tokens = bcc::lexer::lex(source);
  using namespace bcc::tokens;
  std::vector<Token> target{
      TypeKeyword::Int, Identifier{"main"}, OpenParen{},  CloseParen{},
      OpenBrace{},      Keyword::Return,    Int{2},       Operators::Multiply,
      Int{3},           Semicolon{},        CloseBrace{},
  };
  REQUIRE(tokens == target);
}

TEST_CASE("divide lexes", "[lexer]") {
  std::string source{R"(
int main() {
    return 2 / 3;
}
)"};
  auto tokens = bcc::lexer::lex(source);
  using namespace bcc::tokens;
  std::vector<Token> target{
      TypeKeyword::Int, Identifier{"main"}, OpenParen{},  CloseParen{},
      OpenBrace{},      Keyword::Return,    Int{2},       Operators::Divide,
      Int{3},           Semicolon{},        CloseBrace{},
  };
  REQUIRE(tokens == target);
}

TEST_CASE("modulo lexes", "[lexer]") {
  std::string source{R"(
int main() {
    return 2 % 3;
}
)"};
  auto tokens = bcc::lexer::lex(source);
  using namespace bcc::tokens;
  std::vector<Token> target{
      TypeKeyword::Int, Identifier{"main"}, OpenParen{},  CloseParen{},
      OpenBrace{},      Keyword::Return,    Int{2},       Operators::Modulo,
      Int{3},           Semicolon{},        CloseBrace{},
  };
  REQUIRE(tokens == target);
}
