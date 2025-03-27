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
    if (auto x = std::get_if<double>(&value)) {
      std::cout << *x;
    } else if (std::get_if<std::shared_ptr<CFunction>>(&value)) {
      std::cout << "<c function>";
    }
    return std::shared_ptr<CFunction>(new Print());
  };
};

export void load_all(Interpreter &i) {
  i.variables["add"] =
      std::make_unique<Value>(std::shared_ptr<CFunction>(new Add()));
  i.variables["print"] =
      std::make_unique<Value>(std::shared_ptr<CFunction>(new Print()));
}

} // namespace upl2::interpreter::builtins
