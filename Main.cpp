#include <iostream>
#include <queue>
#include <limits>
#include <set>
#include <vector>
#include <string>
#include <stdbool.h>
// using namespace std;

#include "API.h"

void log(const std::string& text) {
    std::cerr << text << std::endl;
}

std::vector<std::vector<int>> directions = {{ 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 }};

std::vector<std::vector<int>> floodFill(int gridSize, const std::vector<std::pair<int, int>>& centers, const std::set<std::pair<int, int>>& verticalWalls, const std::set<std::pair<int, int>>& horizontalWalls){
    std::queue<std::pair<int, int>> q;

    std::vector<std::vector<int>> distance(gridSize, std::vector<int>(gridSize, 999));

    for (const auto& center : centers) {
        int x = center.first;
        int y = center.second;
        distance[y][x] = 0;
        q.push(center);
    }

    while (!q.empty()) {
        std::pair<int, int> curr = q.front();
        q.pop();

        int x = curr.first;
        int y = curr.second;

        for (const auto& direction: directions) {
            int dx = direction[0];
            int dy = direction[1];
            int nx = x + dx;
            int ny = y + dy;

            // log(std::to_string(nx) + " " + std::to_string(ny));

            if (nx < 0 || nx >= gridSize || ny < 0 || ny >= gridSize) {
                continue;
            }

            bool isAccessible = true;

            if (dx == 0) {
                if (dy == -1) {
                    isAccessible = !horizontalWalls.count({ x, y });
                } else {
                    isAccessible = !horizontalWalls.count({ nx, ny });
                }
            } else {
                if (dx == -1) {
                    isAccessible = !verticalWalls.count({ nx, ny });
                } else {
                    isAccessible = !verticalWalls.count({ x, y });
                }
            }

            if (isAccessible && distance[ny][nx] > distance[y][x] + 1) {
                distance[ny][nx] = distance[y][x] + 1;
                q.push({ nx, ny });
            }
        }
    }
    return distance;
}


int main(int argc, char* argv[]) {
    log("Running...");
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "START");
    // API::clearAllText();

    int gridSize = 16;

    int x = 0;
    int y = 0;

    int cy1 = gridSize / 2;
    int cy2 = cy1 - 1;
    int cx1 = gridSize / 2;
    int cx2 = cx1 - 1;

    std::vector<std::pair<int, int>> centers = {{cx1, cy1}, {cx2, cy1}, {cx1, cy2}, {cx2, cy2}};

    std::queue<std::pair<int, int>> toVisit;
    toVisit.push({ x, y });

    std::set<std::pair<int, int>> verticalWalls = {};
    std::set<std::pair<int, int>> horizontalWalls = {};

    std::vector<std::vector<int>> distances = floodFill(gridSize, centers, verticalWalls, horizontalWalls);

    // for (int i = 0; i < gridSize; i++) {
    //     for (int j = 0; j < gridSize; j++) {
    //         log(std::to_string(distances[i][j]));
    //     }
    // }

    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            API::setText(j, i, std::to_string(distances[i][j]));
        }
    }

    // while (true) {
    //     pair<int, int> current = toVisit.front();
    //     toVisit.pop();

    //     x = current.first;
    //     y = current.second;

    //     for (const auto& direction : directions) {
    //         int dx = direction[0];
    //         int dy = direction[1];
    //         int nx = x + dx;
    //         int ny = y + dy;

    //         if (nx < 0 || nx >= gridSize || ny < 0 || ny >= gridSize) {
    //             continue;
    //         }
    //     }        




    //     if ((x == cx1 || x == cx2) && (y == cy1 || y == cy2)) {
    //         break;
    //     }
    // }
}
