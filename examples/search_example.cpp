#include <iostream>
#include <bitset>
#include <vector>
#include <chrono>
#include "../include/search/search.hpp"
#include "../include/io/maze_reader.hpp"
#include "../include/io/route_writer.hpp"

template<class TMaze>
void print_maze(const TMaze &, const std::size_t &);

int main(const int argc, const char *const *const argv) {
    /*
     * process
     */
    const auto time_ms = []() -> float {
        static const auto s = std::chrono::system_clock::now();
        const auto e = std::chrono::system_clock::now();
        return 0.000001f * static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count());
    };
    const auto timeout = [&time_ms]() {
        if (time_ms() > 150) {
            std::cout << "timeout!" << std::endl;
            return true;
        }
        return false;
    };

    const std::vector<std::string> args(argv, argv + argc);

    const auto maze_file = [&args]() {
        try {
            return args.at(1);
        } catch (std::exception &e) {
            return std::string(DEFAULT_MAZE_FILE);
        }
    }();

    const auto search_route_output_file = [&args]() {
        try {
            return args.at(2);
        } catch (std::exception &e) {
            return std::string(DEFAULT_SEARCH_OUTPUT_FILE);
        }
    }();

    const auto shortest_route_output_file = [&args]() {
        try {
            return args.at(3);
        } catch (std::exception &e) {
            return std::string(DEFAULT_SHORTEST_OUTPUT_FILE);
        }
    }();

    std::cout << "maze data: " << maze_file << std::endl;

    auto maze = maze_solver::MazeReader<MAZE_SIZE>()(maze_file);
    constexpr auto start = maze_solver::Coordinate(START_X, START_Y);
    constexpr auto goal = maze_solver::Coordinate(GOAL_X, GOAL_Y);

//    std::for_each(maze.cbegin(), maze.cend(), [](const auto &c) { std::cout << std::bitset<8>(c.flags) << std::endl; });
    print_maze(maze, MAZE_SIZE);

    auto current = start;

    maze_solver::Route search_route;
    maze_solver::Search<MAZE_SIZE> search;

    const auto transfer = [&search, &maze, &search_route, &timeout](auto &_current, const auto &_goal) -> bool {
        search.ChangeGoal(_current, _goal);
        search.ReachNext(maze[_current]);
        search_route.emplace_back(_current);
        while (_current != _goal) {
            const auto ok = search.CalculateNext();
            _current = search.GetNext();
            search.ReachNext(maze[_current]);
            search_route.emplace_back(_current);
            if (!ok || timeout()) { return false; }
        }

        return true;
    };

    //start -> goal
    if (!transfer(current, goal)) {
        std::cout << "this maze cannot solve." << std::endl;
    }

    /*
     * 最短経路になりうるもののうち，未訪問の区画を探索
     */
    {
        search.ChangeGoal(start, goal);
        auto unvisited = search.FindUnvisitedBlocks();
        auto target = unvisited[0];
        while (!unvisited.empty() && !timeout()) {
            const auto ok = transfer(current, target);
            if (ok) {
                search.ChangeGoal(start, goal);
                unvisited = search.FindUnvisitedBlocks();
                target = unvisited[0];
            } else {
                if (unvisited.size() >= 2) {
                    unvisited.pop_front();
                    target = unvisited[0];
                } else {
                    break;
                }
            }
        }
    }

    /*
     * startへ戻る
     */
    if (!transfer(current, start)) {
        std::cout << "cannot return start????" << std::endl;
    }

    search.ChangeGoal(start, goal);
    const auto shortest = search.GetShortestRoute();

//    std::cout << "time[ms]: " << time_ms() << std::endl;
    std::cout << "count astar: " << search.GetCountSolve() << std::endl;

    {
        auto u = search.FindUnvisitedBlocks();
        std::cout << "unvisited | ";
        std::for_each(u.cbegin(), u.cend(), [](const auto &c) { std::cout << c << ","; });
        std::cout << std::endl;
    }

    std::cout << "search route | ";
    std::for_each(search_route.cbegin(), search_route.cend(), [](const auto &c) { std::cout << c << ","; });
    std::cout << std::endl;
    std::cout << "shortest route | ";
    std::for_each(shortest.cbegin(), shortest.cend(), [](const auto &c) { std::cout << c << ","; });
    std::cout << std::endl;

    maze_solver::RouteWriter()(search_route_output_file, search_route);
    maze_solver::RouteWriter()(shortest_route_output_file, shortest);

    std::cout << "time[ms]: " << time_ms() << std::endl;

    return 0;
}

template<class TMaze>
void print_maze(const TMaze &maze, const std::size_t &maze_size) {
    for (int8_t y = maze_size - 1; y >= 0; --y) {
        for (int8_t x = 0; x < maze_size; ++x) {
            printf("%x ", maze[{x, y}].flags);
        }
        std::cout << std::endl;
    }
}