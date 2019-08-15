//
// Created by ryunosuke on 2019/08/16.
//

#pragma once

#include <deque>

struct Node {
    float cost_g;
};

class Dijkstra {
public:
    Dijkstra() {
        open_.emplace_back(start_);

        close_.clear();
    }

    Node start_;
    Node goal_;
    std::deque<Node> open_;
    std::deque<Node> close_;
};