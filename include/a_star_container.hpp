//
// Created by ryunosuke on 2019/08/19.
//

#pragma once

#include <queue>
#include <functional>
#include <memory>
#include "maze.hpp"
#include "a_star_node.hpp"

template<int kMazeSize>
class AStarPriorityQueue {
public:
    using NodeType = AStarNode;
    using NodeContainerType = AStarNodeContainerType;
    using NodeContainerPtr = std::shared_ptr<NodeContainerType>;
    using NodeContainerConstPtr = std::shared_ptr<const NodeContainerType>;

    using IDContainerType = std::deque<int>;
    using CompareFunctionType = std::function<bool(const int &, const int &)>;

//    AStarContainer() :
//            ids(),
//            nodes(std::make_shared<AStarNodes>(new container_type())),
//            comp([&](const int &a, const int &b) { return AStarNode::LessCost()((*nodes)[a], (*nodes)[b]); }) {};

    explicit AStarPriorityQueue(NodeContainerType *x) :
            ids(),
            nodes(x),
            comp([&](const int &a, const int &b) { return NodeType::LessCost()((*nodes)[a], (*nodes)[b]); }) {};

    bool empty() const { return ids.empty(); }

    NodeContainerType::size_type size() const { return ids.size(); }

    NodeContainerType::const_reference top() const {
        __glibcxx_requires_nonempty();
        return (*nodes)[ids.front()];
    }

    void push(const NodeContainerType::value_type &x) {
//        const auto id = std::hash<Coordinate>()(x.id);
        const auto id = CoordinateHash<kMazeSize>()(x.id);
        ids.push_back(id);
        std::push_heap(ids.begin(), ids.end(), comp);
    }

    void push(NodeContainerType::value_type &&x) {
//        const auto id = std::hash<Coordinate>()(x.id);
        const auto id = CoordinateHash<kMazeSize>()(x.id);
        ids.push_back(id);
        std::push_heap(ids.begin(), ids.end(), comp);
    }

//    template<class... Args>
//    void emplace(Args &&... args) {
//        push(std::forward<Args>(args)...);
//    }

    void emplace(const NodeContainerType::value_type &x) {
//        const auto id = std::hash<Coordinate>()(x.id);
        const auto id = CoordinateHash<kMazeSize>()(x.id);
        ids.push_back(id);
        std::push_heap(ids.begin(), ids.end(), comp);
    }

    void pop() {
        pop_heap(ids.begin(), ids.end(), comp);
        ids.pop_back();
    }

    void poyo() {
        std::for_each(ids.begin(), ids.end(), [&](const int i) {
            std::cout << (*nodes)[i].id << "," << (*nodes)[i].cost_f << " | ";
        });
    }

protected:
    IDContainerType ids;
//    const NodeContainerConstPtr nodes;
    NodeContainerType *nodes;
    const CompareFunctionType comp;
};