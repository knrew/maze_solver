//
// Created by ryunosuke on 2019/08/17.
//

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include "../maze.hpp"

namespace maze_solver {
    template<std::size_t kMazeSize>
    class MazeReader {
    private:
        enum Index {
            kX = 0,
            kY = 1,
            kNorth = 2,
            kEast = 3,
            kSouth = 4,
            kWest = 5,
            kStartX = 6,
            kStartY = 7,
            kGoalX = 8,
            kGoalY = 9
        };

    public:
        explicit MazeReader(const std::string &file_name, const std::size_t maze_size, const bool header = true) :
                file_name_(file_name), header_(header), maze_(), start_(), goal_() {
            Read();
        }

        auto GetMaze() const { return maze_; }

        auto GetStart() const { return start_; }

        auto GetGoal() const { return goal_; }

    private:
        void Read() {
            Coordinate coordinate;
            Wall wall;

            std::string line;
            std::ifstream ifs(file_name_);

            if (header_) {
                getline(ifs, line);
            }

            while (getline(ifs, line)) {
                const auto factor = Split(line, ',');
                coordinate.x = std::stoi(factor[Index::kX]);
                coordinate.y = std::stoi(factor[Index::kY]);
                wall.north_exists = std::stoi(factor[Index::kNorth]);
                wall.east_exists = std::stoi(factor[Index::kEast]);
                wall.south_exists = std::stoi(factor[Index::kSouth]);
                wall.west_exists = std::stoi(factor[Index::kWest]);
                start_.x = std::stoi(factor[Index::kStartX]);
                start_.y = std::stoi(factor[Index::kStartY]);
                goal_.x = std::stoi(factor[Index::kGoalX]);
                goal_.y = std::stoi(factor[Index::kGoalY]);

                maze_[coordinate] = wall;
            }
        }

        static std::deque<std::string> Split(const auto &input, const auto delimiter) {
            std::istringstream stream(input);
            std::string field;
            std::deque<std::string> result;
            while (getline(stream, field, delimiter)) {
                result.push_back(field);
            }
            return result;
        }

        const std::string file_name_;
        const bool header_;
        Maze<Wall, kMazeSize> maze_;
        Coordinate start_;
        Coordinate goal_;
    };
}