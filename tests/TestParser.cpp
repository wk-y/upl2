#include <iostream>
#include <sstream>

import Parser;
import Ast;
import Token;

int main() {
  auto tests = {
      "(print)", "(print msg)", "(add = a => b => plus a b)", "(1)", "(\"hi\")",
  };

  for (auto &test : tests) {
    using namespace upl2::ast;
    using namespace upl2::parser;
    std::istringstream s(test);
    std::string program = "print;";

    Parser p(&s);
    p.parse_statement_list();

    if (p.peek().type != upl2::Token::TypeEOF) {
      std::cerr << "did not fully parse the input\n";
      std::cerr << test;
      return 1;
    }
  }
}
