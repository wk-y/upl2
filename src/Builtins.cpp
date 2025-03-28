module;
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <variant>
#include <vector>

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
    throw std::runtime_error("<concat> is non-numeric");
  }
  virtual std::string string() { return "<concat>"; }
  virtual std::shared_ptr<Value> call(Interpreter &i, ast::Node &a) {
    return std::make_shared<ConcatPartial>(i.run(a)->string());
  };
};

class FuncOp : public Value {
  class FuncOpPartial : public Value {
    std::string parameter;

  public:
    FuncOpPartial(std::string s) : parameter(s) {}
    virtual double number() {
      throw std::runtime_error("<func partial> is non-numeric");
    };
    virtual std::string string() { return "<func partial>"; }
    virtual std::shared_ptr<Value> call(Interpreter &i, ast::Node &n) {
      std::unordered_map<std::string, std::shared_ptr<Value>> copy =
          i.variables;
      return std::make_shared<UserFunction>(parameter, n, std::move(copy));
    }
  };

public:
  virtual double number() { throw std::runtime_error("<func> is non-numeric"); }
  virtual std::string string() { return "<func>"; }
  virtual std::shared_ptr<Value> call(Interpreter &, ast::Node &a) {
    auto parameter = std::get_if<ast::Symbol>(&a);
    if (!parameter)
      throw std::runtime_error("non-symbol passed to <func>");

    return std::make_shared<FuncOpPartial>(parameter->name);
  }
};

class Capture : public Value {
  virtual double number() {
    throw std::runtime_error("<capture> is non-numeric");
  }
  virtual std::string string() { return "<capture>"; }
  virtual std::shared_ptr<Value> call(Interpreter &i, ast::Node &n) {
    auto sym = std::get_if<ast::Symbol>(&n);
    if (!sym) {
      throw std::runtime_error("non-symbol passed to capture");
    }
    i.current_captures[sym->name] = i.variables[sym->name];
    return std::make_shared<Capture>();
  }
};

// todo: consider makig this a proper language feature
class Let : public Value {
  class LetPartial : public Value {
    std::shared_ptr<Value> value;

  public:
    LetPartial(std::shared_ptr<Value> v) : value(v) {}

    virtual double number() {
      throw std::runtime_error("<let partial> is non-numeric");
    }
    virtual std::string string() { return "<let partial>"; }
    virtual std::shared_ptr<Value> call(Interpreter &i, ast::Node &a) {
      if (auto x = std::get_if<ast::Symbol>(&a)) {
        if (x->name == "end")
          return value;
      }
      auto v = i.run(a);
      return std::make_shared<LetPartial>(v);
    };
  };

public:
  virtual double number() { throw std::runtime_error("<let> is non-numeric"); }
  virtual std::string string() { return "<let>"; }
  virtual std::shared_ptr<Value> call(Interpreter &i, ast::Node &a) {
    return std::make_shared<LetPartial>(i.run(a));
  };
};

export void load_all(Interpreter &i) {
  i.variables["+"] = std::make_shared<Add>();
  i.variables["="] = std::make_shared<Assign>();
  i.variables["concat"] = std::make_shared<Concat>();
  i.variables["let"] = std::make_shared<Let>();
  i.variables["=>"] = std::make_shared<FuncOp>();
  i.variables["capture"] = std::make_shared<Capture>();
}

} // namespace upl2::interpreter::builtins
