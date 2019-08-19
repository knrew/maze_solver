//
// Created by ryunosuke on 2019/08/19.
//

#pragma once

#include <cstdint>

union Wall {
    enum class Direction : uint8_t {
        kNorth, kEast, kSouth, kWest,
    };

    uint8_t flags;
    struct {
        bool north_exists:1;
        bool east_exists:1;
        bool west_exists:1;
        bool south_exists:1;
        bool is_known_north:1;
        bool is_known_east:1;
        bool is_known_south:1;
        bool is_known_west:1;
    };

    Wall() : flags(0b00000000) {}

    bool HasCheckedWall() const {
        return is_known_north && is_known_east && is_known_south && is_known_west;
    }

    bool WallExists(const Direction d) const {
        switch (d) {
            case Direction::kNorth:
                return north_exists;
            case Direction::kEast:
                return east_exists;
            case Direction::kSouth:
                return south_exists;
            case Direction::kWest:
                return west_exists;
        }
    }
};