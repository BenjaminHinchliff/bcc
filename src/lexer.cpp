#include "bcc/lexer.hpp"

#include <iostream>
#include <unordered_map>

namespace bcc {
namespace lexer {
using namespace tokens;

const std::unordered_map<std::string, Token> stringToKeyword{
    {"int", TypeKeyword::INT}, {"return", Keyword::RETURN}};

const std::unordered_map<char, Token> charToUnary{
    {'-', Minus{}}, {'~', BitwiseComplement{}}, {'!', LogicalNegation{}}};

Tokens lex(const std::string &source) {

  std::vector<Token> tokens;
  for (auto it = source.cbegin(); it != source.cend(); ++it) {
    char letter = *it;
    // skip whitespace
    if (std::isspace(letter))
      continue;

    // ident/keyword handling
    if (std::isalpha(letter)) {
      std::string name;
      do {
        name += letter;
        letter = *(++it);
      } while (std::isalnum(letter) || letter == '_');
      // check for keyword
      auto keyword_it = stringToKeyword.find(name);
      if (keyword_it != stringToKeyword.cend()) {
        tokens.push_back(keyword_it->second);
      } else {
        tokens.emplace_back(Identifier{name});
      }
    }

    // integer literals
    if (std::isdigit(letter)) {
      int value = 0;
      do {
        value = value * 10 + letter - '0';
        letter = *(++it);
      } while (std::isdigit(letter));
      tokens.emplace_back(literals::Int{value});
    }

    // unary operators
    auto unary_it = charToUnary.find(letter);
    if (unary_it != charToUnary.end()) {
      tokens.emplace_back(unary_it->second);
      continue;
    }

    // single character tokens
    switch (letter) {
    case '{':
      tokens.emplace_back(OpenBrace{});
      break;
    case '}':
      tokens.emplace_back(CloseBrace{});
      break;
    case '(':
      tokens.emplace_back(OpenParen{});
      break;
    case ')':
      tokens.emplace_back(CloseParen{});
      break;
    case ';':
      tokens.emplace_back(Semicolon{});
      break;
    }
  }
  return tokens;
}
} // namespace lexer
} // namespace bcc
