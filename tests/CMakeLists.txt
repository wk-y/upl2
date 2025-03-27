add_executable(TestLexing TestLexing.cpp)
target_link_libraries(TestLexing PUBLIC Token)
add_test(NAME TestLexing COMMAND TestLexing)

add_executable(TestParser TestParser.cpp)
target_link_libraries(TestParser PUBLIC Parser Ast)
add_test(NAME TestParser COMMAND TestParser)
