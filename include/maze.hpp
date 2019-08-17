//
// Created by ryunosuke on 2019/08/17.
//

#pragma once

#include "coordinate.hpp"
#include <deque>

union Wall {
    uint8_t flags;
    struct {
        bool north_exists:1;
        bool east_exists:1;
        bool west_exists:1;
        bool south_exists:1;
        bool is_known_north:1;
        bool is_known_east:1;
        bool is_known_south:1;
        bool is_known_west:1;
    };
};

template<uint32_t TMAZE_SIZE = 32>
class Maze : public std::deque<Wall> {
public:
    constexpr Maze() : std::deque<Wall>(TMAZE_SIZE * TMAZE_SIZE) {}

    reference operator[](const Coordinate &c) noexcept {
        return get(c.x, c.y);
    }

    const_reference operator[](const Coordinate &c) const noexcept {
        return get(c.x, c.y);
    }

    reference get(const int x, const int y) noexcept {
        return std::deque<Wall>::operator[](x + y * TMAZE_SIZE);
    }

    const_reference get(const int x, const int y) const noexcept {
        return std::deque<Wall>::operator[](x + y * TMAZE_SIZE);
    }

//    reference at(const Coordinate &c) {
//        return std::deque<Wall>::at(c.x + c.y * TMAZE_SIZE);
//    }

//    const_reference at(const Coordinate &c) const {
//        return std::deque<Wall>::at(c.x + c.y * TMAZE_SIZE);
//    }
};

