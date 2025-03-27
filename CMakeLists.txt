cmake_minimum_required(VERSION 3.28)

project(Upl2)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_testing()


add_subdirectory(src)
add_subdirectory(tests)

