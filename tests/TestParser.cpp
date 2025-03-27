#include <iostream>
#include <sstream>

import Parser;
import Ast;

int main() {
  using namespace upl2::ast;
  using namespace upl2::parser;
  std::string program = "print: \"Hello world!\"";
  std::istringstream s(program);
  Parser p(&s);
  auto sym = p.parse_symbol();
  if (sym != Symbol{"print"}) {
    std::cerr << sym.name << " != print\n";
    return 1;
  }
}
