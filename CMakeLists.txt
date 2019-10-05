cmake_minimum_required(VERSION 3.14)
project(maze_solver)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(tests)

add_executable(maze_solver main.cpp include/maze_constants.hpp include/utility.hpp include/a_star/node.hpp include/a_star/a_star.hpp include/a_star/open_list.hpp)
add_compile_options(concept)