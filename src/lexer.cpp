#include "bcc/lexer.hpp"

#include <algorithm>
#include <iostream>
#include <optional>
#include <unordered_map>

namespace bcc {
namespace lexer {
using namespace tokens;

const std::unordered_map<std::string, Token> stringToKeyword{
    {"int", TypeKeyword::Int}, {"return", Keyword::Return}};

const std::unordered_map<char, Token> oneLetterToToken{{'{', OpenBrace{}},
                                                       {'}', CloseBrace{}},
                                                       {'(', OpenParen{}},
                                                       {')', CloseParen{}},
                                                       {';', Semicolon{}}};

template <typename Iter> std::optional<Operators> lettersToOp(Iter &letters) {
  switch (*letters) {
  case '-':
    return Operators::Minus;
  case '~':
    return Operators::BitwiseNot;
  case '!':
    if (*std::next(letters) == '=') {
      ++letters;
      return Operators::NotEqual;
    } else {
      return Operators::LogicalNot;
    }
  case '+':
    return Operators::Add;
  case '*':
    return Operators::Multiply;
  case '/':
    return Operators::Divide;
  case '&':
    if (*std::next(letters) == '&') {
      ++letters;
      return Operators::LogicalAnd;
    } else {
      return {};
    }
  case '|':
    if (*std::next(letters) == '|') {
      ++letters;
      return Operators::LogicalOr;
    } else {
      return {};
    }
  case '=':
    if (*std::next(letters) == '=') {
      ++letters;
      return Operators::Equal;
    } else {
      return {};
    }
  case '<':
    if (*std::next(letters) == '=') {
      ++letters;
      return Operators::LessThanOrEqual;
    } else {
      return Operators::LessThan;
    }
  case '>':
    if (*std::next(letters) == '=') {
      ++letters;
      return Operators::GreaterThanOrEqual;
    } else {
      return Operators::GreaterThan;
    }
  default:
    return {};
  }
}

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
      tokens.emplace_back(Int{value});
    }

    // operators
    // consider using trie instead of brute-force
    const auto maybeOp = lettersToOp(it);
    if (maybeOp.has_value()) {
      tokens.emplace_back(*maybeOp);
    }

    // misc tokens
    const auto one_it = oneLetterToToken.find(letter);
    if (one_it != oneLetterToToken.end()) {
      tokens.push_back(one_it->second);
    }
  }
  return tokens;
}
} // namespace lexer
} // namespace bcc
