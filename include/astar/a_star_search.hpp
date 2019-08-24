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

template<std::size_t kMazeSize>
class AStarSearch {
public:
    explicit AStarSearch(const Coordinate &start, const Coordinate &goal) :
            start_(start),
            goal_(goal),
            maze_(),
            nodes_(),
            open_(nodes_),
            target_(),
            has_no_answer_(false),
            has_found_answer_(false) {

        nodes_[goal_].id = goal_;

        nodes_[start_].setAll(start_, start_, CalculateHeuristic(start_), AStarNode::State::kOpen);
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
            target_ = top_node.id;
            nodes_[goal_] = top_node;
            has_found_answer_ = true;
            return;
        }

#if defined(ENABLE_COUT)
        std::cout << "top_node: " << top_node.id << " |  " << std::bitset<8>(maze_[top_node.id].flags) << std::endl;
#endif

        if (!maze_[top_node.id].HasCheckedWall()) {
            target_ = top_node.getCoordinate();
            return;
        }

        std::deque<AStarNode> adjacent_nodes;

        for (const auto d : {Wall::Direction::kNorth, Wall::Direction::kEast,
                             Wall::Direction::kSouth, Wall::Direction::kWest}) {
            auto n = top_node;
            if (!maze_[top_node.id].WallExists(d)) {
                switch (d) {
                    case Wall::Direction::kNorth:
                        ++n.id.y;
                        break;
                    case Wall::Direction::kEast:
                        ++n.id.x;
                        break;
                    case Wall::Direction::kSouth:
                        --n.id.y;
                        break;
                    case Wall::Direction::kWest:
                        --n.id.x;
                        break;
                }
                if (maze_.IsOnRange(n.getCoordinate())) {
                    adjacent_nodes.emplace_back(n);
                }
            }
        }

        open_.pop();

#if defined(ENABLE_COUT)
        std::cout << "adj: ";
#endif
        for (const auto &m : adjacent_nodes) {
            const auto h = CalculateHeuristic(top_node.id);
            const auto g = top_node.cost_f - h;
            const auto f_tmp = g + CalculateHeuristic(m.id) + 1;

#if defined(ENABLE_COUT)
            std::cout << m.id << "[" << f_tmp << "], ";
#endif
            const auto is_in_open = nodes_[m.id].state == AStarNode::State::kOpen;
            const auto is_in_close = nodes_[m.id].state == AStarNode::State::kClose;

            if (!is_in_open && !is_in_close) {
                nodes_[m.id].setAll(m.id, top_node.id, f_tmp, AStarNode::State::kOpen);
                open_.emplace(nodes_[m.id]);
            } else if (is_in_open && f_tmp < nodes_[m.id].cost_f) {
                nodes_[m.id].setAll(m.id, top_node.id, f_tmp, AStarNode::State::kOpen);
                open_.emplace(nodes_[m.id]);
            } else if (is_in_close && f_tmp < nodes_[m.id].cost_f) {
                nodes_[m.id].setAll(m.id, top_node.id, f_tmp, AStarNode::State::kOpen);
                open_.emplace(nodes_[m.id]);
            } else {

            }
        }
#if defined(ENABLE_COUT)
        std::cout << std::endl;
#endif

        nodes_[top_node.id].state = AStarNode::State::kClose;
        CalculateNextNode();
    }

    const auto &CalculateOptimalRoute() const {
        static std::deque<Coordinate> optimal_route;
        optimal_route.clear();

        if (!has_found_answer_) { return optimal_route; }

        AStarNode n = nodes_[goal_];
        while (n.id != start_) {
            optimal_route.emplace_front(n.id);
            n = nodes_[n.parent_id];
        }
        optimal_route.emplace_front(nodes_[start_].id);

        return optimal_route;
    }

    const Coordinate &GetNextNodeCoordinate() const { return target_; }

    const AStarNode &GetGoalNode() const { return nodes_[goal_]; }

    bool HasFoundAnswer() const { return has_found_answer_; }

    bool HasNoAnswer() const { return has_no_answer_; }

    void SetWall(const Coordinate &c, const Wall &w) { maze_[c] = w; }

private:
    float CalculateHeuristic(const AStarNode &node) const {
        return CalculateHeuristic(node.id);
    }

    float CalculateHeuristic(const Coordinate &c) const {
        return CalculateDistance(c, goal_);
    }

    static float CalculateDistance(const auto &p1, const auto &p2) {
        return std::sqrt(static_cast<float>((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)));
    }

    bool IsGoal(const AStarNode &node) const {
        return node.id == goal_;
    }

    const Coordinate start_, goal_;
    Maze<Wall, kMazeSize> maze_;
    Maze<AStarNode, kMazeSize> nodes_;
    AStarPriorityQueue<kMazeSize> open_;

    Coordinate target_;
    bool has_no_answer_;
    bool has_found_answer_;
};