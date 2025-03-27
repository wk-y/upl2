#include <iostream>
import Interpreter;
import Parser;
import Token;
import Builtins;

int main() {
  using namespace upl2::parser;
  using namespace upl2::interpreter;
  Interpreter interpreter;
  upl2::interpreter::builtins::load_all(interpreter);

  for (;;) {
    Parser p(&std::cin);

    std::cerr << "$ ";
    auto expr = p.parse_statement();

    auto result = interpreter.run(expr);

    std::cout << result << "\n";
  }
}
