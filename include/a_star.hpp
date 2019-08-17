//
// Created by ryunosuke on 2019/08/16.
//
#pragma once

#include "maze.hpp"
#include <cmath>
#include <algorithm>

struct Node {
    Coordinate coordinate;
    float cost_f;

    constexpr Node() : coordinate(), cost_f(0.f) {}

    constexpr Node(const Coordinate &c, const float f) : coordinate(c), cost_f(f) {}

    struct Less {
        bool operator()(const Node &x, const Node &y) const { return x.cost_f < y.cost_f; }
    };
};

using Nodes = std::deque<Node>;

class AStarSearch {
public:
    AStarSearch(const Coordinate &start_coordinate, const Coordinate &goal_coordinate) {
        start_.coordinate = start_coordinate;
        goal_.coordinate = goal_coordinate;

        open_.clear();
        close_.clear();

        setWall({0, 0}, {0b11110001});

        start_.cost_f = CalculateHeuristic(start_);
        open_.emplace_back(start_);
    }

    void setWall(const Coordinate &c, const Wall &w) {
        maze_[c] = w;
    }

    void CalculateNextTargetCoordinate() {
        if (open_.empty()) {
            has_no_answer = true;
            return;
        }

        const auto top_node_iterator = open_.begin();

        if (isGoal(*top_node_iterator)) {
            has_found_answer = true;
            return;
        }

        if (!maze_.HasCheckedWall(top_node_iterator->coordinate)) {
            target_ = top_node_iterator->coordinate;
            return;
        }

        close_.emplace_back(*top_node_iterator);

        std::deque<Node> adj_nodes;
        for (const auto d : {Maze<>::Direction::NORTH, Maze<>::Direction::EAST,
                             Maze<>::Direction::SOUTH, Maze<>::Direction::WEST}) {
            Node n = *top_node_iterator;
            if (maze_.WallExists(top_node_iterator->coordinate, d)) {
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
                    adj_nodes.emplace_back(n);
                }
            }
        }

        for (const auto &m : adj_nodes) {
            const auto h = CalculateHeuristic(*top_node_iterator);
            const auto g = top_node_iterator->cost_f - h;
            const auto f_tmp = g + CalculateHeuristic(m.coordinate) + 1;

            const auto it_open = std::find_if(
                    open_.cbegin(),
                    open_.cend(),
                    [&m](const auto &x) { return x.coordinate == m.coordinate; }
            );

            const auto it_close = std::find_if(
                    close_.cbegin(),
                    close_.cend(),
                    [&m](const auto &x) { return x.coordinate == m; }
            );

            const auto is_in_open = it_open != open_.cend();
            const auto is_in_close = it_close != close_.cend();

            Node node;
            node.coordinate = m.coordinate;
            if (!is_in_open && !is_in_close) {
                node.cost_f = f_tmp;
                open_.emplace_back(node);
            } else if (is_in_open) {
                if (f_tmp < it_open->cost_f) {
                    node.cost_f = f_tmp;
                    open_.emplace_back(node);
                }
            } else {
                if (f_tmp < it_open->cost_f) {
                    close_.erase(it_close);
                    node.cost_f = f_tmp;
                    open_.emplace_back(node);
                }
            }
        }

        std::sort(open_.begin(), open_.end(), Node::Less());
        
        CalculateNextTargetCoordinate();
    }

    const Coordinate &getTargetCoordinate() const {
        return target_;
    }

    float CalculateHeuristic(const Node &node) const {
        return CalculateHeuristic(node.coordinate);
    }

    float CalculateHeuristic(const Coordinate &c) const {
        return CalculateDistance(c, goal_.coordinate);
    }

    template<typename T>
    static float CalculateDistance(const T &p1, const T &p2) {
        return std::sqrt(static_cast<float>((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)));
    }

    bool isGoal(const Node &node) const {
        return node.coordinate == goal_.coordinate;
    }

    Node start_;
    Node goal_;
    Coordinate target_;
    bool has_no_answer = false;
    bool has_found_answer = false;
    Nodes open_;
    Nodes close_;
    Maze<> maze_;
};