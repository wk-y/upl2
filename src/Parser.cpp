module;
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

export module Parser;
import Token;
import Ast;

namespace upl2::parser {

export class Parser {
  upl2::Token peeked;
  bool peeking = false;

public:
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

  bool done() { return peek().type == Token::TypeEOF; }

  class ParseError : std::exception {
  public:
    const char *what() const noexcept { return "parse error"; };
  };
  std::istream *s;
  Parser(std::istream *s_) : s(s_) {};

  ast::Symbol parse_symbol() {
    if (peek().type == upl2::Token::TypeSymbol)
      return {next().literal};
    throw ParseError();
  }

  ast::Number parse_number() {
    if (peek().type == upl2::Token::TypeNumber)
      return next().literal;
    throw ParseError();
  }

  ast::Node parse_expression() {
    switch (peek().type) {
    case Token::TypeLpar: {
      next();
      ast::Node stmt = peek().type == Token::TypeInfix
                           ? ast::Symbol{next().literal}
                           : parse_statement();
      if (peek().type != Token::TypeRpar)
        throw std::runtime_error("missing right parenthesis");
      next();
      return stmt;
    }
    case Token::TypeSymbol:
      return parse_symbol();
    case Token::TypeNumber:
      return parse_number();
    default:
      throw ParseError();
    }
  }

  ast::Node parse_statement() {
    auto expr = std::make_unique<ast::Node>(parse_expression());

  escapehatch:
    switch (peek().type) {
    case Token::TypeInfix: {
      ast::Node infix = ast::Symbol{next().literal};
      return ast::Call(
          std::make_unique<ast::Node>(
              ast::Call(std::make_unique<ast::Node>(infix), std::move(expr))),
          std::make_unique<ast::Node>(parse_statement()));
    }
    case Token::TypeLpar:
    case Token::TypeSymbol:
    case Token::TypeNumber: {
      expr = std::make_unique<ast::Node>(ast::Call{
          std::make_unique<ast::Node>(std::move(*expr.get())),
          std::make_unique<ast::Node>(parse_expression()),
      });
      goto escapehatch;
    }
    default:
      return *expr.get();
    }
  }

  std::vector<ast::Node> parse_statement_list() {
    std::vector<ast::Node> result;
    while (peek().type == Token::TypeSymbol || peek().type == Token::TypeLpar ||
           peek().type == Token::TypeNumber) {
      result.push_back(parse_statement());
      if (peek().type != Token::TypeSemicolon)
        throw std::runtime_error("missing semicolon");
      next();
    }
    return result;
  }
};

} // namespace upl2::parser
