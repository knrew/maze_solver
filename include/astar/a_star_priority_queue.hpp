//
// Created by ryunosuke on 2019/08/19.
//

#pragma once

#include <queue>
#include <functional>
#include <memory>
#include "../maze.hpp"
#include "a_star_node.hpp"

namespace maze_solver {
    namespace a_star {
        template<std::size_t kMazeSize>
        class IDPriorityQueue {
        public:
            using NodeContainer = Maze<Node, kMazeSize>;
            using IDContainer = std::deque<std::size_t>;
            using CompareFunction = std::function<bool(const std::size_t &, const std::size_t &)>;

            explicit IDPriorityQueue(NodeContainer &nodes) :
                    ids_(),
                    nodes_(nodes),
                    comp_([&](const std::size_t &a, const std::size_t &b) {
                        return Node::Compare()(nodes_[a], nodes_[b]);
                    }) {};

            bool empty() const { return ids_.empty(); }

            typename NodeContainer::size_type size() const { return ids_.size(); }

            typename NodeContainer::const_reference top() const {
                __glibcxx_requires_nonempty();
                return nodes_[ids_.front()];
            }

            void push(const typename NodeContainer::value_type &x) {
                const auto id = Coordinate::Hash<kMazeSize>()(x.getCoordinate());
                ids_.push_back(id);
                std::push_heap(ids_.begin(), ids_.end(), comp_);
            }

            void push(typename NodeContainer::value_type &&x) {
                const auto id = Coordinate::Hash<kMazeSize>()(x.getCoordinate());
                ids_.push_back(id);
                std::push_heap(ids_.begin(), ids_.end(), comp_);
            }

            /*
             * TODO:implement emaplace correctly
             */
//    template<class... Args>
//    void emplace(Args &&... args) {
//        push(std::forward<Args>(args)...);
//    }

            void emplace(const typename NodeContainer::value_type &x) {
                push(x);
            }

            void pop() {
                pop_heap(ids_.begin(), ids_.end(), comp_);
                ids_.pop_back();
            }

        protected:
            IDContainer ids_;
            const NodeContainer &nodes_;
            const CompareFunction comp_;
        };
    }
}