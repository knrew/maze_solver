//
// Created by ryunosuke on 2019/09/13.
//

#pragma once

namespace maze_solver {
    namespace math {
        template<typename T>
        static constexpr bool IsOnRange(const T &val, const T &min, const T &max) noexcept {
            return min <= val && val <= max;
        }
    }
}