//
// Created by ryunosuke on 2019/10/05.
//

#pragma once

#include "../maze.hpp"
#include "node.hpp"
#include "open_list.hpp"

namespace maze_solver::a_star {
    class AStar {
    public:
        using Nodes = Maze<Node>;

        enum class State : uint8_t {
            kOk, kFoundAnswer, kNoAnswer
        };

        AStar() {}

        void Solve(const Maze<Wall> &maze, const Coordinate &start, const Coordinate &goal) {
            auto nodes = Nodes();
            auto open = OpenList();
            auto state = State::kOk;

            open.bindNodesPtr(&nodes);
            open.bindCompareFunction();

            nodes[start].cost_f = CalculateHeuristic(start, goal);
            nodes[start].state = Node::State::kOpen;

            open.emplace(start);



        }

        static float CalculateHeuristic(const Coordinate &c, const Coordinate &goal) {
            return std::sqrt((c.x - goal.x) * (c.x - goal.x) + (c.y - goal.y) * (c.y - goal.y));
        }


    };

}