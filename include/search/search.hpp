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
        Search() :
                maze_(),
                astar_(),
                shortest_() {
//            astar_.solve(maze_, start, goal);
//            shortest_ = astar_.GetShortestRoute();
        }

        //without maze
        void reset(const Coordinate &current, const Coordinate &goal) {
            goal_ = goal;
            astar_.solve(maze_, current, goal_);
            shortest_ = astar_.GetShortestRoute();
        }

        Coordinate CalculateNext(const Coordinate &current) {
            const auto d = GetDirection(current, shortest_[1]);
            if (maze_[current].WallExists(d)) {
                astar_.solve(maze_, current, goal_);
                shortest_ = astar_.GetShortestRoute();
            }

            const auto ret = shortest_[1];
            if (shortest_[0] == current) {
                shortest_.pop_front();
                return std::move(ret);
            } else {
                return std::move(shortest_[0]);
            }
        }

        void SetWall(const Coordinate &c, const Wall &w) { maze_[c] = w; }

        Route GetShortestRoute() const { return astar_.GetShortestRoute(); }

        Route unvisited() {
            Route ret;
            for (auto it = shortest_.rbegin(); it != shortest_.rend(); ++it) {
                if (!maze_[*it].IsKnownAllDirection()) { ret.emplace_back(*it); }
            }
            return std::move(ret);
        }

    protected:
        static Wall::Direction GetDirection(const Coordinate &current, const Coordinate &next) {
            if (current.x == next.x && current.y + 1 == next.y) { return Wall::Direction::kNorth; }
            if (current.x + 1 == next.x && current.y == next.y) { return Wall::Direction::kEast; }
            if (current.x == next.x && current.y - 1 == next.y) { return Wall::Direction::kSouth; }
            if (current.x - 1 == next.x && current.y == next.y) { return Wall::Direction::kWest; }

            std::cout << "????" << std::endl;
            return Wall::Direction::kNorth;
        }

        Coordinate goal_;
        Maze<Wall, kMazeSize> maze_;
        astar::AStar<kMazeSize> astar_;
        Route shortest_;
    };
}