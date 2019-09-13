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
        template<class TContainer>
        void operator()(const std::string &file_name, const TContainer &route) {
            std::ofstream output(file_name);
            output << "x,y" << std::endl;
            std::for_each(route.cbegin(), route.cend(),
                          [&output](const auto &c) { output << +c.x << "," << +c.y << std::endl; });
            output.close();
        }
    };
}