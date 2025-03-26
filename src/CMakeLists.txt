add_library(Builtins builtins.cpp builtins.hpp)
add_library(Interpreter interpreter.cpp interpreter.hpp)
add_library(Value value.cpp value.hpp)

link_libraries(Builtins PUBLIC Interpreter)
link_libraries(Interpreter PUBLIC Value)
