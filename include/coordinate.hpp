//
// Created by ryunosuke on 2019/08/17.
//

#pragma once

#include <cstdint>
#include <cmath>
#include <ostream>

namespace maze_solver {
    struct Coordinate {
        std::int8_t x, y;

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

        static constexpr bool IsOnRange(const Coordinate &c, const std::size_t min, const std::size_t max) noexcept {
            return c.x >= min && c.x < max && c.y >= min && c.y < max;
        }

        bool IsOnRange(const std::size_t min, const std::size_t max) const noexcept {
            return IsOnRange(*this, min, max);
        }

        struct Distance {
            float operator()(const Coordinate &c1, const Coordinate &c2) const noexcept {
                return std::sqrt(static_cast<float>((c1.x - c2.x) * (c1.x - c2.x) + (c1.y - c2.y) * (c1.y - c2.y)));
            }
        };

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