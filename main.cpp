#include <iostream>
#include <bitset>
#include <vector>
#include "include/astar/solver.hpp"
#include "include/io/maze_reader.hpp"
#include "include/io/route_writer.hpp"

int main(const int argc, const char *const *const argv) {
    const std::vector<std::string> args(argv, argv + argc);

    const auto maze_file = [&args]() {
        try {
            return args.at(1);
        } catch (std::exception &e) {
            return std::string(getenv("HOME")) + "/micromouse/maze_data/maze.txt";
        }
    }();

    const auto search_route_output_file = [&args]() {
        try {
            return args.at(2);
        } catch (std::exception &e) {
            return std::string(getenv("HOME")) + "/micromouse/maze_solver/search_route.csv";
        }
    }();

    const auto optimal_route_output_file = [&args]() {
        try {
            return args.at(3);
        } catch (std::exception &e) {
            return std::string(getenv("HOME")) + "/micromouse/maze_solver/optimal_route.csv";
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
    solver.solve(maze);
    solver.solve(maze);
    solver.solve(maze);

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