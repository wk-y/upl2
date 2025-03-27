#include <iostream>
#include <sstream>
#include <variant>
import Parser;
import Interpreter;
import Builtins;

int main() {
  std::istringstream s("add 1 2");
  upl2::parser::Parser p(&s);
  upl2::interpreter::Interpreter i;
  upl2::interpreter::builtins::load_all(i);
  auto expr = p.parse_statement();
  auto result = i.run(expr);
  auto rptr = std::get_if<double>(&result);
  if (!rptr) {
    std::cerr << "expected double\n";
    return 1;
  }
  if (*rptr != 3) {
    std::cerr << "expected 3, got " << *rptr << ".\n";
    return 1;
  }
}
