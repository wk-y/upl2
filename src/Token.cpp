module;
#include <cctype>
#include <climits>
#include <cstdio>
#include <istream>

export module Token;

namespace upl2 {

constexpr bool issymbol(int x) {
  switch (x) {
  case '+':
  case '-':
  case '*':
  case '/':
  case '=':
  case ':':
  case '<':
  case '>':
    return true;
  }
  return false;
}

export class Token {
public:
  enum Type {
    TypeError,
    TypeEOF,
    TypeSymbol,
    TypeInfix,
    TypeSemicolon,
    TypeLpar,
    TypeRpar,
    TypeNumber,
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

    if (std::isdigit(c)) {
      t = {TypeNumber, ""};
      do {
        t.literal.push_back(c);
        c = s.get();
      } while (std::isdigit(c));

      if (c != '.') {
        s.unget();
        return s;
      }

      t.literal.push_back('.');
      while (std::isdigit((c = s.get()))) {
        t.literal.push_back(c);
      }
      s.unget();
      return s;
    }

    if (std::isalpha(c)) {
      t = {
          .type = TypeSymbol,
          .literal = "",
      };
      do {
        t.literal += c;
        c = s.get();
      } while (std::isalnum(c));
      s.unget();
      return s;
    }

    if (issymbol(c)) {
      t = {
          .type = TypeInfix,
          .literal = "",
      };
      do {
        t.literal += c;
        c = s.get();
      } while (issymbol(c));
      s.unget();
      return s;
    }

    switch (c) {
    case ';':
      t = {TypeSemicolon, ";"};
      break;
    case '(':
      t = {TypeLpar, "("};
      break;
    case ')':
      t = {TypeRpar, ")"};
      break;
    default:
      t = {TypeError, std::string{(char)c, 0}};
    }
    return s;
  }
};

} // namespace upl2
