cmake_minimum_required(VERSION 3.14)
project(maze_solver)

set(CMAKE_CXX_STANDARD 14)

add_executable(maze_solver main.cpp include/astar/solver.hpp include/common/coordinate.hpp include/common/maze.hpp include/io/maze_reader.hpp include/io/route_writer.hpp include/astar/open_list.hpp include/common/wall.hpp include/astar/node.hpp include/utility/mymath.hpp)

add_definitions(-DMAZE_SIZE=16)