#include <catch2/catch.hpp>
#include <tcc/tcc.hpp>

#include <iostream>

TEST_CASE("lexer tests", "[lexer]") {
  SECTION("return 2") {
    std::string source{R"#(
int main() {
    return 2;
}
)#"};
    auto tokens = tcc::lexer::lex(source);
    using namespace tcc::tokens;
    std::vector<Token> target{
        Keyword::INT,     Identifier{"main"}, OpenParen{},
        CloseParen{},     OpenBrace{},        Keyword::RETURN,
        literals::Int{2}, Semicolon{},        CloseBrace{},
    };
    //std::cout << tokens << '\n';
    REQUIRE(tokens == target);
  }
}
