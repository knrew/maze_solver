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
        Search(const Coordinate &start, const Coordinate &goal) :
//        current_(start),
        maze_(), astar(start, goal) {
//            astar.solve(maze_, start);
//            shortest_ = astar.GetShortestRoute();
//            CalculateNext();
        }

//        void solve() {
//            astar.solve(maze_, current_);
//            shortest_ = astar.GetShortestRoute();
//            CalculateNext();
//        }

//        void SetCurrent(const Coordinate &current) { current_ = current; }

//        const Coordinate &GetNext() const { return next_; }

        void SetWall(const Coordinate &c, const Wall &w) { maze_[c] = w; }

//        auto GetShortestRoute() const { return astar.GetShortestRoute(); }

//    protected:
//        void CalculateNext() {
//            for (std::size_t i = 0; i < shortest_.size(); ++i) {
//                if (shortest_[i] == current_) {
//                    next_ = shortest_[i + 1];
//                }
//            }
//
//            std::cout << "poyo" << std::endl;
////            return shortest_[shortest_.size() - 1];
//        }

        static Wall::Direction GetDirection(const Coordinate &current, const Coordinate &next) {
            if (current.x == next.x && current.y + 1 == next.y) { return Wall::Direction::kNorth; }
            if (current.x + 1 == next.x && current.y == next.y) { return Wall::Direction::kEast; }
            if (current.x == next.x && current.y - 1 == next.y) { return Wall::Direction::kSouth; }
            if (current.x - 1 == next.x && current.y == next.y) { return Wall::Direction::kWest; }

            return Wall::Direction::kNorth;
        }

//        Coordinate current_;
//        Coordinate next_;
        Maze<Wall, kMazeSize> maze_;
        astar::AStar<kMazeSize> astar;
//        Route shortest_;
    };
}