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
    return 1;
  }
}
