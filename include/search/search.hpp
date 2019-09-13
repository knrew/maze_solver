//
// Created by ryunosuke on 2019/09/13.
//

#pragma once

#include "../common/maze.hpp"
#include "../common/wall.hpp"
#include "../astar/astar.hpp"

namespace maze_solver {

    template<std::size_t kMazeSize>
    class Search {
    public:
        Search() : maze_() {}

        Maze<Wall, kMazeSize> maze_;
        A
    };
}