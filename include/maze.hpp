//
// Created by ryunosuke on 2019/08/17.
//

#pragma once

#include <cstdint>
#include <deque>
#include "coordinate.hpp"

namespace maze_solver {
    template<class T, const std::size_t kMazeSize>
    class Maze : public std::deque<T> {
    public:
        constexpr Maze() : std::deque<T>(kMazeSize * kMazeSize) {}

        constexpr Maze(const Maze &maze) : std::deque<T>(maze) {}

        static constexpr bool IsOnRange(const Coordinate &c) noexcept {
            return c.IsOnRange(0, kMazeSize);
        }

        typename std::deque<T>::reference operator[](const Coordinate &c) noexcept {
            return std::deque<T>::operator[](Coordinate::Hash<kMazeSize>()(c));
        }

        typename std::deque<T>::const_reference operator[](const Coordinate &c) const noexcept {
            return std::deque<T>::operator[](Coordinate::Hash<kMazeSize>()(c));
        }

        /*
         * TODO:↓delete
         */
        typename std::deque<T>::reference GetWithIndex(const std::size_t index) noexcept {
            return std::deque<T>::operator[](index);
        }

        typename std::deque<T>::const_reference GetWithIndex(const std::size_t index) const noexcept {
            return std::deque<T>::operator[](index);
        }
    };
}