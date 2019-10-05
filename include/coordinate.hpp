//
// Created by ryunosuke on 2019/08/17.
//

#pragma once

#include <cstdint>
#include <cmath>
#include <ostream>
#include "maze_constants.hpp"

namespace maze_solver {
    struct Coordinate {
        std::int8_t x, y;

        constexpr Coordinate() noexcept : x(0), y(0) {}

        template<typename T>
        constexpr Coordinate(const T x, const T y) noexcept : x(x), y(y) {}

        template<typename T>
        constexpr Coordinate(const std::initializer_list<T> &list)  : x(*list.begin()), y(*(list.begin() + 1)) {}

        constexpr Coordinate(const Coordinate &c) = default;

        Coordinate &operator=(const Coordinate &c) = default;

        virtual ~Coordinate() = default;

        constexpr Coordinate(Coordinate &&c) = default;

        constexpr bool operator==(const Coordinate &c) const noexcept {
            return this->x == c.x && this->y == c.y;
        }

        constexpr bool operator!=(const Coordinate &c) const noexcept {
            return !(*this == c);
        }

        struct Hash {
            std::int32_t operator()(Coordinate const &c) const noexcept {
                return c.x + c.y * constants::kMazeSize;
            }
        };
    };
}

std::ostream &operator<<(std::ostream &os, const maze_solver::Coordinate &obj) {
    os << "(" << +obj.x << "," << +obj.y << ")";
    return os;
}