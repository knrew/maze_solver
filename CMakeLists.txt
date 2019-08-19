cmake_minimum_required(VERSION 3.14)
project(maze_solver)

set(CMAKE_CXX_STANDARD 14)

add_executable(maze_solver main.cpp include/a_star_search.hpp include/coordinate.hpp include/maze.hpp include/maze_reader.hpp include/route_writer.hpp include/a_star_container.hpp include/wall.hpp include/a_star_node.hpp)