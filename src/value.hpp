#ifndef VALUE_HPP
#define VALUE_HPP

#include <exception>

class Interpreter;

class CallException : std::exception {
  char const *what() const noexcept;
};

class Value {
public:
  virtual ~Value();
  virtual void call(Interpreter &) = 0;
};

class NumberValue : public Value {
public:
  double n;
  NumberValue(double n);
  virtual void call(Interpreter &);
};

class Function : public Value {
  void (*f)(Interpreter &);

public:
  Function(void (*f)(Interpreter &));
  void call(Interpreter &);
};

#endif
