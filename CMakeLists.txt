cmake_minimum_required(VERSION 3.14)
project(maze_solver)

set(CMAKE_CXX_STANDARD 14)

#add_executable(maze_solver main.cpp include/astar/astar.hpp include/common/coordinate.hpp include/common/maze.hpp include/io/maze_reader.hpp include/io/route_writer.hpp include/astar/open_list.hpp include/common/wall.hpp include/astar/node.hpp include/common/utility.hpp include/common/route.hpp include/search/search.hpp)

add_definitions(-DMAZE_SIZE=\(16\))
add_definitions(-DSTART_X=\(0\))
add_definitions(-DSTART_Y=\(0\))
add_definitions(-DGOAL_X=\(7\))
add_definitions(-DGOAL_Y=\(7\))

add_definitions(-DDEFAULT_MAZE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/../maze_data/maze0010.txt")
add_definitions(-DDEFAULT_SEARCH_OUTPUT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/search.csv")
add_definitions(-DDEFAULT_SHORTEST_OUTPUT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/shortest.csv")

#add_definitions(-DASTAR_SOLVER_DEBUG)

add_subdirectory(examples)