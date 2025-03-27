# Untitled Programming Language 2

Another toy programming language (no relation to Untitled Programming Language).

## Grammar
```
<statement> ::= (<symbol> | <assignment> | <func> | <call>) ";"
<assignment> ::= <symbol> "=" <statement>
<func> ::= <symbol> "=>" <statement>
<call> ::= <symbol> <statement>
```
