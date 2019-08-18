//
// Created by ryunosuke on 2019/08/17.
//

#pragma once

#include <cstdint>

struct Coordinate {
    int x, y;

    constexpr Coordinate() : x(0), y(0) {}

    constexpr Coordinate(const int x, const int y) : x(x), y(y) {}

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
};

std::ostream &operator<<(std::ostream &os, const Coordinate &obj) {
    os << "(" << obj.x << "," << obj.y << ")";
    return os;
}

namespace std {
    template<>
    struct hash<Coordinate> {
        using argument_type = Coordinate;
        using result_type  = std::size_t;

        result_type operator()(argument_type const &c) const noexcept {
            result_type const h1(std::hash<int>{}(c.x));
            result_type const h2(std::hash<int>{}(c.y));
            return h1 ^ (h2 << 1);
        }
    };
}