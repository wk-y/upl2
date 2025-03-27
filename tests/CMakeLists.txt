add_executable(TestLexing TestLexing.cpp)
target_link_libraries(TestLexing PUBLIC Token)
add_test(NAME TestLexing COMMAND TestLexing)

add_executable(TestParser TestParser.cpp)
target_link_libraries(TestParser PUBLIC Parser Ast Token)
add_test(NAME TestParser COMMAND TestParser)

add_executable(TestInterpreter TestInterpreter.cpp)
target_link_libraries(TestInterpreter PUBLIC Interpreter Ast Parser Builtins)
add_test(NAME TestInterpreter COMMAND TestInterpreter)
