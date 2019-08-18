//
// Created by ryunosuke on 2019/08/19.
//

#pragma once

#include <queue>
#include "maze.hpp"
#include <functional>
#include <memory>


struct AStarNode {
    Coordinate coordinate;
    float cost_f;
    int id;
    int parent_id;

    AStarNode() : coordinate(), cost_f(), id(-1), parent_id(-1) {}

    struct LessCost {
        bool operator()(const AStarNode &x, const AStarNode &y) const { return x.cost_f < y.cost_f; }
    };
};

using AStarNodes =std::deque<AStarNode>;

class AStarContainer {
public:
    using container_type = AStarNodes;
    using value_type = typename container_type::value_type;
    using reference = typename container_type::reference;
    using const_reference  = typename container_type::const_reference;
    using size_type = typename container_type::size_type;

    using nodes_ptr = std::shared_ptr<container_type>;
    using nodes_const_ptr = std::shared_ptr<const container_type>;
    using id_container_type = std::deque<int>;
    using compare_type = std::function<bool(const int &, const int &)>;

    explicit AStarContainer(const nodes_ptr &x) :
            ids(),
            nodes(x),
            comp([&](const int &a, const int &b) { return AStarNode::LessCost()((*nodes)[a], (*nodes)[b]); }) {};

    bool empty() const { return ids.empty(); }

    size_type size() const { return ids.size(); }

    const_reference top() const {
        __glibcxx_requires_nonempty();
        return (*nodes)[ids.front()];
    }

    void push(const value_type &x) {
        const auto id = std::hash<Coordinate>()(x.coordinate);
        ids.push_back(id);
        std::push_heap(ids.begin(), ids.end(), comp);
    }

    void push(value_type &&x) {
        const auto id = std::hash<Coordinate>()(x.coordinate);
        ids.push_back(id);
        std::push_heap(ids.begin(), ids.end(), comp);
    }

    template<class... Args>
    void emplace(Args &&... args) {
        ids.emplace_back(std::forward<Args>(args)...);
        push_heap(ids.begin(), ids.end(), comp);
    }

    void pop() {
        pop_heap(ids.begin(), ids.end(), comp);
        ids.pop_back();
    }

protected:
    id_container_type ids;
    const nodes_const_ptr nodes;
    const compare_type comp;
};