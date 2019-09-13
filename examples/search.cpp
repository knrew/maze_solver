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

    const auto maze = maze_solver::MazeReader<MAZE_SIZE>()(maze_file);
    constexpr auto start = maze_solver::Coordinate(START_X, START_Y);
    constexpr auto goal = maze_solver::Coordinate(GOAL_X, GOAL_Y);

//    std::for_each(maze.cbegin(), maze.cend(), [](const auto &c) { std::cout << std::bitset<8>(c.flags) << std::endl; });
    print_maze(maze, MAZE_SIZE);

    maze_solver::Route search_route;

    maze_solver::Search<MAZE_SIZE> search(start, goal);

    auto current = start;
    search.maze_[current] = maze[current];
    search_route.emplace_back(current);

    while (current != goal) {
        search.astar.solve(search.maze_, current);
        current = search.astar.GetShortestRoute()[1];
        search.maze_[current] = maze[current];
        search_route.emplace_back(current);
    }

//    const auto search_route = search.astar.GetSearchRoute();
    search.astar.solve(maze, start);
    const auto shortest = search.astar.GetShortestRoute();

    std::cout << "search route | ";
    std::for_each(search_route.cbegin(), search_route.cend(), [](const auto &c) { std::cout << c << ","; });
    std::cout << std::endl;
    std::cout << "shortest route | ";
    std::for_each(shortest.cbegin(), shortest.cend(), [](const auto &c) { std::cout << c << ","; });
    std::cout << std::endl;

    maze_solver::RouteWriter()(search_route_output_file, search_route);
    maze_solver::RouteWriter()(shortest_route_output_file, shortest);

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