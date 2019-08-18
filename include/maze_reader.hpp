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
#include "maze.hpp"

class MazeReader {
private:
    enum Index {
        X = 0,
        Y = 1,
        NORTH = 2,
        EAST = 3,
        SOUTH = 4,
        WEST = 5,
        START_X = 6,
        START_Y = 7,
        GOAL_X = 8,
        GOAL_Y = 9
    };

public:
    explicit MazeReader(const auto &file_name, const auto header = true) :
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
            coordinate.x = std::stoi(factor[Index::X]);
            coordinate.y = std::stoi(factor[Index::Y]);
            wall.north_exists = std::stoi(factor[Index::NORTH]);
            wall.east_exists = std::stoi(factor[Index::EAST]);
            wall.south_exists = std::stoi(factor[Index::SOUTH]);
            wall.west_exists = std::stoi(factor[Index::WEST]);
            start_.x = std::stoi(factor[Index::START_X]);
            start_.y = std::stoi(factor[Index::START_Y]);
            goal_.x = std::stoi(factor[Index::GOAL_X]);
            goal_.y = std::stoi(factor[Index::GOAL_Y]);

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
    Maze<> maze_;
    Coordinate start_;
    Coordinate goal_;
};