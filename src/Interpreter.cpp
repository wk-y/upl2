module;
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

export module Interpreter;
import Ast;

namespace upl2::interpreter {

export class Interpreter;

class Function;

typedef std::variant<double, ast::Function> Value;

class Function : public std::function<Value(Interpreter &, ast::Node &)> {};

// typedef std::variant<double, Function> Value;

export class Interpreter {
  std::unordered_map<std::string, std::unique_ptr<Value>> variables;

public:
  Value run(ast::Node &node) {
    return std::visit([this](auto &&a) -> Value { return run(a); }, node);
  }

  Value run(ast::Assignment &a) {
    variables[a.lhs.name] = std::make_unique<Value>(run(*a.rhs.get()));
    return *variables[a.lhs.name].get();
  }

  Value run(ast::Call &a) {
    auto fn = run(*a.functor.get());
    // auto f = std::get_if<Function>(&fn);
    // if (!f)
    //   throw std::runtime_error("tried to call a non-function");
    // return (*f)(*this, *a.operand.get());
    return double(0); // fixme
  }

  Value run(ast::Function &def) {
    auto x = def;
    return x;
  }

  Value run(ast::Number &n) { return n.value; }
  Value run(ast::Symbol &s) { return *variables[s.name].get(); }
};

} // namespace upl2::interpreter
