#include <bitset>
#include "include/maze_reader.hpp"
#include "include/a_star.hpp"
#include "include/route_writer.hpp"

int main(const int argc, const char *const *const argv) {
    const std::string file_name = "/home/ryunosuke/micromouse/maze8x8.csv";
    const std::string search_route_file_name = "/home/ryunosuke/micromouse/search_route.csv";
    const std::string opt_route_file_name = "/home/ryunosuke/micromouse/optimal_route.csv";

    Maze<> maze;
    Coordinate start_coordinate, goal_coordinate;
    {
        MazeReader reader(file_name, true);
        maze = reader.GetMaze();
        start_coordinate = reader.GetStart();
        goal_coordinate = reader.GetGoal();
        std::cout << "start: " << start_coordinate << std::endl;
        std::cout << "goal: " << goal_coordinate << std::endl;
    }

    std::deque<Coordinate> search_route;
    AStarSearch a_star(start_coordinate, goal_coordinate);
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

//    std::cout << "search route  | ";
//    std::for_each(search_route.cbegin(), search_route.cend(), [](const auto &c) { std::cout << c << ","; });
//    std::cout << std::endl;

    RouteWriter::Write(search_route_file_name, search_route);

    const auto optimal_route = a_star.CalculateOptimalRoute();

    std::cout << "optimal route | ";
    std::for_each(optimal_route.cbegin(), optimal_route.cend(), [](const auto &c) { std::cout << c << ","; });
    std::cout << std::endl;
    RouteWriter::Write(opt_route_file_name, optimal_route);

    return 0;
}