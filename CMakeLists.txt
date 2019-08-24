cmake_minimum_required(VERSION 3.14)
project(maze_solver)

set(CMAKE_CXX_STANDARD 14)

add_executable(maze_solver main.cpp include/astar/solver.hpp include/coordinate.hpp include/maze.hpp include/csv/maze_reader.hpp include/csv/route_writer.hpp include/astar/open_list.hpp include/wall.hpp include/astar/node.hpp)

add_definitions(-DMAZE_SIZE=64)