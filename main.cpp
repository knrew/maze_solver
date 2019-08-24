#if 0
#include <iostream>
#include "include/a_star_search.hpp"

int main() {
    AStarSearch search(32, {0, 0}, {7, 7});
    return 0;
}

#else

#include <bitset>
#include <cstdlib>
#include "include/csv/maze_reader.hpp"
#include "include/astar/a_star_search.hpp"
#include "include/csv/route_writer.hpp"

int main(const int argc, const char *const *const argv) {
    const std::vector<std::string> args(argv, argv + argc);

    const auto maze_data_file_name = [&]() {
        try {
            return args.at(1);
        } catch (std::exception &e) {
            return std::string(getenv("HOME")) + "/micromouse/maze8x8.csv";
        }
    }();

    const auto search_route_file_name = [&]() {
        try {
            return args.at(2);
        } catch (std::exception &e) {
            return std::string(getenv("HOME")) + "/micromouse/search_route.csv";
        }
    }();

    const auto opt_route_file_name = [&]() {
        try {
            return args.at(3);
        } catch (std::exception &e) {
            return std::string(getenv("HOME")) + "/micromouse/optimal_route.csv";
        }
    }();

    std::cout << "maze data: " << maze_data_file_name << std::endl;

    maze_solver::Maze<maze_solver::Wall, 8> maze;
    maze_solver::Coordinate start_coordinate, goal_coordinate;
    {
        maze_solver::MazeReader<8> reader(maze_data_file_name, true);
        maze = reader.GetMaze();
        start_coordinate = reader.GetStart();
        goal_coordinate = reader.GetGoal();

//        std::for_each(maze.cbegin(), maze.cend(), [&](auto &w) { std::cout << std::bitset<8>(w.flags) << std::endl; });
    }

    std::cout << "start: " << start_coordinate << std::endl;
    std::cout << "goal: " << goal_coordinate << std::endl;

    std::deque<maze_solver::Coordinate> search_route;
    maze_solver::a_star::Solver<8> a_star(start_coordinate, goal_coordinate);
    while (true) {
        if (a_star.HasFoundAnswer()) {
//            std::cout << "optimal route has found!" << std::endl;
            break;
        }

        if (a_star.HasNoAnswer()) {
            std::cout << "This maze cannot be solved." << std::endl;
            break;
        }

        a_star.CalculateNextNode();
        const auto target = a_star.GetNextNodeCoordinate();

        search_route.emplace_back(target);

        auto wall = maze[target];
        wall.is_known_north = wall.is_known_east = wall.is_known_south = wall.is_known_west = true;
        a_star.SetWall(target, wall);
    }

    const auto optimal_route = a_star.CalculateOptimalRoute();

    std::cout << "search route  | ";
    std::for_each(search_route.cbegin(), search_route.cend(), [](const auto &c) { std::cout << c << ","; });
    std::cout << std::endl;
    std::cout << "optimal route | ";
    std::for_each(optimal_route.cbegin(), optimal_route.cend(), [](const auto &c) { std::cout << c << ","; });
    std::cout << std::endl;

    maze_solver::RouteWriter::Write(search_route_file_name, search_route);
    maze_solver::RouteWriter::Write(opt_route_file_name, optimal_route);

    return 0;
}

#endif