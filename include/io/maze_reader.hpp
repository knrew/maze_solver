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
        Maze <Wall, kMazeSize> operator()(const std::string &file_name) {
            Maze<Wall, kMazeSize> maze;

            std::ifstream ifs(file_name);

            std::vector<std::string> raw_maze;

            {
                std::string line;
                while (getline(ifs, line)) {
                    raw_maze.emplace_back(line);
                }
            }

            std::size_t y = raw_maze.size() - 1;
            for (const auto &e : raw_maze) {
                const auto line = Split(e, ' ');
                for (std::size_t x = 0; x < line.size(); ++x) {
                    if (line[x].empty()) { continue; }
                    const auto b = std::bitset<8>(std::stoi(line[x], nullptr, 16));
                    /*
                     * TODO:????
                     */
                    maze[Coordinate(x, y)].north_exists = b[0];
                    maze[Coordinate(x, y)].east_exists = b[1];
                    maze[Coordinate(x, y)].south_exists = b[2];
                    maze[Coordinate(x, y)].west_exists = b[3];
                    maze[Coordinate(x, y)].is_known_north = b[4];
                    maze[Coordinate(x, y)].is_known_east = b[5];
                    maze[Coordinate(x, y)].is_known_south = b[6];
                    maze[Coordinate(x, y)].is_known_west = b[7];
//                    maze[Coordinate(x, y)].flags = std::stoi(line[x], nullptr, 16);
                }
                --y;
            }

            return std::move(maze);
        }

    private:
        static std::vector<std::string> Split(const std::string &input, const char delimiter) {
            std::istringstream stream(input);
            std::vector<std::string> result;

            std::string field;
            while (getline(stream, field, delimiter)) {
                result.emplace_back(field);
            }

            return std::move(result);
        }
    };
}