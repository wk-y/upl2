#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "value.hpp"
#include <initializer_list>
#include <memory>
#include <vector>

class StackFrame {
public:
  std::vector<std::unique_ptr<Value>> stack;
};

class Interpreter {
  StackFrame stack;

public:
  Interpreter(std::initializer_list<Value *> stack);

  std::unique_ptr<Value> eval_and_pop();
  std::unique_ptr<Value> pop();
  void push(std::unique_ptr<Value>);
};

#endif
