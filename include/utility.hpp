//
// Created by ryunosuke on 2019/10/05.
//

#pragma once

#include <type_traits>
#include "maze_constants.hpp"

namespace maze_solver::utility {
    template<typename T>
    static constexpr bool IsOnRange(const T &val) {
        constexpr bool is_integer =
                std::is_same_v<T, int>
                || std::is_same_v<T, uint8_t> || std::is_same_v<T, int8_t>
                || std::is_same_v<T, uint16_t> || std::is_same_v<T, int16_t>
                || std::is_same_v<T, uint32_t> || std::is_same_v<T, int32_t>;
        constexpr bool is_float = std::is_same_v<T, float> || std::is_same_v<T, double>;
        if constexpr (is_integer || is_float) {
            return T(0) <= val && val < constants::kMazeSize;
        } else {
            return decltype(val.x)(0) <= val.x && val.x < constants::kMazeSize
                   && decltype(val.y)(0) <= val.y && val.y < constants::kMazeSize;
        }
    }
}
