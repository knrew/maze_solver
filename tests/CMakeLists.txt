add_executable(astar_test astar_test.cpp)
add_executable(search_test search_test.cpp)

add_definitions(-DMAZE_SIZE=\(16\))
add_definitions(-DSTART_X=\(0\))
add_definitions(-DSTART_Y=\(0\))
add_definitions(-DGOAL_X=\(7\))
add_definitions(-DGOAL_Y=\(7\))

add_definitions(-DDEFAULT_MAZE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/../../maze_data/maze0010.txt")
add_definitions(-DDEFAULT_SEARCH_OUTPUT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/../search.csv")
add_definitions(-DDEFAULT_SHORTEST_OUTPUT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/../shortest.csv")

#add_definitions(-DASTAR_SOLVER_DEBUG)