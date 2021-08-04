cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

include_directories(${PROJECT_BINARY_DIR}/src)

add_library(b64decode b64decode.c)
add_library(pofix-stack pofix-stack.c)
add_library(pofix-calc pofix-calc.c)
add_library(infix-calc infix-calc.c)

add_executable(foo foo.c)
target_link_libraries(foo b64decode infix-calc pofix-calc pofix-stack)