//
// Created by ryunosuke on 19/07/15.
//

#pragma once

//#include <cstdint>
//#include <deque>


#include <queue>
#include <cmath>
#include <algorithm>
//#include <algorithm>

struct Wall {
    bool has_checked;
    bool north;
    bool east;
    bool south;
    bool west;

    Wall() : has_checked(false), north(false), east(false), south(false), west(false) {}
};

struct Coordinate {
    int8_t x, y;

    Coordinate() : x(0), y(0) {}
};

struct Node {
    enum class State : uint8_t {
        OPEN, CLOSE
    };
    State state;
    Coordinate coordinate;
    Wall wall;
    float cost_f;
    float cost_g;
    float cost_h;

    Node() : state(State::OPEN), coordinate(), wall(), cost_f(0.f), cost_g(0.f), cost_h(0.f) {}
};

struct Compare {
    bool operator()(const Node &x, const Node &y) const { return x.cost_f < y.cost_f; }
};

template<typename _Tp, typename _Sequence = std::vector<_Tp>, typename _Compare  = std::less<typename _Sequence::value_type> >
class AStarPriorityQueue : public std::priority_queue<_Tp, _Sequence, _Compare> {

};

class AStarSearch {
public:
    static constexpr int kMazeSize = 16;

    AStarSearch(const Node &start, const std::deque<Node> &goal) :
            start_(start),
            goal_(goal),
            open_(),
            has_found_path(false) {}

    void Initialize() {
//        start_.cost_g = 0.f;
//        start_.cost_f = start_.cost_h = CalcHeuristic(start_, goal_);
        open_.push(start_);
    }


    void SetWallInfo(const Coordinate coordinate, const Wall wall) {}

    bool CalcNextSquare() {
        if (open_.empty()) { return false; }

        const auto node = open_.top();

        if (!node.wall.has_checked) {
            next_node_ = node;
            return true;
        }

        if (isGoal(node, goal_)) {
            has_found_path = true;
            return true;
        }

//        close_.push(node);

        Node tmp;
        std::deque<Node> adjacent_node;

        for (auto &m : adjacent_node) {
            m.cost_h = CalcHeuristic(m, goal_);
            m.cost_f = m.cost_g + CalcHeuristic(m, goal_);

        }

//        return CalcNextSquare();
    }

    const Node &getNextSquare() const { return next_node_; }

private:
    const std::deque<Node> &GetAdjacentNode(const Node &n) const {
        static Node tmp;
        static std::deque<Node> ret;
        static const auto is_on_range = [](const Node &node) -> bool {
            return (node.coordinate.x >= 0 && node.coordinate.x < kMazeSize &&
                    node.coordinate.y >= 0 && node.coordinate.y < kMazeSize);
        };
        static const auto is_out_range = !is_on_range;

        ret.clear();

        if (n.wall.north) {
            tmp.coordinate.x = n.coordinate.x;
            tmp.coordinate.y = n.coordinate.y + 1;
            if (is_on_range(tmp)) { ret.emplace_back(tmp); }
        }
        if (n.wall.east) {
            tmp.coordinate.x = n.coordinate.x + 1;
            tmp.coordinate.y = n.coordinate.y;
            if (is_on_range(tmp)) { ret.emplace_back(tmp); }
        }
        if (n.wall.south) {
            tmp.coordinate.x = n.coordinate.x - 1;
            tmp.coordinate.y = n.coordinate.y;
            if (is_on_range(tmp)) { ret.emplace_back(tmp); }
        }
        if (n.wall.west) {
            tmp.coordinate.x = n.coordinate.x - 1;
            tmp.coordinate.y = n.coordinate.y;
            if (is_on_range(tmp)) { ret.emplace_back(tmp); }
        }

        return ret;
    }

    static float CalcHeuristic(const Node &n, const std::deque<Node> &goal) {
        return CalcDistance(n, goal.at(0));
    }

    static constexpr float CalcDistance(const Node &n1, const Node &n2) {
        return CalcDistance < float > (n1.coordinate, n2.coordinate);
    }

    template<typename T, typename U>
    static constexpr T CalcDistance(const U &p1, const U &p2) {
        return std::sqrt(static_cast<T>((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)));
    }

    static bool isGoal(const Node &n, const std::deque<Node> &goal) {
        for (const auto &g : goal) {
            if (n.coordinate.x == g.coordinate.x && n.coordinate.y == g.coordinate.y) {
                return true;
            }
        }
        return false;
    }

    const Node start_;
    const std::deque<Node> goal_;
    Node next_node_;
    AStarPriorityQueue<Node, std::deque<Node>, Compare> open_;
    bool has_found_path;
};
