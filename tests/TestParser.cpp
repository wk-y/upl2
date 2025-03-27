#include <iostream>
#include <sstream>

import Parser;
import Ast;

int main() {
  using namespace upl2::ast;
  using namespace upl2::parser;
  std::string program = "print;";
  std::istringstream s(program);
  Parser p(&s);
  auto sym = p.parse_statement();
}
