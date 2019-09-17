//
// Created by ryunosuke on 2019/08/19.
//

#pragma once

#include <cstdint>
#include "../common/coordinate.hpp"
#include "../common/maze.hpp"

namespace maze_solver {
    namespace astar {
        class Node {
        public:
            struct Compare {
                bool operator()(const Node &x, const Node &y) const noexcept { return x.cost_f_ > y.cost_f_; }
            };

        protected:
            enum class State : uint8_t {
                kNone, kOpen, kClose
            };

        public:
            Node() : coordinate_(), parent_coordinate_(), cost_f_(), state_(State::kNone) {}

            const Coordinate &GetCoordinate() const noexcept { return coordinate_; }

            void SetCoordinate(const Coordinate &c) noexcept { coordinate_ = c; }

            const Coordinate &GetParentCoordinate() const noexcept { return parent_coordinate_; }

            void SetParentCoordinate(const Coordinate &c) noexcept { parent_coordinate_ = c; }

            float GetCostF() const noexcept { return cost_f_; }

            void SetCostF(const float cost_f) noexcept { cost_f_ = cost_f; }

            bool IsOpen() const noexcept { return state_ == State::kOpen; }

            bool IsClose() const noexcept { return state_ == State::kClose; }

            void ToOpen() noexcept { state_ = State::kOpen; }

            void ToClose() noexcept { state_ = State::kClose; }

        protected:
            Coordinate coordinate_;
            Coordinate parent_coordinate_;
            float cost_f_;
            State state_;
        };

        template<std::size_t kMazeSize>
        class Nodes : public Maze<Node, kMazeSize> {
        public:
            constexpr Nodes() : Maze<Node, kMazeSize>() {
                for (std::size_t i = 0; i < kMazeSize; i++) {
                    for (std::size_t j = 0; j < kMazeSize; j++) {
                        const Coordinate c(i, j);
                        Maze<Node, kMazeSize>::operator[](c).SetCoordinate(c);
                    }
                }
            }
        };


    }
}


