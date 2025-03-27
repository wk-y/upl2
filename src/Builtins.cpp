module;
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <variant>

export module Builtins;
import Interpreter;
import Ast;

namespace upl2::interpreter::builtins {

class Add : public Value {
  class AddPartial : public CFunction {
  public:
    AddPartial(double v) : acc(v) {}
    virtual ~AddPartial() = default;
    double acc;
    virtual double number() {
      throw std::runtime_error("<add> is non-numeric");
    }
    virtual std::string string() { return "<add>"; };
    virtual std::shared_ptr<Value> call(Interpreter &i, ast::Node &n) {
      auto v = i.run(n);
      return std::make_shared<Number>(acc + v->number());
    };
  };

public:
  virtual ~Add() = default;
  virtual double number() { throw std::runtime_error("<add> is non-numeric"); }
  virtual std::string string() { return "<add>"; };
  virtual std::shared_ptr<Value> call(Interpreter &i, ast::Node &n) {
    return std::make_shared<AddPartial>(i.run(n)->number());
  };
};

// class Print : public CFunction {
// public:
//   virtual Value run(Interpreter &i, ast::Node a) {
//     auto value = i.run(a);
//     std::cout << value;
//     return std::shared_ptr<CFunction>(new Print());
//   };
// };

class Assign : public Value {
  class AssignPartial : public Value {
  public:
    AssignPartial(std::string s) : symbol_name(s) {};
    std::string symbol_name;
    virtual double number() {
      throw std::runtime_error("<assign partial> is non-numeric");
    }
    virtual std::string string() { return "<assign partial>"; }
    virtual std::shared_ptr<Value> call(Interpreter &i, ast::Node &a) {
      return i.variables[symbol_name] = i.run(a);
    };
  };

public:
  virtual double number() {
    throw std::runtime_error("<assign> is non-numeric");
  }
  virtual std::string string() { return "<assign>"; }
  virtual std::shared_ptr<Value> call(Interpreter &, ast::Node &a) {
    auto symbol = std::get_if<ast::Symbol>(&a);
    if (symbol)
      return std::make_shared<AssignPartial>(symbol->name);
    throw std::runtime_error("cannot assign to non-symbol");
  };
};

class Concat : public Value {
  class ConcatPartial : public Value {
  public:
    ConcatPartial(std::string s) : first(s) {};
    std::string first;
    virtual double number() {
      throw std::runtime_error("<concat partial> is non-numeric");
    }
    virtual std::string string() { return "<concat partial>"; }
    virtual std::shared_ptr<Value> call(Interpreter &i, ast::Node &a) {
      return std::make_shared<String>(first + i.run(a)->string());
    };
  };

public:
  virtual double number() {
    throw std::runtime_error("<assign> is non-numeric");
  }
  virtual std::string string() { return "<assign>"; }
  virtual std::shared_ptr<Value> call(Interpreter &i, ast::Node &a) {
    return std::make_shared<ConcatPartial>(i.run(a)->string());
  };
};

export void load_all(Interpreter &i) {
  i.variables["+"] = std::make_shared<Add>();
  i.variables["="] = std::make_unique<Assign>();
  i.variables["concat"] = std::make_unique<Concat>();
}

} // namespace upl2::interpreter::builtins
