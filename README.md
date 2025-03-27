# Untitled Programming Language 2

Another toy programming language (no relation to Untitled Programming Language).

## Grammar
```
<statement_list> ::= {"(" <statement> ")"}
<statement> ::= (<expr> | <call> | <infix_call>)
<call> ::= <expr> <expr> {<expr>}
<infix_call> ::= <symbol> <infix> <statement>
<expr> ::= <symbol> | <number> | <string> | "(" <statement> ")" | "(" <infix> ")"
<symbol> ::= [a-Z][a-Z0-9]*
<number> ::= [0-9]+(\.[0-9]*)?
<infix> ::= [+-*/=:<>]+
<string> ::= "\"" ([^\\] | \\.)* "\""
```
