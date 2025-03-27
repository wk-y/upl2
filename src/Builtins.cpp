module;
#include <iostream>
#include <memory>
#include <stdexcept>
#include <variant>

export module Builtins;
import Interpreter;
import Ast;

namespace upl2::interpreter::builtins {

class Add : public CFunction {
  class AddPartial : public CFunction {
  public:
    AddPartial(double v_) : v(v_) {}
    double v;
    virtual Value run(Interpreter &i, ast::Node a) {
      auto value = i.run(a);
      auto vptr = std::get_if<double>(&value);
      if (!vptr)
        std::runtime_error("attempted to add non-number");
      return double(v + *vptr);
    }
  };

public:
  virtual Value run(Interpreter &i, ast::Node a) {
    auto value = i.run(a);
    auto vptr = std::get_if<double>(&value);
    if (!vptr)
      std::runtime_error("attempted to add non-number");
    return std::shared_ptr<CFunction>(new AddPartial(*vptr));
  };
};

class Print : public CFunction {
public:
  virtual Value run(Interpreter &i, ast::Node a) {
    auto value = i.run(a);
    std::cout << value;
    return std::shared_ptr<CFunction>(new Print());
  };
};

class Assign : public CFunction {
  class AssignPartial : public CFunction {
  public:
    AssignPartial(std::string &n) : name(n) {}

    std::string name;
    virtual Value run(Interpreter &i, ast::Node a) {
      i.variables[name] = std::make_unique<Value>(i.run(a));
      return *i.variables[name].get();
    }
  };

public:
  virtual Value run(Interpreter &, ast::Node a) {
    auto sym = std::get_if<ast::Symbol>(&a);
    if (!sym)
      throw std::runtime_error("attempting to assign to non-symbol");
    return std::shared_ptr<CFunction>(new AssignPartial(sym->name));
  };
};

export void load_all(Interpreter &i) {
  i.variables["+"] =
      std::make_unique<Value>(std::shared_ptr<CFunction>(new Add()));
  i.variables["print"] =
      std::make_unique<Value>(std::shared_ptr<CFunction>(new Print()));
  i.variables["="] =
      std::make_unique<Value>(std::shared_ptr<CFunction>(new Assign()));
}

} // namespace upl2::interpreter::builtins
