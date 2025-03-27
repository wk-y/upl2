module;
#include <cstring>
#include <exception>
#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>

export module Parser;
import Token;
import Ast;

namespace upl2::parser {

export class Parser {
  upl2::Token peeked;
  bool peeking = false;

  upl2::Token &peek() {
    if (peeking)
      return peeked;
    *s >> peeked;
    peeking = true;
    return peeked;
  }

  upl2::Token &next() {
    peek();
    peeking = false;
    return peeked;
  }

public:
  class ParseError : std::exception {
  public:
    const char *what() const noexcept { return "parse error"; };
  };
  std::istream *s;
  Parser(std::istream *s_) : s(s_) {};

  upl2::ast::Symbol parse_symbol() {
    if (peek().type == upl2::Token::TypeSymbol)
      return {next().literal};
    throw ParseError();
  }

  ast::Node parse_statement() {
    switch (peek().type) {
    case Token::TypeSymbol: {
      auto sym = parse_symbol();

      switch (peek().type) {
      case Token::TypeArrow:
        next();
        return ast::Function{
            std::make_unique<ast::Node>(sym),
            std::make_unique<ast::Node>(parse_statement()),
        };
      case Token::TypeEqual:
        return ast::Assignment{
            std::make_unique<ast::Node>(sym),
            std::make_unique<ast::Node>(parse_statement()),
        };
      default:
        return sym;
      }
    }
    default:
      throw std::runtime_error("failed to parse symbol");
    }
  }
};
} // namespace upl2::parser
