#include <bitset>
#include "include/maze_reader.hpp"
#include "include/a_star.hpp"

int main() {
    std::string file_name = "/home/ryunosuke/micromouse/maze5x5.csv";
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
                std::cout << std::bitset<8>(w.flags) << std::endl;
            }
        }
    }

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
        std::cout << "(" << next.x << ", " << next.y << ")" << std::endl;

        auto wall = maze[next];
        wall.is_known_north = wall.is_known_east = wall.is_known_south = wall.is_known_west = true;
        search.setWall(next, wall);
    }

    return 0;
}