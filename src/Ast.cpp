module;
#include <iostream>
#include <memory>
#include <sstream>
#include <variant>

export module Ast;

namespace upl2::ast {

export class Symbol;
export class Number;
export class Assignment;
export class Function;
export class Call;

#define NODE_DEF                                                               \
  export typedef std::variant<Symbol, Number, Assignment, Function, Call> Node;
NODE_DEF

class Symbol {

public:
  std::string name;

  Symbol(std::string name_) : name(name_) {}

  bool operator==(Symbol &rhs) { return this->name == rhs.name; }
};

class Number {
public:
  double value;

  Number(std::string literal) { std::istringstream(literal) >> value; }

  bool operator==(Number &rhs) { return this->value == rhs.value; }
};

class Assignment {
public:
  std::unique_ptr<Node> lhs, rhs;
  bool operator==(Assignment &rhs);
};

class Function {
public:
  std::unique_ptr<Node> operand, body;
  bool operator==(Function &rhs);
};

class Call {
public:
  std::unique_ptr<Node> functor, operand;

  bool operator==(Call &rhs);
};

bool Assignment::operator==(Assignment &rhs) {
  return this->lhs == rhs.lhs && this->rhs == rhs.rhs;
}

bool Function::operator==(Function &rhs) {
  return this->operand == rhs.operand && this->body == rhs.body;
}

bool Call::operator==(Call &rhs) {
  return (this->functor.get() == rhs.functor.get()) &&
         this->operand.get() == rhs.operand.get();
}

NODE_DEF

} // namespace upl2::ast
