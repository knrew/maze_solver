//
// Created by ryunosuke on 2019/10/05.
//

#pragma once

#include <cstdint>

namespace maze_solver ::constants {
#if defined(MAZE_SIZE)
    static constexpr std::int32_t kMazeSize = MAZE_SIZE;
#else
    static constexpr std::int32_t kMazeSize = 32;
#endif
}