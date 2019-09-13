//
// Created by ryunosuke on 2019/08/17.
//

#pragma once

#include <deque>
#include <fstream>
#include <algorithm>
#include "../common/maze.hpp"

namespace maze_solver {
    class RouteWriter {
    public:
        void operator()(const std::string &file_name, const Route &r) {
            std::ofstream output(file_name);
            output << "x,y" << std::endl;
            std::for_each(r.cbegin(), r.cend(),
                          [&output](const auto &c) { output << +c.x << "," << +c.y << std::endl; });
            output.close();
        }
    };
}