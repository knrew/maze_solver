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
#include "include/maze_reader.hpp"
#include "include/a_star_search.hpp"
#include "include/route_writer.hpp"

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

    Maze<Wall, 8> maze;
    Coordinate start_coordinate, goal_coordinate;
    {
        MazeReader<8> reader(maze_data_file_name, true);
        maze = reader.GetMaze();
        start_coordinate = reader.GetStart();
        goal_coordinate = reader.GetGoal();

        int cnt = 0;
        std::for_each(maze.cbegin(), maze.cend(), [&](auto &w) {
            cnt++;
            std::cout << std::bitset<8>(w.flags) << std::endl;
        });
        std::cout << cnt << std::endl;
    }

    std::cout << "start: " << start_coordinate << std::endl;
    std::cout << "goal: " << goal_coordinate << std::endl;

    std::deque<Coordinate> search_route;
    AStarSearch<8> a_star(start_coordinate, goal_coordinate);

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
        const auto next = a_star.GetTargetCoordinate();

        search_route.emplace_back(next);

        auto wall = maze[next];
        wall.is_known_north = wall.is_known_east = wall.is_known_south = wall.is_known_west = true;
        a_star.SetWall(next, wall);
    }

    std::cout << "search route  | ";
    std::for_each(search_route.cbegin(), search_route.cend(), [](const auto &c) { std::cout << c << ","; });
    std::cout << std::endl;

    RouteWriter::Write(search_route_file_name, search_route);

    const auto optimal_route = a_star.CalculateOptimalRoute();

    std::cout << "optimal route | ";
    std::for_each(optimal_route.cbegin(), optimal_route.cend(), [](const auto &c) { std::cout << c << ","; });
    std::cout << std::endl;
    RouteWriter::Write(opt_route_file_name, optimal_route);

    return 0;
}

#endif