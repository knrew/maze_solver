//
// Created by ryunosuke on 2019/08/17.
//

#pragma once

#include <cstdint>
#include <cmath>
#include <ostream>
#include "maze_constants.hpp"

namespace maze_solver {
    //
    // 座標情報を格納する構造体
    //
    struct Coordinate {
        std::int8_t x, y;

        constexpr Coordinate() noexcept : x(0), y(0) {}

        template<typename T>
        constexpr
        Coordinate(const T x, const T y) noexcept : x(static_cast<std::int8_t>(x)), y(static_cast<std::int8_t>(y)) {}

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

        struct DecodeHash {
            const Coordinate &operator()(const std::int32_t hash) {
                static Coordinate c;
                c.x = hash - constants::kMazeSize * static_cast<int32_t>(hash / constants::kMazeSize);
                c.y = hash / constants::kMazeSize;
                return c;
            }
        };

    };
}

std::ostream &operator<<(std::ostream &os, const maze_solver::Coordinate &obj) {
    os << "(" << +obj.x << "," << +obj.y << ")";
    return os;
}