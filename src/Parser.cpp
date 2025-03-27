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

  ast::Node parse_expression() {
    switch (peek().type) {
    case Token::TypeLpar: {
      ast::Node stmt = parse_statement();
      if (peek().type != Token::TypeRpar)
        throw std::runtime_error("missing right parenthesis");
      next();
      return stmt;
    }
    case Token::TypeSymbol:
      return parse_symbol();
    default:
      throw ParseError();
    }
  }

  ast::Node parse_statement() {
    auto expr = parse_expression();

    switch (peek().type) {
    case Token::TypeArrow: {
      next();
      return ast::Function{
          std::make_unique<ast::Node>(std::move(expr)),
          std::make_unique<ast::Node>(parse_statement()),
      };
    }
    case Token::TypeEqual: {
      next();
      return ast::Assignment{
          std::make_unique<ast::Node>(std::move(expr)),
          std::make_unique<ast::Node>(parse_statement()),
      };
    }
    case Token::TypeLpar:
    case Token::TypeSymbol:
      return ast::Call{
          std::make_unique<ast::Node>(std::move(expr)),
          std::make_unique<ast::Node>(parse_statement()),
      };
    default:
      return expr;
    }
  }

  std::vector<ast::Node> parse_statement_list() {
    std::vector<ast::Node> result;
    while (peek().type == Token::TypeSymbol || peek().type == Token::TypeLpar) {
      result.push_back(parse_statement());
      if (peek().type != Token::TypeSemicolon)
        throw std::runtime_error("missing semicolon");
      next();
    }
    return result;
  }
};

} // namespace upl2::parser
