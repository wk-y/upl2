#include <iostream>
#include <sstream>
#include <variant>
import Parser;
import Interpreter;

int main() {
  std::istringstream s("1");
  upl2::parser::Parser p(&s);
  upl2::interpreter::Interpreter i;
  auto expr = p.parse_statement();
  auto result = i.run(expr);
  auto rptr = std::get_if<double>(&result);
  if (!rptr) {
    std::cerr << "expected double\n";
    return 1;
  }
  if (*rptr != 1) {
    std::cerr << "expected 1, got " << *rptr << ".\n";
    return 1;
  }
}
