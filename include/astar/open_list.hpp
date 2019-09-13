//
// Created by ryunosuke on 2019/08/19.
//

#pragma once

#include <deque>
#include <algorithm>
#include <functional>
#include "../common/maze.hpp"
#include "node.hpp"

/*
 * priority queue(int)
 */
namespace maze_solver {
    namespace astar {
        template<std::size_t kMazeSize>
        class OpenList {
        public:
            using NodeContainer = Nodes<kMazeSize>;
            using IDContainer = std::deque<std::size_t>;
            using CompareFunction = std::function<bool(const std::size_t &, const std::size_t &)>;

            explicit OpenList(NodeContainer &nodes) :
                    ids_(),
                    nodes_(nodes),
                    comp_([&](const std::size_t &a, const std::size_t &b) {
                        return Node::Compare()(nodes_.GetWithIndex(a), nodes_.GetWithIndex(b));
                    }) {};

            void clear() noexcept { this->ids_.clear(); }

            bool empty() const { return ids_.empty(); }

            typename NodeContainer::size_type size() const { return ids_.size(); }

            typename NodeContainer::const_reference top() const {
                __glibcxx_requires_nonempty();
                return nodes_.GetWithIndex(ids_.front());
            }

            void push(const typename NodeContainer::value_type &x) {
                const auto id = Coordinate::Hash<kMazeSize>()(x.GetCoordinate());
                ids_.push_back(id);
                std::push_heap(ids_.begin(), ids_.end(), comp_);
            }

            void push(typename NodeContainer::value_type &&x) {
                const auto id = Coordinate::Hash<kMazeSize>()(x.GetCoordinate());
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