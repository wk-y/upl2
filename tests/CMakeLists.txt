add_executable(TestAdd test_add.cpp)
target_link_libraries(TestAdd PUBLIC Interpreter Value Builtins)
add_test(NAME TestAdd COMMAND TestAdd)
