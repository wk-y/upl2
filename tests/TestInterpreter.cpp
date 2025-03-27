#include <initializer_list>
#include <iostream>
#include <sstream>
#include <variant>
import Parser;
import Interpreter;
import Builtins;

int main() {
  struct Test {
    std::string statements;
    std::initializer_list<double> expected;
  } tests[] = {
      {"x = 1 + 2; x + 4;", {3, 7}},
      {"x = 3; y = 5; x + y;", {3, 5, 8}},
  };

  for (auto &test : tests) {
    std::istringstream s(test.statements);
    upl2::parser::Parser p(&s);
    auto statements = p.parse_statement_list();
    if (!p.done()) {
      std::cerr << "did not finish parsing\n";
      return 1;
    }

    if (statements.size() != test.expected.size()) {
      std::cerr << "Expected " << test.expected.size() << " statements, got "
                << statements.size() << ":\n";
      std::cerr << test.statements;

      return 1;
    }

    upl2::interpreter::Interpreter i;
    upl2::interpreter::builtins::load_all(i);

    auto expected = test.expected.begin();

    for (auto &expr : statements) {
      auto result = i.run(expr);
      auto rptr = std::get_if<double>(&result);
      if (!rptr) {
        std::cerr << "expected double\n";
        return 1;
      }
      if (*rptr != *expected) {
        std::cerr << "expected " << *expected << ", got " << *rptr << ".\n";
        return 1;
      }
      expected++;
    }
  }
}
