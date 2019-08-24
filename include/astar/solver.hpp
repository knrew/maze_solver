//
// Created by ryunosuke on 2019/08/16.
//
#pragma once

#include <cstdint>
#include "../maze.hpp"
#include "../wall.hpp"
#include "node.hpp"
#include "open_list.hpp"

//#define ENABLE_COUT

#if defined(ENABLE_COUT)

#include <iostream>
#include <bitset>

#endif

namespace maze_solver {
    namespace a_star {
        template<std::size_t kMazeSize>
        class Solver {
        public:
            using Route = std::deque<Coordinate>;

            explicit Solver(const Coordinate &start, const Coordinate &goal) :
                    start_(start),
                    goal_(goal),
                    target_(start),
                    maze_(),
                    nodes_(),
                    open_(nodes_),
                    has_no_answer_(false),
                    has_found_answer_(false) {
                nodes_[start_].setCostF(CalculateHeuristic(start_));
                nodes_[start_].toOpen();
                open_.emplace(nodes_[start_]);
            }

            void CalculateNextNode() {
#if defined(ENABLE_COUT)
                std::cout << "----------" << std::endl;
#endif

                if (has_found_answer_) { return; }

                if (open_.empty()) {
                    has_no_answer_ = true;
                    return;
                }

                const auto top_node = open_.top();

                if (IsGoal(top_node)) {
                    target_ = top_node.getCoordinate();
                    nodes_[goal_] = top_node;
                    has_found_answer_ = true;
                    return;
                }

#if defined(ENABLE_COUT)
                std::cout << "top_node: " << top_node.id << " |  " << std::bitset<8>(maze_[top_node.id].flags) << std::endl;
#endif

                if (!maze_[top_node.getCoordinate()].IsKnownAllDirection()) {
                    target_ = top_node.getCoordinate();
                    return;
                }

                static std::deque<Coordinate> adjacent_node_coordinates;
                adjacent_node_coordinates.clear();

                for (const auto d : {Wall::Direction::kNorth, Wall::Direction::kEast,
                                     Wall::Direction::kSouth, Wall::Direction::kWest}) {
                    auto c = top_node.getCoordinate();
                    if (!maze_[top_node.getCoordinate()].WallExists(d)) {
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

                        if (maze_.IsOnRange(c)) {
                            adjacent_node_coordinates.emplace_back(c);
                        }
                    }
                }

                open_.pop();

#if defined(ENABLE_COUT)
                std::cout << "adj: ";
#endif
                for (const auto &m : adjacent_node_coordinates) {
                    const auto f_tmp = CalculateNextCostF(top_node, m);

#if defined(ENABLE_COUT)
                    std::cout << m << "[" << f_tmp << "], ";
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

#if defined(ENABLE_COUT)
                std::cout << std::endl;
#endif

                nodes_[top_node.getCoordinate()].toClose();
                CalculateNextNode();
            }

            const auto &CalculateOptimalRoute() const {
                static Route optimal_route;
                optimal_route.clear();

                if (!has_found_answer_) { return optimal_route; }

                Node n = nodes_[goal_];
                while (n.getCoordinate() != start_) {
                    optimal_route.emplace_front(n.getCoordinate());
                    n = nodes_[n.getParentCoordinate()];
                }
                optimal_route.emplace_front(nodes_[start_].getCoordinate());

                return optimal_route;
            }

            const Coordinate &GetNextNodeCoordinate() const { return target_; }

            const Node &GetGoalNode() const { return nodes_[goal_]; }

            bool HasFoundAnswer() const { return has_found_answer_; }

            bool HasNoAnswer() const { return has_no_answer_; }

            void SetWall(const Coordinate &c, const Wall &w) { maze_[c] = w; }

        private:
            float CalculateNextCostF(const Node &current, const Coordinate &next) const {
                const auto h = CalculateHeuristic(current.getCoordinate());
                const auto g = current.getCostF() - h;
                const auto f = g + CalculateHeuristic(next) + 1;
                return f;
            }

            float CalculateHeuristic(const Node &node) const {
                return CalculateHeuristic(node.getCoordinate());
            }

            float CalculateHeuristic(const Coordinate &c) const {
                return Coordinate::Distance()(c, goal_);
            }

            bool IsGoal(const Node &node) const {
                return node.getCoordinate() == goal_;
            }

            const Coordinate start_, goal_;
            Coordinate target_;
            Maze<Wall, kMazeSize> maze_;
            Nodes<kMazeSize> nodes_;
            OpenList<kMazeSize> open_;
            bool has_no_answer_;
            bool has_found_answer_;
        };
    }
}