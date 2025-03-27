module;
#include <memory>
#include <sstream>
#include <variant>

export module Ast;

namespace upl2::ast {

export class Symbol;
export class Number;
export class Call;

export typedef std::variant<Symbol, Number, Call> Node;

export class Symbol {

public:
  std::string name;

  Symbol(std::string name_) : name(name_) {}

  bool operator==(Symbol &rhs) { return this->name == rhs.name; }
};

export class Number {
public:
  double value;

  Number(const Number &rhs) = default;
  Number(std::string literal) { std::istringstream(literal) >> value; }

  bool operator==(Number &rhs) { return this->value == rhs.value; }
};

export class Call {
public:
  Call(const Call &x) {
    functor = std::make_unique<Node>(*x.functor.get());
    operand = std::make_unique<Node>(*x.operand.get());
  }

  Call(std::unique_ptr<Node> functor_, std::unique_ptr<Node> operand_)
      : functor(std::move(functor_)), operand(std::move(operand_)) {}
  std::unique_ptr<Node> functor, operand;

  bool operator==(Call &rhs);
};

bool Call::operator==(Call &rhs) {
  return (this->functor.get() == rhs.functor.get()) &&
         this->operand.get() == rhs.operand.get();
}

} // namespace upl2::ast
