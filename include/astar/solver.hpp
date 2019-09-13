//
// Created by ryunosuke on 2019/08/16.
//
#pragma once

#include "../common/mymath.hpp"
#include "../common/route.hpp"
#include "../common/maze.hpp"
#include "../common/wall.hpp"
#include "node.hpp"
#include "open_list.hpp"

#if defined(ASTAR_SOLVER_DEBUG)

#include <iostream>
#include <bitset>

#endif

namespace maze_solver {
    namespace a_star {
        template<const std::size_t kMazeSize>
        class Solver {
        public:
            explicit Solver(const Coordinate &start, const Coordinate &goal) :
                    start_(start), goal_(goal),
                    nodes_(),
                    open_(nodes_),
                    has_no_answer_(false) {

            }

            void solve(const Maze<Wall, kMazeSize> &maze) {
#if defined(ASTAR_SOLVER_DEBUG)
                std::cout << "solver start." << std::endl;
#endif

                open_.clear();
                nodes_[start_].setCostF(CalculateHeuristic(start_));
                nodes_[start_].toOpen();
                open_.emplace(nodes_[start_]);

                search_route.clear();
                while (true) {
                    if (open_.empty()) {
                        has_no_answer_ = true;
                        break;
                    }

                    const auto top_node = open_.top();
                    search_route.emplace_back(top_node.getCoordinate());
                    open_.pop();

#if defined(ASTAR_SOLVER_DEBUG)
                    std::cout << "top:" << top_node.getCoordinate() << "|";
#endif

                    if (IsGoal(top_node)) {
                        nodes_[goal_] = top_node;
                        break;
                    }

#if defined(ASTAR_SOLVER_DEBUG)
                    printf("%x|", maze[top_node.getCoordinate()].flags);
                    std::cout << std::bitset<4>(maze[top_node.getCoordinate()].flags) << "|";
#endif

                    static std::deque<Coordinate> adjacent_node_coordinates;
                    adjacent_node_coordinates.clear();
                    adjacent_node_coordinates = GetAdjacentNodeCoordinates(top_node, maze[top_node.getCoordinate()]);

#if defined(ASTAR_SOLVER_DEBUG)
                    std::cout << "adj:";
#endif
                    for (const auto &m : adjacent_node_coordinates) {
                        const auto f_tmp = CalculateNextCostF(top_node, m);

#if defined(ASTAR_SOLVER_DEBUG)
                        std::cout << m << ",";
#endif

                        const auto update_node = [&]() {
                            nodes_[m].setParentCoordinate(top_node.getCoordinate());
                            nodes_[m].setCostF(f_tmp);
                            nodes_[m].toOpen();
                            open_.emplace(nodes_[m]);
                        };

                        if (!nodes_[m].isOpen() && !nodes_[m].isClose()) {
                            update_node();
                        } else if (f_tmp < nodes_[m].getCostF()) {
                            update_node();
                        } else {}
                    }

#if defined(ASTAR_SOLVER_DEBUG)
                    std::cout << std::endl;
#endif

                    nodes_[top_node.getCoordinate()].toClose();

#if defined(ASTAR_SOLVER_DEBUG)
                    std::cout << "-----" << std::endl;
#endif
                }

                if (has_no_answer_) {
#if defined(ASTAR_SOLVER_DEBUG)
                    std::cout << "this maze cannot solve." << std::endl;
#endif
                }

#if defined(ASTAR_SOLVER_DEBUG)
                std::cout << "solver finish." << std::endl;
#endif
            }

            auto CalculateShortestRoute() const {
                Route optimal_route;
//                if (!has_found_answer_) { return optimal_route; }

                Node n = nodes_[goal_];
                while (n.getCoordinate() != start_) {
                    optimal_route.emplace_front(n.getCoordinate());
                    n = nodes_[n.getParentCoordinate()];
                }
                optimal_route.emplace_front(nodes_[start_].getCoordinate());

                return std::move(optimal_route);
            }

        private:
            auto GetAdjacentNodeCoordinates(const Node &node, const Wall wall) {
                std::deque<Coordinate> adjs;

                for (const auto d : {Wall::Direction::kNorth, Wall::Direction::kEast,
                                     Wall::Direction::kSouth, Wall::Direction::kWest}) {
                    auto c = node.getCoordinate();
                    if (!wall.WallExists(d)) {
                        switch (d) {
                            case Wall::Direction::kNorth:
                                ++c.y;
                                break;
                            case Wall::Direction::kEast:
                                ++c.x;
                                break;
                            case Wall::Direction::kSouth:
                                --c.y;
                                break;
                            case Wall::Direction::kWest:
                                --c.x;
                                break;
                        }

                        if (math::IsOnRange(c, {0, 0}, {kMazeSize - 1, kMazeSize - 1})) {
                            adjs.emplace_back(c);
                        }
                    }
                }

                return std::move(adjs);
            }

            float CalculateNextCostF(const Node &current, const Coordinate &next) const {
                const auto h = CalculateHeuristic(current.getCoordinate());
                const auto g = current.getCostF() - h;
                const auto f = g + CalculateHeuristic(next) + 1;
                return f;
            }

            float CalculateHeuristic(const Coordinate &c) const {
                return std::sqrt((c.x - goal_.x) * (c.x - goal_.x) + (c.y - goal_.y) * (c.y - goal_.y));
            }

            bool IsGoal(const Node &node) const {
                return node.getCoordinate() == goal_;
            }

//            Maze<Wall, kMazeSize> maze_;
            const Coordinate start_, goal_;
            Nodes<kMazeSize> nodes_;
            OpenList<kMazeSize> open_;
            bool has_no_answer_;

        public:
            Route search_route;
            Route shortest_route;
        };
    }
}