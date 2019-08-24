//
// Created by ryunosuke on 2019/08/19.
//

#pragma once

#include "../coordinate.hpp"

namespace maze_solver {
    namespace a_star {
        class Node {
        public:
            struct Compare {
                bool operator()(const Node &x, const Node &y) const { return x.cost_f_ > y.cost_f_; }
            };

        protected:
            enum class State : uint8_t {
                kNone, kOpen, kClose
            };

        public:
            Node() : coordinate_(), parent_coordinate_(), cost_f_(), state_(State::kNone) {}

            const Coordinate &getCoordinate() const { return coordinate_; }

            void setCoordinate(const Coordinate &c) { coordinate_ = c; }

            const Coordinate &getParentCoordinate() const { return parent_coordinate_; }

            void setParentCoordinate(const Coordinate &c) { parent_coordinate_ = c; }

            float getCost() const { return cost_f_; }

            void setCost(const float cost_f) { cost_f_ = cost_f; }

            bool isOpen() const { return state_ == State::kOpen; }

            bool isClose() const { return state_ == State::kClose; }

            void toOpen() { state_ = State::kOpen; }

            void toClose() { state_ = State::kClose; }

        protected:
            Coordinate coordinate_;
            Coordinate parent_coordinate_;
            float cost_f_;
            State state_;
        };
    }
}
