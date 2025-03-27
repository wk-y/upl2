module;
#include <memory>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>

export module Interpreter;
import Ast;

namespace upl2::interpreter {

export class Interpreter;

export class Value {
public:
  virtual ~Value() = 0;
  virtual double number() = 0;
  virtual std::string string() = 0;
  virtual std::shared_ptr<Value> call(Interpreter &, ast::Node &) = 0;
};

Value ::~Value() {}

export std::ostream &operator<<(std::ostream &s, std::shared_ptr<Value> &v) {
  s << v->string();
  return s;
}

export class Number : public Value {
public:
  double value;

  Number(double v) : value(v) {}
  ~Number() = default;

  virtual double number() { return value; };
  virtual std::string string() {
    std::ostringstream tmp;
    tmp << value;
    return tmp.str();
  };
  virtual std::shared_ptr<Value> call(Interpreter &, ast::Node &) {
    throw std::runtime_error("numbers are not callable");
  }
  virtual std::shared_ptr<Value> copy() {
    return std::make_shared<Number>(value);
  };
};

export class String : public Value {
public:
  String(std::string s) : value(s) {}
  std::string value;
  virtual double number() { return std::stod(value); };
  virtual std::string string() { return value; };
  virtual std::shared_ptr<Value> call(Interpreter &, ast::Node &) {
    throw std::runtime_error("string is not callable");
  }
};

export class CFunction : public Value {
public:
  virtual double number() {
    throw std::runtime_error("functions are not numbers");
  };
  virtual std::string string() { return "<function>"; };
};

export class Interpreter {
public:
  std::unordered_map<std::string, std::shared_ptr<Value>> variables;

  std::shared_ptr<Value> run(ast::Node &node) {
    return std::visit([this](auto &&a) { return run(a); }, node);
  }

  std::shared_ptr<Value> run(ast::Call &a) {
    auto fn = run(*a.functor.get());

    return fn->call(*this, *a.operand.get());
  }

  std::shared_ptr<Value> run(ast::Number &n) {
    return std::unique_ptr<Value>(new Number(n.value));
  }

  std::shared_ptr<Value> run(ast::StringNode &s) {
    return std::make_shared<String>(s.value);
  }

  std::shared_ptr<Value> run(ast::Symbol &s) {
    if (variables.contains(s.name))
      return variables[s.name];
    throw std::runtime_error("variable not defined");
  }
};

} // namespace upl2::interpreter
