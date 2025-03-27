module;
#include <cctype>
#include <cstdio>
#include <istream>

export module Token;

namespace upl2 {

export class Token {
public:
  enum Type {
    TypeError,
    TypeEOF,
    TypeSymbol,
    TypeArrow,
    TypeEqual,
    TypeColon,
    TypeSemicolon,
    TypeLpar,
    TypeRpar,
  } type;
  std::string literal;

  friend std::istream &operator>>(std::istream &s, Token &t) {
    int c = s.get();
    while (std::isspace(c)) {
      c = s.get();
    }

    if (c == EOF) {
      t = {TypeEOF, "EOF"};
      return s;
    }

    if (std::isalpha(c)) {
      t = {
          .type = TypeSymbol,
          .literal = "",
      };
      while (std::isalnum(c)) {
        t.literal += c;
        c = s.get();
      }
      s.unget();
      return s;
    }

    switch (c) {
    case ';':
      t = {TypeSemicolon, ";"};
      break;
    case ':':
      t = {TypeColon, ":"};
      break;
    case '(':
      t = {TypeLpar, "("};
      break;
    case ')':
      t = {TypeColon, ")"};
      break;
    case '=':
      c = s.peek();
      if (c == '>') {
        s.get();
        t = {TypeArrow, "=>"};
      } else {
        t = {TypeEqual, "="};
      }
      break;
    default:
      t = {TypeError, std::string{(char)c, 0}};
    }
    return s;
  }
};

} // namespace upl2
