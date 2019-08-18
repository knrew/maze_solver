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

    Wall() : flags(0b00000000) {}
};

template<class T = Wall, uint32_t kMazeSize = 16>
class Maze : public std::deque<T> {
public:
    enum class Direction : uint8_t {
        kNorth, kEast, kSouth, kWest,
    };

    constexpr Maze() : std::deque<T>(kMazeSize * kMazeSize) {}

    static constexpr bool IsOnRange(const Coordinate &c) {
        return c.x >= 0 && c.x < kMazeSize && c.y >= 0 && c.y < kMazeSize;
    }

    static constexpr bool IsOutOfRange(const Coordinate &c) {
        return !IsOnRange(c);
    }

    constexpr bool HasCheckedWall(const Coordinate &c) const {
        return HasCheckedWall(Get(c.x, c.y));
    }

    constexpr bool HasCheckedWall(const T &w) const {
        return w.is_known_north && w.is_known_east && w.is_known_south && w.is_known_west;
    }

    bool WallExists(const Coordinate &c, const Direction d) const {
        return WallExists(Get(c.x, c.y), d);
    }

    bool WallExists(const T &w, const Direction d) const {
        switch (d) {
            case Direction::kNorth:
                return w.north_exists;
            case Direction::kEast:
                return w.east_exists;
            case Direction::kSouth:
                return w.south_exists;
            case Direction::kWest:
                return w.west_exists;
        }
    }

    typename std::deque<T>::reference operator[](const Coordinate &c) noexcept {
        return Get(c.x, c.y);
    }

    typename std::deque<T>::const_reference operator[](const Coordinate &c) const noexcept {
        return Get(c.x, c.y);
    }

    typename std::deque<T>::reference Get(const int x, const int y) noexcept {
        return std::deque<T>::operator[](x + y * kMazeSize);
    }

    typename std::deque<T>::const_reference Get(const int x, const int y) const noexcept {
        return std::deque<T>::operator[](x + y * kMazeSize);
    }

//    reference at(const Coordinate &c) {
//        return std::deque<Wall>::at(c.x + c.y * TMAZE_SIZE);
//    }

//    const_reference at(const Coordinate &c) const {
//        return std::deque<Wall>::at(c.x + c.y * TMAZE_SIZE);
//    }
};

