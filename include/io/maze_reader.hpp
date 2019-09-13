//
// Created by ryunosuke on 2019/08/17.
//

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <utility>
#include "../common/maze.hpp"
#include "../common/wall.hpp"

namespace maze_solver {
    template<std::size_t kMazeSize>
    class MazeReader {
    public:
        explicit MazeReader(std::string file_name) : file_name_(std::move(file_name)) {
            start_.x = start_.y = 0;
            goal_.x = goal_.y = 7;
        }

        void Read() {
            std::ifstream ifs(file_name_);
            std::string line;

            std::size_t y = kMazeSize - 1;
            while (getline(ifs, line)) {
                const auto walls = Split(line, ' ');
                for (std::size_t x = 0; x < walls.size(); ++x) {
                    if (walls[x] == "") { continue; }
                    maze_[Coordinate(x, y)].flags = std::stoi(walls[x], nullptr, 16);
                }
                --y;
            }
        }

        auto GetMaze() const { return maze_; }

        auto GetStart() const { return start_; }

        auto GetGoal() const { return goal_; }

        static std::vector<std::string> Split(const std::string &input, const char delimiter) {
            std::istringstream stream(input);
            std::string field;
            std::vector<std::string> result;
            while (getline(stream, field, delimiter)) {
                result.emplace_back(field);
            }
            return result;
        }

        const std::string file_name_;
        Maze <Wall, kMazeSize> maze_;
        Coordinate start_;
        Coordinate goal_;
    };
}