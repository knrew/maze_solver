//
// Created by ryunosuke on 2019/08/17.
//

#pragma once

#include <cstdint>

namespace maze_solver {
    struct Coordinate {
        int8_t x, y;

        constexpr Coordinate() : x(0), y(0) {}

        constexpr Coordinate(const int8_t x, const int8_t y) : x(x), y(y) {}

        constexpr Coordinate(const Coordinate &c) = default;

        constexpr bool operator==(const Coordinate &c) const noexcept {
            return this->x == c.x && this->y == c.y;
        }

        constexpr bool operator!=(const Coordinate &c) const noexcept {
            return !(*this == c);
        }

        constexpr bool operator>(const Coordinate &c) const noexcept {
            return this->x > c.x && this->y > c.y;
        }

        constexpr bool operator>=(const Coordinate &c) const noexcept {
            return this->x >= c.x && this->y >= c.y;
        }

        constexpr bool operator<(const Coordinate &c) const noexcept {
            return !(*this >= c);
        }

        constexpr bool operator<=(const Coordinate &c) const noexcept {
            return !(*this > c);
        }

        template<std::size_t kSize>
        struct Hash {
            std::size_t operator()(Coordinate const &c) const noexcept {
                return c.x + c.y * kSize;
            }
        };
    };
}

std::ostream &operator<<(std::ostream &os, const maze_solver::Coordinate &obj) {
    os << "(" << +obj.x << "," << +obj.y << ")";
    return os;
}