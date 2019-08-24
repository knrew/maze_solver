//
// Created by ryunosuke on 2019/08/17.
//

#pragma once

#include "coordinate.hpp"
#include "wall.hpp"
#include <deque>

namespace maze_solver {
    template<class T, const std::size_t kMazeSize>
    class Maze : public std::deque<T> {
    public:
        Maze() : std::deque<T>(kMazeSize * kMazeSize) {}

        Maze(const Maze &maze) : std::deque<T>(maze) {}

        static constexpr bool IsOnRange(const Coordinate &c) {
            return c.x >= 0 && c.x < kMazeSize && c.y >= 0 && c.y < kMazeSize;
        }

        static constexpr bool IsOutOfRange(const Coordinate &c) {
            return !IsOnRange(c);
        }

        typename std::deque<T>::reference operator[](const Coordinate &c) {
            return std::deque<T>::operator[](Coordinate::Hash<kMazeSize>()(c));
        }

        typename std::deque<T>::const_reference operator[](const Coordinate &c) const {
            return std::deque<T>::operator[](Coordinate::Hash<kMazeSize>()(c));
        }

        typename std::deque<T>::reference operator[](const std::size_t index) {
            return std::deque<T>::operator[](index);
        }

        typename std::deque<T>::const_reference operator[](const std::size_t index) const {
            return std::deque<T>::operator[](index);
        }
    };
}