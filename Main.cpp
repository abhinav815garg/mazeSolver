#include <iostream>
#include <queue>
#include <deque>
#include <set>
#include <vector>
#include <string>
#include <stdbool.h>
// using namespace std;

#include "API.h"

void log(const std::string &text)
{
    std::cerr << text << std::endl;
}

int gridSize = 16;

int cy1 = gridSize / 2;
int cy2 = cy1 - 1;
int cx1 = gridSize / 2;
int cx2 = cx1 - 1;

std::vector<std::pair<int, int>> centers = {{cx1, cy1}, {cx2, cy1}, {cx1, cy2}, {cx2, cy2}};

std::vector<std::vector<int>> directions = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

std::vector<std::vector<int>> floodFill(int gridSize, const std::vector<std::pair<int, int>> &centers, const std::set<std::pair<int, int>> &verticalWalls, const std::set<std::pair<int, int>> &horizontalWalls)
{
    std::queue<std::pair<int, int>> q;

    std::vector<std::vector<int>> distance(gridSize, std::vector<int>(gridSize, 999));

    for (const auto &center : centers)
    {
        int x = center.first;
        int y = center.second;
        distance[y][x] = 0;
        q.push(center);
    }

    while (!q.empty())
    {
        std::pair<int, int> curr = q.front();
        q.pop();

        int x = curr.first;
        int y = curr.second;

        for (const auto &direction : directions)
        {
            int dx = direction[0];
            int dy = direction[1];
            int nx = x + dx;
            int ny = y + dy;

            if (nx < 0 || nx >= gridSize || ny < 0 || ny >= gridSize)
            {
                continue;
            }

            bool isAccessible = true;

            if (dx == 0)
            {
                if (dy == -1)
                {
                    isAccessible = !horizontalWalls.count({nx, ny});
                }
                else
                {
                    isAccessible = !horizontalWalls.count({x, y});
                }
            }
            else
            {
                if (dx == -1)
                {
                    isAccessible = !verticalWalls.count({nx, ny});
                }
                else
                {
                    isAccessible = !verticalWalls.count({x, y});
                }
            }

            if (isAccessible && distance[ny][nx] > distance[y][x] + 1)
            {
                distance[ny][nx] = distance[y][x] + 1;
                q.push({nx, ny});
            }
        }
    }
    return distance;
}

void mouseControl(int currX, int currY, int nextX, int nextY, int &facing)
{
    int dx = nextX - currX;
    int dy = nextY - currY;

    if (dx == 0 && dy == -1)
    {
        if (facing == 0)
        {
            API::turnRight();
            API::turnRight();
            API::moveForward();
        }
        else if (facing == 1)
        {
            API::turnRight();
            API::moveForward();
        }
        else if (facing == 2)
        {
            API::moveForward();
        }
        else
        {
            API::turnLeft();
            API::moveForward();
        }
        facing = 2;
    }
    else if (dx == 1 && dy == 0)
    {
        if (facing == 0)
        {
            API::turnRight();
            API::moveForward();
        }
        else if (facing == 1)
        {
            API::moveForward();
        }
        else if (facing == 2)
        {
            API::turnLeft();
            API::moveForward();
        }
        else
        {
            API::turnRight();
            API::turnRight();
            API::moveForward();
        }
        facing = 1;
    }
    else if (dx == 0 && dy == 1)
    {
        if (facing == 0)
        {
            API::moveForward();
        }
        else if (facing == 1)
        {
            API::turnLeft();
            API::moveForward();
        }
        else if (facing == 2)
        {
            API::turnRight();
            API::turnRight();
            API::moveForward();
        }
        else
        {
            API::turnRight();
            API::moveForward();
        }
        facing = 0;
    }
    else
    {
        if (facing == 0)
        {
            API::turnLeft();
            API::moveForward();
        }
        else if (facing == 1)
        {
            API::turnRight();
            API::turnRight();
            API::moveForward();
        }
        else if (facing == 2)
        {
            API::turnRight();
            API::moveForward();
        }
        else
        {
            API::moveForward();
        }
        facing = 3;
    }
}

int wallSet(int x, int y, int facing, std::set<std::pair<int, int>> &verticalWalls, std::set<std::pair<int, int>> &horizontalWalls)
{
    bool frontWall = API::wallFront();
    bool rightWall = API::wallRight();
    bool leftWall = API::wallLeft();

    if (facing == 0)
    {
        if (frontWall)
        {
            horizontalWalls.insert({x, y});
        }
        if (rightWall)
        {
            verticalWalls.insert({x, y});
        }
        if (leftWall)
        {
            if (x > 0)
            {
                verticalWalls.insert({x - 1, y});
            }
            else {
                API::setWall(x, y, 'w');
            }
        }
    }
    if (facing == 1)
    {
        if (frontWall)
        {
            verticalWalls.insert({x, y});
        }
        if (rightWall)
        {
            if (y > 0)
            {
                horizontalWalls.insert({x, y - 1});
            }
            else {
                API::setWall(x, y, 's');
            }
        }
        if (leftWall)
        {
            horizontalWalls.insert({x, y});
        }
    }
    if (facing == 2)
    {
        if (frontWall)
        {
            if (y > 0)
            {
                horizontalWalls.insert({x, y - 1});
            }
            else {
                API::setWall(x, y, 's');
            }
        }
        if (rightWall)
        {
            if (x > 0)
            {
                verticalWalls.insert({x - 1, y});
            }
            else {
                API::setWall(x, y, 'w');
            }
        }
        if (leftWall)
        {
            verticalWalls.insert({x, y});
        }
    }
    if (facing == 3)
    {
        if (frontWall)
        {
            if (x > 0)
            {
                verticalWalls.insert({x - 1, y});
            }
            else {
                API::setWall(x, y, 'w');
            }
        }
        if (rightWall)
        {
            horizontalWalls.insert({x, y});
        }
        if (leftWall)
        {
            if (y > 0)
            {
                horizontalWalls.insert({x, y - 1});
            }
            else {
                API::setWall(x, y, 's');
            }
        }
    }

    for (const auto &wall : verticalWalls)
    {
        API::setWall(wall.first, wall.second, 'e');
    }

    for (const auto &wall : horizontalWalls)
    {
        API::setWall(wall.first, wall.second, 'n');
    }

    int wallCount = frontWall + rightWall + leftWall;
    return wallCount;
}

bool isCellAccessible(int x, int y, int nx, int ny, const std::set<std::pair<int, int>> &verticalWalls, const std::set<std::pair<int, int>> &horizontalWalls)
{
    int dx = nx - x;
    int dy = ny - y;

    if (dx == 0)
    {
        if (dy == -1)
        {
            return !horizontalWalls.count({nx, ny});
        }
        else
        {
            return !horizontalWalls.count({x, y});
        }
    }
    else
    {
        if (dx == -1)
        {
            return !verticalWalls.count({nx, ny});
        }
        else
        {
            return !verticalWalls.count({x, y});
        }
    }
}

void exploreCenters(int x, int y, int &facing, std::vector<std::vector<int>> &distances, std::set<std::pair<int, int>> &verticalWalls, std::set<std::pair<int, int>> &horizontalWalls)
{
    wallSet(x, y, facing, verticalWalls, horizontalWalls);

    if (x == 7 && y == 7)
    {
        mouseControl(x, y, 7, 8, facing);
        wallSet(7, 8, facing, verticalWalls, horizontalWalls);
        mouseControl(7, 8, 8, 8, facing);
        wallSet(8, 8, facing, verticalWalls, horizontalWalls);
        mouseControl(8, 8, 8, 7, facing);
        wallSet(8, 7, facing, verticalWalls, horizontalWalls);
        mouseControl(8, 7, 7, 7, facing);
    }
    else if (x == 8 && y == 7) {
        mouseControl(x, y, 8, 8, facing);
        wallSet(8, 8, facing, verticalWalls, horizontalWalls);
        mouseControl(8, 8, 7, 8, facing);
        wallSet(7, 8, facing, verticalWalls, horizontalWalls);
        mouseControl(7, 8, 7, 7, facing);
        wallSet(7, 7, facing, verticalWalls, horizontalWalls);
        mouseControl(7, 7, 8, 7, facing);
    }
    else if (x == 7 && y == 8) {
        mouseControl(x, y, 8, 8, facing);
        wallSet(8, 8, facing, verticalWalls, horizontalWalls);
        mouseControl(8, 8, 8, 7, facing);
        wallSet(8, 7, facing, verticalWalls, horizontalWalls);
        mouseControl(8, 7, 7, 7, facing);
        wallSet(7, 7, facing, verticalWalls, horizontalWalls);
        mouseControl(7, 7, 7, 8, facing);
    }
    else {
        mouseControl(x, y, 8, 7, facing);
        wallSet(8, 7, facing, verticalWalls, horizontalWalls);
        mouseControl(8, 7, 7, 7, facing);
        wallSet(7, 7, facing, verticalWalls, horizontalWalls);
        mouseControl(7, 7, 7, 8, facing);
        wallSet(7, 8, facing, verticalWalls, horizontalWalls);
        mouseControl(7, 8, 8, 8, facing);
    }

    distances = floodFill(gridSize, centers, verticalWalls, horizontalWalls);


    for (int i = 0; i < gridSize; i++)
    {
        for (int j = 0; j < gridSize; j++)
        {
            API::setText(j, i, std::to_string(distances[i][j]));
        }
    }
}

int main(int argc, char *argv[])
{
    log("Running...");
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "START");

    int x = 0;
    int y = 0;

    int facing = 0;
    // 0: up, 1: right, 2: down, 3: left


    std::deque<std::pair<int, int>> toVisit;

    std::deque<std::pair<int, int>> shortestPath;
    shortestPath.push_back({x, y});

    std::set<std::pair<int, int>> verticalWalls = {};
    std::set<std::pair<int, int>> horizontalWalls = {};

    std::vector<std::vector<int>> distances = floodFill(gridSize, centers, verticalWalls, horizontalWalls);

    for (int i = 0; i < gridSize; i++)
    {
        for (int j = 0; j < gridSize; j++)
        {
            API::setText(j, i, std::to_string(distances[i][j]));
        }
    }

    while (true)
    {
        std::pair<int, int> current = shortestPath.back();
        x = current.first;
        y = current.second;

        if ((x == cx1 || x == cx2) && (y == cy1 || y == cy2))
        {   
            exploreCenters(x, y, facing, distances, verticalWalls, horizontalWalls);
            API::setColor(x, y, 'R');
            int i = shortestPath.size() - 1;
            for (const auto &cell : shortestPath)
            {
                int cx = cell.first;
                int cy = cell.second;
                API::setText(cx, cy, std::to_string(i));
                i--;
                API::setColor(cx, cy, 'Y');
            }
            break;
        }

        int wallCount = wallSet(x, y, facing, verticalWalls, horizontalWalls);

        std::vector<std::vector<int>> distances = floodFill(gridSize, centers, verticalWalls, horizontalWalls);

        for (int i = 0; i < gridSize; i++)
        {
            for (int j = 0; j < gridSize; j++)
            {
                API::setText(j, i, std::to_string(distances[i][j]));
            }
        }

        int minDiff = 999;

        for (const auto &direction : directions)
        {
            int dx = direction[0];
            int dy = direction[1];
            int nx = x + dx;
            int ny = y + dy;

            if (nx < 0 || nx >= gridSize || ny < 0 || ny >= gridSize)
            {
                continue;
            }

            if (((distances[y][x] - distances[ny][nx] < minDiff) && (distances[y][x] - distances[ny][nx] > 0)) && (isCellAccessible(x, y, nx, ny, verticalWalls, horizontalWalls)))
            {
                if (minDiff != 999){
                    toVisit.pop_back();
                }
                toVisit.push_back({nx, ny});
                minDiff = distances[y][x] - distances[ny][nx];
            }
        }

        std::pair<int, int> next = toVisit.back();
        int nx = next.first;
        int ny = next.second;

        for (const auto &cell: shortestPath)
        {
            if (cell == next)
            {
                while (shortestPath.back() != next)
                {
                    shortestPath.pop_back();
                }
                shortestPath.pop_back();
                break;
            }
        }

        shortestPath.push_back(next);
        toVisit.pop_back();

        API::setColor(nx, ny, 'b');

        mouseControl(x, y, nx, ny, facing);
    }
}
