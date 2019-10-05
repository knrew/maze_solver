#include <iostream>
#include "include/maze.hpp"
#include "include/wall.hpp"
#include "include/coordinate.hpp"
#include "include/maze_math.hpp"

int main() {
    using namespace maze_solver;

    Maze<Coordinate> maze;

    int i = 0;
    for (auto &m : maze) {
        m = Coordinate(i, i * 2);
        ++i;
    }

    for (const auto &m : maze) {
        std::cout << m << "|" << std::boolalpha << math::IsOnRange(m) << std::endl;
    }

    return 0;
}
