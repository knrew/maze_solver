cmake_minimum_required(VERSION 3.14)
project(maze_solver)

set(CMAKE_CXX_STANDARD 14)

add_executable(maze_solver main.cpp include/a_star.hpp include/dijkstra.hpp)