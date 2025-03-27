#include <cctype>
#include <iostream>
#include <vector>
import Interpreter;
import Parser;
import Token;
import Builtins;
import Ast;

int main() {
  using namespace upl2::parser;
  using namespace upl2::interpreter;
  Interpreter interpreter;
  upl2::interpreter::builtins::load_all(interpreter);

  for (;;) {
    Parser p(&std::cin);

    std::cerr << "$ ";

    // Read statements, stopping at a new line
    std::vector<upl2::ast::Node> statements;
    statements.push_back(p.parse_statement());
    for (;;) {
      auto c = std::cin.get();
      if (c == EOF)
        break;
      if (c == '\n')
        break;
      if (std::isspace(c))
        continue;
      std::cin.unget();
      statements.push_back(p.parse_statement());
    }

    // Evaluate
    for (auto &statement : statements) {
      auto result = interpreter.run(statement);
      std::cout << result << "\n";
    }
  }
}
