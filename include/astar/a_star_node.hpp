//
// Created by ryunosuke on 2019/08/19.
//

#pragma once

#include "../coordinate.hpp"

class AStarNode {
public:
    struct Compare {
        bool operator()(const AStarNode &x, const AStarNode &y) const { return x.cost_f_ > y.cost_f_; }
    };

    enum class State : uint8_t {
        kNone, kOpen, kClose
    };

    AStarNode() : coordinate_(), parent_coordinate_(), cost_f_(), state_(State::kNone) {}

    const Coordinate &getCoordinate() const { return coordinate_; }

    const Coordinate &getParentCoordinate() const { return parent_coordinate_; }

    float getCost() const { return cost_f_; }

    State getState() const { return state_; }

    bool isOpen() const { return state_ == State::kOpen; }

    bool isClose() const { return state_ == State::kClose; }

    bool isNeitherOpenNorClose() const { return state_ == State::kNone; }

    void setCoordinate(const Coordinate &c) { coordinate_ = c; }

    void setState(const State s) { state_ = s; }

    void
    setAll(const Coordinate &coordinate, const Coordinate &parent_coordinate, const float cost_f, const State &state) {
        coordinate_ = coordinate;
        parent_coordinate_ = parent_coordinate;
        cost_f = cost_f;
        state_ = state;
    }

protected:
    Coordinate coordinate_;
    Coordinate parent_coordinate_;
    float cost_f_;
    State state_;
};