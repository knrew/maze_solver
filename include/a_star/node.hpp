//
// Created by ryunosuke on 2019/10/05.
//

#pragma once

#include <cstdint>
#include "../wall.hpp"

namespace maze_solver::a_star {
    //
    // Node : A*アルゴリズムで最短経路を算出する際の各ノード(マス)が持つ情報
    // parent : どこの(どの方向の)ノードから来たかを表す
    // child : は次どの方向のノードへ行くかを表す
    // state: A*ではOpenリストとCloseリストを用いるが，これをノードが持つ情報と見なし，stateとする
    // cost_f : A*で使われているcost
    //
    struct Node {
        using Direction = Wall::Direction;

        enum class State : int8_t {
            kNone, kOpen, kClose
        };

        Direction parent;
        Direction child;
        State state;
        float cost_f;

        Node() : parent(Direction::kNorth), child(Direction::kNorth), state(State::kNone), cost_f(0.f) {}
    };


}