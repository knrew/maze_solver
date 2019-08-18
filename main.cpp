#include <bitset>
#include "include/maze_reader.hpp"
#include "include/a_star.hpp"
#include "include/route_writer.hpp"

int main() {
//    std::string file_name = "/home/ryunosuke/micromouse/maze5x5.csv";
    std::string file_name = "/home/ryunosuke/micromouse/maze8x8.csv";
    
    Maze<> maze;
    Coordinate start, goal;
    {
        MazeReader reader(file_name, true);
        maze = reader.getMaze();
        start = reader.getStart();
        goal = reader.getGoal();

        std::cout << "s : " << (int) reader.getStart().x << ", " << (int) reader.getStart().y << std::endl;
        std::cout << "g : " << (int) reader.getGoal().x << ", " << (int) reader.getGoal().y << std::endl;
        for (int8_t i = 0; i < 5; i++) {
            for (int8_t j = 0; j < 5; j++) {
                const Coordinate c = {i, j};
                const auto w = maze[c];
//                std::cout << std::bitset<8>(w.flags) << std::endl;
            }
        }
    }

    std::deque<Coordinate> route;
    AStarSearch search(start, goal);
    while (1) {
        if (search.HasFoundAnswer()) {
            std::cout << "found!" << std::endl;
            break;
        }
        if (search.HasNoAnswer()) {
            std::cout << "No Answer." << std::endl;
            break;
        }

        search.CalculateNextTargetCoordinate();
        const auto next = search.getTargetCoordinate();

        route.emplace_back(next);

        auto wall = maze[next];
        wall.is_known_north = wall.is_known_east = wall.is_known_south = wall.is_known_west = true;
        search.setWall(next, wall);
    }

    std::for_each(route.cbegin(), route.cend(), [](auto &c) { std::cout << "(" << c.x << ", " << c.y << "),"; });
    std::cout << std::endl;

    RouteWriter::write("/home/ryunosuke/micromouse/search_route.csv", route);

    std::deque<Coordinate> opt_route;
    opt_route.clear();
    std::cout << "opt" << std::endl;
    Node n = search.getGoal();
    while (!(n.parent_coordinate == start)) {
        opt_route.emplace_back(n.coordinate);
        n = *search.searchNodeWithParents(n.parent_coordinate);
    }
    opt_route.emplace_back(n.coordinate);
    opt_route.emplace_back(start);

    std::reverse(opt_route.begin(), opt_route.end());
    std::for_each(opt_route.cbegin(), opt_route.cend(), [](auto &c) { std::cout << "(" << c.x << ", " << c.y << "),"; });
    std::cout << std::endl;
    RouteWriter::write("/home/ryunosuke/micromouse/opt_route.csv", opt_route);

    return 0;
}