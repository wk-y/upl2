#include "value.hpp"
#include "interpreter.hpp"

char const *CallException::what() const noexcept { return "Not callable."; }

Value::~Value() {}

NumberValue::NumberValue(double _n) : n(_n) {}
void NumberValue::call(Interpreter &) {}

Function::Function(void (*_f)(Interpreter &)) : f(_f) {}
void Function::call(Interpreter &i) { f(i); }
