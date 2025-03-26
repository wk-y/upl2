#include "../src/builtins.hpp"
#include "../src/interpreter.hpp"
#include "../src/value.hpp"
#include <initializer_list>
#include <memory>
#include <sstream>
#include <stdexcept>

int main(void) {
  using namespace upl2::builtins;
  struct Test {
    std::string name;
    std::initializer_list<Value *> init;
    double expected;
  } tests[] = {
      {
          "Basic add",
          {new Function(add), new NumberValue(1), new NumberValue(2)},
          3,
      },
      {
          "Two adds",
          {new Function(add), new NumberValue(1), new Function(add),
           new NumberValue(2), new NumberValue(3)},
          6,
      },
  };

  for (auto &test : tests) {
    Interpreter i(test.init);
    auto x = i.eval_and_pop();
    auto v = dynamic_cast<NumberValue *>(x.get());
    if (!v) {
      throw std::runtime_error("Not a number");
    }
    if (v->n != test.expected) {
      throw std::runtime_error((std::stringstream()
                                << test.name << ": Expected " << test.expected
                                << ", got " << v->n)
                                   .str());
    }
  }
}
