add_library(Ast)
target_sources(Ast PUBLIC FILE_SET CXX_MODULES FILES Ast.cpp)

add_library(Token)
target_sources(Token PUBLIC FILE_SET CXX_MODULES FILES Token.cpp)


add_library(Parser)
target_sources(Parser PUBLIC FILE_SET CXX_MODULES FILES Parser.cpp)
target_link_libraries(Parser PUBLIC Token Ast)

add_library(Interpreter)
target_sources(Interpreter PUBLIC FILE_SET CXX_MODULES FILES Interpreter.cpp)
target_link_libraries(Interpreter PUBLIC Ast)

add_library(Builtins)
target_sources(Builtins PUBLIC FILE_SET CXX_MODULES FILES Builtins.cpp)
target_link_libraries(Builtins PUBLIC Interpreter Ast)


add_executable(Repl)
target_sources(Repl PUBLIC Repl.cpp)
target_link_libraries(Repl PUBLIC Builtins Interpreter Parser Token)
