#include <cassert>
#include <format>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

import Token;

int main() {
  using namespace upl2;
  struct TestCase {
    int test_no;
    std::string input;
    std::vector<Token> expected;
  };
  std::vector<TestCase> tests = {
      TestCase{
          1,
          "puts: x;",
          {
              {Token::TypeSymbol, "puts"},
              {Token::TypeColon, ":"},
              {Token::TypeSymbol, "x"},
              {Token::TypeSemicolon, ";"},
              {Token::TypeEOF, "EOF"},
          },
      },
  };
  for (auto &test : tests) {
    std::istringstream s(test.input);
    for (auto expected : test.expected) {
      Token t;
      s >> t;
      if (t.literal != expected.literal || t.type != expected.type) {
        std::cerr << "Expected " << expected.literal << "(" << expected.type
                  << "), got " << t.literal << "(" << t.type << ")\n";
        return 1;
      }
    }
  }
}
