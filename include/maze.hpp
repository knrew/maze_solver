//
// Created by ryunosuke on 2019/08/17.
//

#pragma once

#include "coordinate.hpp"
#include "wall.hpp"
#include <deque>

template<class T, int kMazeSize>
class Maze : public std::deque<T> {
public:
    explicit Maze() : std::deque<T>(kMazeSize * kMazeSize) {}

    Maze(const Maze &maze) : std::deque<T>(maze) {}

    Maze &operator=(const Maze &maze) {
        if (this != &maze) { std::deque<T>::operator=(maze); }
        return *this;
    }

    constexpr bool IsOnRange(const Coordinate &c) {
        return c.x >= 0 && c.x < kMazeSize && c.y >= 0 && c.y < kMazeSize;
    }

    constexpr bool IsOutOfRange(const Coordinate &c) {
        return !IsOnRange(c);
    }

    typename std::deque<T>::reference operator[](const Coordinate &c) {
//        return std::deque<T>::operator[](std::hash<Coordinate>()(c));
        return std::deque<T>::operator[](CoordinateHash<kMazeSize>()(c));
    }

    typename std::deque<T>::const_reference operator[](const Coordinate &c) const {
//        return std::deque<T>::operator[](std::hash<Coordinate>()(c));
        return std::deque<T>::operator[](CoordinateHash<kMazeSize>()(c));
    }
};

