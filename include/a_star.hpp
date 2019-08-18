//
// Created by ryunosuke on 2019/08/16.
//
#pragma once

#include "maze.hpp"
#include <cmath>
#include <algorithm>

//#define ENABLE_COUT

struct Node {
    Coordinate coordinate;
    Coordinate parent_coordinate;
    float cost_f;

    constexpr Node() : coordinate(), parent_coordinate(), cost_f(0.f) {}

    constexpr explicit Node(const Coordinate &c) : coordinate(c), parent_coordinate(), cost_f(0.f) {}

    constexpr explicit Node(const Coordinate &c, const float f) : coordinate(c), parent_coordinate(), cost_f(f) {}

    constexpr explicit Node(const Coordinate &c, const Coordinate &parent_c, const float f) :
            coordinate(c), parent_coordinate(parent_c), cost_f(f) {}

    struct LessCost {
        bool operator()(const auto &x, const auto &y) const { return x.cost_f < y.cost_f; }
    };
};

using Nodes = std::deque<Node>;

class AStarSearch {
public:
    AStarSearch(const Coordinate &start_coordinate, const Coordinate &goal_coordinate)
            : start_(), goal_(), target_(), has_no_answer(false), has_found_answer(false), open_(), close_(), maze_() {
#if defined(ENABLE_COUT)
        std::cout << "AStarSearch start." << std::endl;
#endif
        start_.coordinate = start_coordinate;
        goal_.coordinate = goal_coordinate;

        open_.clear();
        close_.clear();

        Wall wall;
        wall.flags = 0b00000000;
        SetWall(start_.coordinate, wall);

        start_.cost_f = CalculateHeuristic(start_);
        start_.parent_coordinate = {0, 0};
        open_.emplace_back(start_);
    }

    void SetWall(const Coordinate &c, const Wall &w) {
        maze_[c] = w;
    }

    void CalculateNextNode() {
#if defined(ENABLE_COUT)
        std::cout << "----------" << std::endl;
#endif

        if (has_found_answer) { return; }

        if (open_.empty()) {
            has_no_answer = true;
            return;
        }

        const auto top_node_iterator = open_.begin();

        if (IsGoal(*top_node_iterator)) {
            target_ = top_node_iterator->coordinate;
            goal_ = (*top_node_iterator);
            has_found_answer = true;
            return;
        }

#if defined(ENABLE_COUT)
        std::cout << "top_node: " << top_node_iterator->coordinate << " |  "
                  << std::bitset<8>(maze_[top_node_iterator->coordinate].flags) << std::endl;
#endif
        if (!maze_.HasCheckedWall(top_node_iterator->coordinate)) {
            target_ = top_node_iterator->coordinate;
            return;
        }
        close_.emplace_back(*top_node_iterator);

        Nodes adjacent_nodes;
        for (const auto d : {Maze<>::Direction::NORTH, Maze<>::Direction::EAST,
                             Maze<>::Direction::SOUTH, Maze<>::Direction::WEST}) {
            auto n = *top_node_iterator;
            if (!maze_.WallExists(top_node_iterator->coordinate, d)) {
                switch (d) {
                    case Maze<>::Direction::NORTH:
                        ++n.coordinate.y;
                        break;
                    case Maze<>::Direction::EAST:
                        ++n.coordinate.x;
                        break;
                    case Maze<>::Direction::SOUTH:
                        --n.coordinate.y;
                        break;
                    case Maze<>::Direction::WEST:
                        --n.coordinate.x;
                        break;
                }
                if (Maze<>::IsOnRange(n.coordinate)) {
                    adjacent_nodes.emplace_back(n);
                }
            }
        }

#if defined(ENABLE_COUT)
        std::cout << "adj: ";
#endif
        for (const auto &m : adjacent_nodes) {
#if defined(ENABLE_COUT)
            std::cout << m.coordinate << ", ";
#endif
            const auto h = CalculateHeuristic(*top_node_iterator);
            const auto g = top_node_iterator->cost_f - h;
            const auto f_tmp = g + CalculateHeuristic(m.coordinate) + 1;

            const auto iterator_open = std::find_if(open_.cbegin(), open_.cend(),
                                                    [&m](const auto &x) { return x.coordinate == m.coordinate; });

            const auto iterator_close = std::find_if(close_.cbegin(), close_.cend(),
                                                     [&m](const auto &x) { return x.coordinate == m.coordinate; });

            const auto is_in_open = iterator_open != open_.cend();
            const auto is_in_close = iterator_close != close_.cend();

            Node node;
            node.coordinate = m.coordinate;
            if (!is_in_open && !is_in_close) {
                node.cost_f = f_tmp;
                node.parent_coordinate = top_node_iterator->coordinate;
                open_.emplace_back(node);
            } else if (is_in_open && f_tmp < iterator_open->cost_f) {
                node.cost_f = f_tmp;
                node.parent_coordinate = top_node_iterator->coordinate;
                open_.emplace_back(node);
            } else if (is_in_close && f_tmp < iterator_close->cost_f) {
                close_.erase(iterator_close);
                node.cost_f = f_tmp;
                node.parent_coordinate = top_node_iterator->coordinate;
                open_.emplace_back(node);
            } else {}
        }
#if defined(ENABLE_COUT)
        std::cout << std::endl;
#endif

        open_.erase(top_node_iterator);
        std::sort(open_.begin(), open_.end(), Node::LessCost());

        CalculateNextNode();
    }

    const auto &GetTargetCoordinate() const {
        return target_;
    }

    const auto &GetGoalNode() const {
        return goal_;
    }

    bool HasFoundAnswer() const {
        return has_found_answer;
    }

    bool HasNoAnswer() const {
        return has_no_answer;
    }

    const auto &CalculateOptimalRoute() const {
        const auto find_node_if_parents = [&](const auto &parents_coordinate) {
            return std::find_if(close_.cbegin(), close_.cend(),
                                [&parents_coordinate](const auto &x) { return x.coordinate == parents_coordinate; });
        };

        static std::deque<Coordinate> optimal_route;
        optimal_route.clear();

        if (!has_found_answer) { return optimal_route; }

        Node n = GetGoalNode();
        while (!(n.parent_coordinate == start_.coordinate)) {
            optimal_route.emplace_back(n.coordinate);
            n = *find_node_if_parents(n.parent_coordinate);
        }
        optimal_route.emplace_back(n.coordinate);
        optimal_route.emplace_back(start_.coordinate);
        std::reverse(optimal_route.begin(), optimal_route.end());

        return optimal_route;
    }

private:
    float CalculateHeuristic(const Node &node) const {
        return CalculateHeuristic(node.coordinate);
    }

    float CalculateHeuristic(const Coordinate &c) const {
        return CalculateDistance(c, goal_.coordinate);
    }

    static float CalculateDistance(const auto &p1, const auto &p2) {
        return std::sqrt(static_cast<float>((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)));
    }

    bool IsGoal(const auto &node) const {
        return node.coordinate == goal_.coordinate;
    }

    Node start_;
    Node goal_;
    Coordinate target_;
    bool has_no_answer;
    bool has_found_answer;
    Nodes open_;
    Nodes close_;
    Maze<> maze_;
};