#include <initializer_list>
#include <iostream>
#include <sstream>
#include <variant>
import Parser;
import Interpreter;
import Builtins;

int main() {
  struct skip {};
  struct Test {
    std::string statements;
    std::initializer_list<std::variant<double, std::string, skip>> expected;
  } tests[] = {{"(x = 1 + 2) (x + 4)", {3., 7.}},
               {"(x = 3) (y = 5) (x + y)", {3., 5., 8.}},
               {"((x) = 1) (x)", {1., 1.}},
               {"((*) = (+)) (2 * 3)", {skip{}, 5.}},
               {"((let (x = 5) (x + 1) end) + 3)", {9.}}};

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

      if (std::get_if<skip>(expected)) {
        // skip
      } else if (auto exp = std::get_if<double>(expected)) {
        auto actual = result->number();
        if (actual != *exp) {
          std::cerr << "expected " << *exp << ", got " << actual << ".\n";
          return 1;
        }
      } else if (auto exp = std::get_if<std::string>(expected)) {
        auto actual = result->string();
        if (actual != *exp) {
          std::cerr << "expected " << *exp << ", got " << actual << ".\n";
          return 1;
        }
      }
      expected++;
    }
  }
}
