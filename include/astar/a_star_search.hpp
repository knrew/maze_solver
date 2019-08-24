//
// Created by ryunosuke on 2019/08/16.
//
#pragma once

#include "../maze.hpp"
#include "a_star_priority_queue.hpp"
#include "a_star_node.hpp"
#include <cmath>
#include <algorithm>

//#define ENABLE_COUT

namespace maze_solver {
    namespace a_star {
        template<std::size_t kMazeSize>
        class Solver {
        public:
            using Route = std::deque<Coordinate>;

            explicit Solver(const Coordinate &start, const Coordinate &goal) :
                    start_(start),
                    goal_(goal),
                    maze_(),
                    nodes_(),
                    open_(nodes_),
                    target_(),
                    has_no_answer_(false),
                    has_found_answer_(false) {

                nodes_[start_].setParentCoordinate(start_);
                nodes_[start_].setCost(CalculateHeuristic(start_));
                nodes_[start_].toOpen();
                open_.emplace(nodes_[start_]);

                Wall wall;
                wall.flags = 0b00000000;
                SetWall(start_, wall);
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

                if (!maze_[top_node.getCoordinate()].HasCheckedWall()) {
                    target_ = top_node.getCoordinate();
                    return;
                }

                static std::deque<Node> adjacent_nodes;
                adjacent_nodes.clear();

                for (const auto d : {Wall::Direction::kNorth, Wall::Direction::kEast,
                                     Wall::Direction::kSouth, Wall::Direction::kWest}) {
                    auto n = top_node;
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
                            n.setCoordinate(c);
                            adjacent_nodes.emplace_back(n);
                        }
                    }
                }

                open_.pop();

#if defined(ENABLE_COUT)
                std::cout << "adj: ";
#endif
                for (const auto &m : adjacent_nodes) {
                    const auto f_tmp = CalculateNextCostF(top_node, m);

#if defined(ENABLE_COUT)
                    std::cout << m.id << "[" << f_tmp << "], ";
#endif

                    const auto update_node = [&]() {
                        nodes_[m.getCoordinate()].setParentCoordinate(top_node.getCoordinate());
                        nodes_[m.getCoordinate()].setCost(f_tmp);
                        nodes_[m.getCoordinate()].toOpen();
                        open_.emplace(nodes_[m.getCoordinate()]);
                    };

                    if (!nodes_[m.getCoordinate()].isOpen() && !nodes_[m.getCoordinate()].isClose()) {
                        update_node();
                    } else if (nodes_[m.getCoordinate()].isOpen() && f_tmp < nodes_[m.getCoordinate()].getCost()) {
                        update_node();
                    } else if (nodes_[m.getCoordinate()].isClose() && f_tmp < nodes_[m.getCoordinate()].getCost()) {
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
            float CalculateNextCostF(const Node &current, const Node &next) const {
                const auto h = CalculateHeuristic(current.getCoordinate());
                const auto g = current.getCost() - h;
                const auto f = g + CalculateHeuristic(next.getCoordinate()) + 1;
                return f;
            }

            float CalculateHeuristic(const Node &node) const {
                return CalculateHeuristic(node.getCoordinate());
            }

            float CalculateHeuristic(const Coordinate &c) const {
                return CalculateDistance(c, goal_);
            }

            static float CalculateDistance(const auto &p1, const auto &p2) {
                return std::sqrt(static_cast<float>((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)));
            }

            bool IsGoal(const Node &node) const {
                return node.getCoordinate() == goal_;
            }

            const Coordinate start_, goal_;
            Maze<Wall, kMazeSize> maze_;
            Nodes<kMazeSize> nodes_;
            IDPriorityQueue<kMazeSize> open_;

            Coordinate target_;
            bool has_no_answer_;
            bool has_found_answer_;
        };
    }
}