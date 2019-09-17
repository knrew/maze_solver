//
// Created by ryunosuke on 2019/09/13.
//

#pragma once

#include "../common/maze.hpp"
#include "../common/wall.hpp"
#include "../astar/astar.hpp"

namespace maze_solver {

    /*
     * このアルゴリズムでは未探索の壁はないものとしてゴールまでの最短経路を計算し，それにしたがって進む．
     * ルート上に壁があれば，それまでの壁情報をもとに逐次最短ルートを計算しなおして進む．
     * 一旦ゴールしても，その時点での最短経路上に未探索の区画がある限りそこを探索しに行く．
     */
    template<std::size_t kMazeSize>
    class Search {
    protected:
        enum class State : uint8_t {
            kSearchingGoal,
            kSearchingShortest,
            kToStart,
            kFinished,
            kNoAnswer,
        };

    public:
        Search(const Coordinate &start, const Coordinate &goal) :
                start_(start), goal_(goal),
                current_(start), next_(start), target_(start),
                maze_(),
                astar_(),
                shortest_(),
                unvisited_blocks_(),
                state_(State::kSearchingGoal) {
            astar_.solve(maze_, start_, goal_);
            shortest_ = astar_.GetShortestRoute();
        }

        bool update() {
            if (HasFinished()) { return true; }

            /*
             * 目標位置に到達していない場合，更新の必要はない
             */
            if (current_ != next_) { return true; }

            switch (state_) {
                /*
                 * ゴールにむかって探索
                 */
                case State::kSearchingGoal:
                    target_ = goal_;
                    if (current_ == goal_) {
                        state_ = State::kSearchingShortest;
                        break;
                    }
                    /*
                     * ゴールまでの経路がない=迷路が解けない
                     */
                    if (!CalculateNext()) {
                        state_ = State::kNoAnswer;
                        return false;
                    }
                    break;


                /*
                 * 最短経路上の未探索区画を探索
                 */
                case State::kSearchingShortest:

                    /*
                     * targetに到達したら次のtargetを決める．
                     * start-goal間の最短経路を再計算し，現在位置currentから最も近い未探索区画を新しいtargetとする．
                     */
                    if (current_ == target_) {
                        if (astar_.solve(maze_, start_, goal_)) {
                            state_ = State::kNoAnswer;
                            return false;
                        }
                        shortest_ = astar_.GetShortestRoute();
                        unvisited_blocks_ = FindUnvisitedBlocks();

                        if (unvisited_blocks_.empty()) {
                            state_ = State::kToStart;
                            return true;
                        }

                        target_ = unvisited_blocks_[0];
                        unvisited_blocks_.pop_front();

                        /*
                         * TODO:targetの四方が壁で囲まれている場合(物理的に進入不可能である場合)，探索が終わらない可能性があるので，その部分のエラー処理を書く．
                         */
                        if (!astar_.solve(maze_, current_, target_)) {} else {}
                        shortest_ = astar_.GetShortestRoute();

                    } else {
                        if (!CalculateNext()) {
                            state_ = State::kToStart;
                            return false;
                        }
                    }

                    break;

                /*
                 * 最短経路が見つかればスタートまで戻る．
                 * stateがFinishになったときにshortestにstart-goal間の最短経路が格納されるようにする
                 * (currentやtargetがそれぞれstartやgoalでない場合にCaluculateNextを呼び出していると，shortestに格納されるのは最短経路ではない)
                 */
                case State::kToStart:
                    target_ = start_;
                    if (current_ == start_) {
                        astar_.solve(maze_, start_, goal_);
                        shortest_ = astar_.GetShortestRoute();
                        next_ = target_ = start_;
                        state_ = State::kFinished;
                        break;
                    }
                    if (!CalculateNext()) {
                        state_ = State::kNoAnswer;
                        return false;
                    }
                    break;
                case State::kFinished:
                    break;
                case State::kNoAnswer:
                    return false;
                    break;
            }

            return true;
        }

        const Coordinate &GetNext() const { return next_; }

        /*
         * CalculateNextによって算出した次の区画に到達したら呼び出す．そのときの壁情報を引数に渡す
         */
        void ReachNext(const Wall &w) {
            current_ = next_;
            SetWall(current_, w);
        }

        void SetWall(const Coordinate &c, const Wall &w) {
            maze_[c] = w;
            maze_[c].SetKnownAll(true);
        }

        const Route &GetShortestRoute() const {
            return shortest_;
        }

        bool HasFoundRoute() const {
            return state_ == State::kSearchingShortest || state_ == State::kToStart || state_ == State::kFinished;
        }

        bool HasFoundShortestRoute() const { return state_ == State::kToStart || state_ == State::kFinished; }

        bool HasFinished() const { return state_ == State::kFinished; }

        bool HasNoAnswer() const { return state_ == State::kNoAnswer; }
    protected:
        /*
         * 次進むべき区画を計算する．
         * 既知ではない壁は存在しないと仮定した場合の目標位置(shortest_[1])について，壁の有無をチェック(shortest_[0]は現在位置になっている)．
         * 壁が存在すればそれまでの壁情報からA*で最短経路を再計算して次の目標位置を求める．
         * shortest_には最短経路のルートが格納されているのでおり，先頭(index :0 )は現在位置なので，nextはshortest_[1]になる.
         * (frontをpopすればshortest_[0]が該当する)
         * ゴールへの経路が存在しなければfalseを，それ以外ならtrueを返す
         */
        bool CalculateNext() {
            /*
             * TODO:shortest範囲外参照のエラー処理
             */
            const auto d = GetDirection(current_, shortest_[1]);
            if (maze_[current_].WallExists(d)) {
                if (!astar_.solve(maze_, current_, target_)) { return false; }
                shortest_ = astar_.GetShortestRoute();
            }

            if (shortest_[0] == current_) { shortest_.pop_front(); }

            next_ = shortest_[0];

            return true;
        }

        /*
         * 最短経路上にあって，未訪問の区画をリストアップ
         */
        auto FindUnvisitedBlocks() const {
            static std::deque<Coordinate> ret;
            ret.clear();
            for (const auto &c : shortest_) {
                if (!maze_[c].IsKnownAllDirection()) { ret.emplace_front(c); }
            }

            return std::move(ret);
        }

        /*
         * 現在位置と次の位置(目標位置)から進む方向を計算
         */
        static Wall::Direction GetDirection(const Coordinate &current, const Coordinate &next) {
            if (current.x == next.x && current.y + 1 == next.y) { return Wall::Direction::kNorth; }
            if (current.x + 1 == next.x && current.y == next.y) { return Wall::Direction::kEast; }
            if (current.x == next.x && current.y - 1 == next.y) { return Wall::Direction::kSouth; }
            if (current.x - 1 == next.x && current.y == next.y) { return Wall::Direction::kWest; }
            return Wall::Direction::kNorth;
        }

        /*
         * goal : 迷路のゴール
         * target : 最短経路上の未探索区画を見にくことを繰り返すこのアルゴリズムにおいて近傍の未探索区画
         * next : 次のステップで向かう区画.currentと隣接している
         */
        const Coordinate start_, goal_;
        Coordinate current_, next_, target_;
        Maze<Wall, kMazeSize> maze_;
        astar::AStar<kMazeSize> astar_;
        Route shortest_;
        std::deque<Coordinate> unvisited_blocks_;
        State state_;
    };
}