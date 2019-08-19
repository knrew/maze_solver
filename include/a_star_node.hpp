//
// Created by ryunosuke on 2019/08/19.
//

#pragma once

#include "coordinate.hpp"

struct AStarNode {
    enum class State : uint8_t {
        kNone, kOpen, kClose
    };

    Coordinate id;
    Coordinate parent_id;
    float cost_f;
    State state;

    AStarNode() : id(), parent_id(), cost_f(), state(State::kNone) {}

    const Coordinate &getCoordinate() const { return id; }

    void setAll(const Coordinate &id, const Coordinate &parent_id, const float cost_f, const State &state) {
        this->id = id;
        this->parent_id = parent_id;
        this->cost_f = cost_f;
        this->state = state;
    }

    struct LessCost {
        bool operator()(const AStarNode &x, const AStarNode &y) const { return x.cost_f > y.cost_f; }
    };
};