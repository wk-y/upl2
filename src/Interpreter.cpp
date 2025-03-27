module;
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>

export module Interpreter;
import Ast;

namespace upl2::interpreter {

export class Interpreter;

export class CFunction;

export typedef std::variant<double, ast::Function, std::shared_ptr<CFunction>>
    Value;

export class CFunction {
public:
  virtual ~CFunction() = 0;
  virtual Value run(Interpreter &, ast::Node) = 0;
};

CFunction::~CFunction() {}

// typedef std::variant<double, Function> Value;

export class Interpreter {
public:
  std::unordered_map<std::string, std::unique_ptr<Value>> variables;

  Value run(ast::Node &node) {
    return std::visit([this](auto &&a) -> Value { return run(a); }, node);
  }

  Value run(ast::Assignment &a) {
    variables[a.lhs.name] = std::make_unique<Value>(run(*a.rhs.get()));
    return *variables[a.lhs.name].get();
  }

  Value run(ast::Call &a) {
    auto fn = run(*a.functor.get());

    if (auto f = std::get_if<std::shared_ptr<CFunction>>(&fn)) {
      return f->get()->run(*this, *a.operand);
    }

    throw std::runtime_error("tried to call a non-function");
  }

  Value run(ast::Function &def) {
    auto x = def;
    return x;
  }

  Value run(ast::Number &n) { return n.value; }
  Value run(ast::Symbol &s) {
    if (variables.contains(s.name))
      return *variables[s.name].get();
    std::runtime_error("variable not defined");
  }
};

} // namespace upl2::interpreter
