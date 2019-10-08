//
// Created by ryunosuke on 2019/08/19.
//

#pragma once

//#include <deque>
//#include <algorithm>
#include <cstdint>
#include <functional>
#include <optional>
#include "node.hpp"
#include "../maze.hpp"

namespace maze_solver ::a_star {
    template<class TNodes = Maze <Node>, typename TIDContainer = std::deque<std::int32_t>>
    class OpenList {
    public:
        using Compare = std::function<bool(const std::int32_t &, const std::int32_t &)>;

        OpenList() : ids_(), nodes_(nullptr), compare_(nullptr) {}

        void bindNodesPtr(TNodes *const nodes) {
            nodes_ = nodes;
        }

        void bindCompareFunction(Compare &&compare) {
            *compare_ = std::move(compare);
        }

        void clear() noexcept { this->ids_.clear(); }

        [[nodiscard]] bool empty() const { return ids_.empty(); }

        typename TNodes::size_type size() const { return ids_.size(); }

        //TODO:nodes_がnullptrのときの処理
        typename TNodes::const_reference top() const {
            return (*nodes_)[Coordinate::DecodeHash()(ids_.front())];
        }

        void push(const Coordinate &c) {
            const auto id = Coordinate::Hash()(c);
            ids_.push_back(id);
            std::push_heap(ids_.begin(), ids_.end(), compare_);
        }

        template <typename T>
        void emplace(const T &x) {
            push(x);
        }

        void pop() {
            pop_heap(ids_.begin(), ids_.end(), compare_);
            ids_.pop_back();
        }

    protected:
        TIDContainer ids_;
        const TNodes *nodes_;
        Compare *compare_;
    };
}
