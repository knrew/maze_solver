//
// Created by ryunosuke on 2019/08/17.
//

#pragma once

#include <deque>
#include <fstream>
#include "../maze.hpp"
#include <algorithm>

namespace maze_solver {
    class RouteWriter {
    public:
        static void Write(const auto &file_name, const auto &route) {
            std::ofstream output(file_name);
            output << "x,y" << std::endl;
            std::for_each(route.cbegin(), route.cend(),
                          [&output](const auto &c) { output << +c.x << "," << +c.y << std::endl; });
            output.close();
        }
    };
}