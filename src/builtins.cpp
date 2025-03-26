#include "interpreter.hpp"
#include <stdexcept>
namespace upl2 {
namespace builtins {

void add(Interpreter &i) {
  i.pop();
  auto v1 = i.eval_and_pop();
  auto v2 = i.eval_and_pop();
  auto x = dynamic_cast<NumberValue *>(v1.get());
  auto y = dynamic_cast<NumberValue *>(v2.get());
  if (!x || !y) {
    throw std::runtime_error("Attempted addition on non-number");
  }
  x->n += y->n;
  i.push(std::move(v1));
}

} // namespace builtins
} // namespace upl2
