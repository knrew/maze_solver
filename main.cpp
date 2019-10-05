#include <iostream>
#include "include/maze.hpp"
#include "include/wall.hpp"
#include "include/coordinate.hpp"
#include "include/utility.hpp"
#include "include/a_star/a_star.hpp"

std::tuple<int, int, int> poyo() {
    return {1, 1, 1};
}

int main() {
    using namespace maze_solver;

    const auto maze = Maze<Wall>();

    auto searcher = a_star::AStar();

    searcher.Solve(maze, {0, 0}, {0, 0});

    return 0;
}
