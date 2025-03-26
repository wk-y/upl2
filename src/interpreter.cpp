#include "interpreter.hpp"
#include <memory>

Interpreter::Interpreter(std::initializer_list<Value *> _stack) {

  for (auto e = _stack.end(); e-- > _stack.begin();) {
    stack.stack.push_back(std::unique_ptr<Value>(*e));
  }
}

std::unique_ptr<Value> Interpreter::eval_and_pop() {
  stack.stack.back()->call(*this);
  return pop();
}

std::unique_ptr<Value> Interpreter::pop() {
  auto v = std::move(stack.stack.back());
  stack.stack.pop_back();
  return v;
}

void Interpreter::push(std::unique_ptr<Value> v) {
  stack.stack.push_back(std::move(v));
}
