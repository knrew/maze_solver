#include <iostream>
#include <bitset>
#include <vector>
#include <chrono>
#include "include/astar/solver.hpp"
#include "include/io/maze_reader.hpp"
#include "include/io/route_writer.hpp"

int main(const int argc, const char *const *const argv) {
    maze_solver::Wall w;
    w.flags = 0x9;
    std::cout << std::boolalpha
              << w.north_exists << ","
              << w.east_exists << ","
              << w.south_exists << ","
              << w.west_exists << std::endl;

//    return 0;

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

    const auto optimal_route_output_file = [&args]() {
        try {
            return args.at(3);
        } catch (std::exception &e) {
            return std::string(DEFAULT_SHOTEST_OUTPUT_FILE);
        }
    }();

    std::cout << "maze data: " << maze_file << std::endl;

    const auto maze = maze_solver::MazeReader<MAZE_SIZE>()(maze_file);
    constexpr auto start = maze_solver::Coordinate(START_X, START_Y);
    constexpr auto goal = maze_solver::Coordinate(GOAL_X, GOAL_Y);

//    std::for_each(maze.cbegin(), maze.cend(), [](const auto &c) { std::cout << std::bitset<8>(c.flags) << std::endl; });
    for (int8_t y = MAZE_SIZE - 1; y >= 0; --y) {
        for (int8_t x = 0; x < MAZE_SIZE; ++x) {
            printf("%x ", maze[{x, y}].flags);
        }
        std::cout << std::endl;
    }

    std::cout << "start: " << start << std::endl;
    std::cout << "goal: " << goal << std::endl;

    maze_solver::a_star::Solver<MAZE_SIZE> solver(start, goal);

    auto s = std::chrono::system_clock::now();
//    for (int i = 0; i < 30; ++i) {
    solver.solve(maze);
//    }
    auto e = std::chrono::system_clock::now();
    std::cout << "calculation time[ms]: "
              << 0.000001f * std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count() << std::endl;

    const auto search_route = solver.search_route;
    const auto optimal_route = solver.CalculateOptimalRoute();

    std::cout << "search route  | ";
    std::for_each(search_route.cbegin(), search_route.cend(), [](const auto &c) { std::cout << c << ","; });
    std::cout << std::endl;
    std::cout << "optimal route | ";
    std::for_each(optimal_route.cbegin(), optimal_route.cend(), [](const auto &c) { std::cout << c << ","; });
    std::cout << std::endl;

    maze_solver::RouteWriter()(search_route_output_file, search_route);
    maze_solver::RouteWriter()(optimal_route_output_file, optimal_route);

    return 0;
}