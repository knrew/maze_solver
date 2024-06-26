//
// Created by ryunosuke on 2019/08/16.
//
#pragma once

#include "../common/utility.hpp"
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
    namespace astar {
        template<const std::size_t kMazeSize>
        class AStar {
        public:
            explicit AStar() : search_route_(), shortest_route_() {}

            bool solve(const Maze<Wall, kMazeSize> &maze, const Coordinate &start, const Coordinate &goal) {
#if defined(ASTAR_SOLVER_DEBUG)
                std::cout << "solver start." << std::endl;
#endif
                Nodes<kMazeSize> nodes_;
                OpenList<kMazeSize> open_(nodes_);
                bool has_no_answer_ = false;

                nodes_[start].SetCostF(CalculateHeuristic(start, goal
                ));
                nodes_[start].ToOpen();
                open_.emplace(nodes_[start]);

                search_route_.clear();
                while (true) {
                    if (open_.empty()) {
                        has_no_answer_ = true;
                        break;
                    }

                    const auto top_node = open_.top();
                    search_route_.emplace_back(top_node.GetCoordinate());
                    open_.pop();

#if defined(ASTAR_SOLVER_DEBUG)
                    std::cout << "top:" << top_node.getCoordinate() << "|";
#endif

                    if (IsGoal(top_node, goal)) {
                        nodes_[goal] = top_node;
                        break;
                    }

#if defined(ASTAR_SOLVER_DEBUG)
                    printf("%x|", maze[top_node.getCoordinate()].flags);
                    std::cout << std::bitset<4>(maze[top_node.getCoordinate()].flags) << "|";
#endif

                    static std::deque<Coordinate> adjacent_node_coordinates;
                    adjacent_node_coordinates.clear();
                    adjacent_node_coordinates = GetAdjacentNodeCoordinates(top_node, maze[top_node.GetCoordinate()]);

#if defined(ASTAR_SOLVER_DEBUG)
                    std::cout << "adj:";
#endif
                    for (const auto &m : adjacent_node_coordinates) {
                        const auto f_tmp = CalculateNextCostF(top_node, m, goal);

#if defined(ASTAR_SOLVER_DEBUG)
                        std::cout << m << ",";
#endif

                        const auto update_node = [&]() {
                            nodes_[m].SetParentCoordinate(top_node.GetCoordinate());
                            nodes_[m].SetCostF(f_tmp);
                            nodes_[m].ToOpen();
                            open_.emplace(nodes_[m]);
                        };

                        if (!nodes_[m].IsOpen() && !nodes_[m].IsClose()) {
                            update_node();
                        } else if (f_tmp < nodes_[m].GetCostF()) {
                            update_node();
                        } else {}
                    }

#if defined(ASTAR_SOLVER_DEBUG)
                    std::cout << std::endl;
#endif

                    nodes_[top_node.GetCoordinate()].ToClose();

#if defined(ASTAR_SOLVER_DEBUG)
                    std::cout << "-----" << std::endl;
#endif
                }

                if (has_no_answer_) {
#if defined(ASTAR_SOLVER_DEBUG)
                    std::cout << "this maze cannot solve." << std::endl;
#endif
                    return false;
                }

                shortest_route_.clear();

                Node n = nodes_[goal];
                while (n.GetCoordinate() != start) {
                    shortest_route_.emplace_front(n.GetCoordinate());
                    n = nodes_[n.GetParentCoordinate()];
                }
                shortest_route_.emplace_front(nodes_[start].GetCoordinate());

#if defined(ASTAR_SOLVER_DEBUG)
                std::cout << "solver finish." << std::endl;
#endif
                return true;
            }

            auto GetSearchRoute() const { return std::move(search_route_); }

            auto GetShortestRoute() const { return std::move(shortest_route_); }

        private:
            auto GetAdjacentNodeCoordinates(const Node &node, const Wall wall) {
                std::deque<Coordinate> adjs;

                for (const auto d : {Wall::Direction::kNorth, Wall::Direction::kEast,
                                     Wall::Direction::kSouth, Wall::Direction::kWest}) {
                    auto c = node.GetCoordinate();
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

            float CalculateNextCostF(const Node &current, const Coordinate &next, const Coordinate &goal) const {
                const auto h = CalculateHeuristic(current.GetCoordinate(), goal);
                const auto g = current.GetCostF() - h;
                const auto f = g + CalculateHeuristic(next, goal) + 1;
                return f;
            }

            static float CalculateHeuristic(const Coordinate &c, const Coordinate &goal) {
                return std::sqrt((c.x - goal.x) * (c.x - goal.x) + (c.y - goal.y) * (c.y - goal.y));
            }

            static bool IsGoal(const Node &node, const Coordinate &goal) {
                return node.GetCoordinate() == goal;
            }

            Route search_route_;
            Route shortest_route_;
        };
    }
}