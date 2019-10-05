//
// Created by ryunosuke on 2019/10/05.
//

#pragma once

#include <deque>
#include "coordinate.hpp"
#include "maze_constants.hpp"

namespace maze_solver {
    template<class TNode, typename TContainer = std::deque<TNode>>
    class Maze {
    public:
        using iterator = typename TContainer::iterator;
        using const_iterator = typename TContainer::const_iterator;
        using reference = typename TContainer::reference;
        using const_reference = typename TContainer::const_reference;
//        using size_type = typename TSequence::size_type;
        using size_type = std::int32_t;

        Maze() : container_(constants::kMazeSize * constants::kMazeSize) {}

        Maze(const Maze &maze) : container_(maze.container_) {}

        virtual ~Maze() = default;

        Maze(Maze &&maze) noexcept = default;

        [[nodiscard]] size_type size() const { return container_.size(); }

        reference operator[](const Coordinate &c) {
            return container_[Coordinate::Hash()(c)];
        }

        const_reference operator[](const Coordinate &c) const {
            return container_[Coordinate::Hash()(c)];
        }

        iterator begin() { return container_.begin(); }

        iterator end() { return container_.end(); }

        const_iterator begin() const { return container_.begin(); }

        const_iterator end() const { return container_.end(); }

        const_iterator cbegin() const { return container_.cbegin(); }

        const_iterator cend() const { return container_.cend(); }

    protected:
        TContainer container_;
    };
}