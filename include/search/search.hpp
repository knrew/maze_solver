//
// Created by ryunosuke on 2019/09/13.
//

#pragma once

#include "../common/maze.hpp"
#include "../common/wall.hpp"
#include "../astar/astar.hpp"

namespace maze_solver {
    template<std::size_t kMazeSize>
    class Search {
    public:
        enum class State : uint8_t {
            kSearchGoal, kSearchShortest, kToStart, kFinished
        };

        Search() : goal_(), maze_(), astar_(), shortest_(), state_(State::kSearchGoal) {}

        /*
         * 次進むべき区画を計算する．
         * 既知ではない壁は存在しないと仮定した場合の目標位置(shortest_[1])について，壁の有無をチェック(shortest_[0]は現在位置になっている)．
         * 壁が存在すればそれまでの壁情報からA*で最短経路を再計算して次の目標位置を求める．
         * shortest_には最短経路のルートが格納されているのでおり，先頭(index :0 )は現在位置なので，nextはshortest_[1]になる.
         * (frontをpopすればshortest_[0]が該当する)
         * ゴールへの経路が存在しなければfalseを，それ以外ならtrueを返す
         */
        bool CalculateNext() {
            const auto d = GetDirection(current_, shortest_[1]);
            if (maze_[current_].WallExists(d)) {
                if (!astar_.solve(maze_, current_, goal_)) { return false; }
                shortest_ = astar_.GetShortestRoute();
            }

            if (shortest_[0] == current_) { shortest_.pop_front(); }

            next_ = std::move(shortest_[0]);

            return true;
        }

        /*
         * 目指す位置(区画)の変更
         */
        bool ChangeGoal(const Coordinate &current, const Coordinate &goal) {
            current_ = current;
            goal_ = goal;
            next_ = current;
            if (!astar_.solve(maze_, current_, goal_)) { return false; }
            shortest_ = astar_.GetShortestRoute();

            return true;
        }

        /*
         * CalculateNextによって算出した次の区画に到達したら呼び出す．そのときの壁情報を引数に渡す
         */
        void ReachNext(const Wall &w) {
            current_ = next_;
            SetWall(current_, w);
        }

        /*
         * 最短経路上にあって，未訪問の区画をリストアップ
         */
        auto FindUnvisitedBlocks() {
            std::deque<Coordinate> ret;
            for (const auto &c : shortest_) {
                if (!maze_[c].IsKnownAllDirection()) { ret.emplace_back(c); }
            }

            return std::move(ret);
        }

        void SetWall(const Coordinate &c, const Wall &w) { maze_[c] = w; }

        const Coordinate &GetNext() const { return next_; }

        Route GetShortestRoute() const { return astar_.GetShortestRoute(); }

        /*
         * デバッグ用．使う必要はないはず
         */
        auto GetCurrent() const { return current_; }

    protected:
        /*
         * 現在位置と次の位置(目標位置)から進む方向を計算
         */
        static Wall::Direction GetDirection(const Coordinate &current, const Coordinate &next) {
            if (current.x == next.x && current.y + 1 == next.y) { return Wall::Direction::kNorth; }
            if (current.x + 1 == next.x && current.y == next.y) { return Wall::Direction::kEast; }
            if (current.x == next.x && current.y - 1 == next.y) { return Wall::Direction::kSouth; }
            if (current.x - 1 == next.x && current.y == next.y) { return Wall::Direction::kWest; }

            std::cout << "????" << std::endl;
            return Wall::Direction::kNorth;
        }

        Coordinate goal_;
        Coordinate current_;
        Coordinate next_;
        Maze<Wall, kMazeSize> maze_;
        astar::AStar<kMazeSize> astar_;
        Route shortest_;
        State state_;
    };
}