# Untitled Programming Language 2

Another toy programming language (no relation to Untitled Programming Language).

## Grammar
```
<statement_list> ::= {<statement> ";"}
<statement> ::= (<expr> | <assignment> | <func> | <call>)
<assignment> ::= <expr> "=" <statement>
<func> ::= <expr> "=>" <statement>
<call> ::= <expr> <statement>
<expr> ::= <symbol> | "(" <statement> ")"
```
