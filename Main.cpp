#include <iostream>
#include <queue>
#include <deque>
#include <set>
#include <vector>
#include <string>
#include <stdbool.h>
#include <stack>
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

int facing = 0;

std::pair<int, int> start = {0, 0};

std::vector<std::pair<int, int>> centers = {{cx1, cy1}, {cx2, cy1}, {cx1, cy2}, {cx2, cy2}};

std::vector<std::vector<int>> directions = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

std::set<std::pair<int, int>> verticalWalls = {};
std::set<std::pair<int, int>> horizontalWalls = {};

std::stack<std::pair<int, int>> toVisit;
std::deque<std::pair<int, int>> currentPath = {start};

std::set<std::deque<std::pair<int, int>>> allPaths;

std::set<std::pair<int, int>> visited;

std::set<std::deque<std::pair<int, int>>> pathsToCenter;

std::deque<std::pair<int, int>> currPathToGoal;

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

std::vector<std::vector<int>> distances = floodFill(gridSize, centers, verticalWalls, horizontalWalls);

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
            else
            {
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
            else
            {
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
            else
            {
                API::setWall(x, y, 's');
            }
        }
        if (rightWall)
        {
            if (x > 0)
            {
                verticalWalls.insert({x - 1, y});
            }
            else
            {
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
            else
            {
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
            else
            {
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
    return false;
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
    else if (x == 8 && y == 7)
    {
        mouseControl(x, y, 8, 8, facing);
        wallSet(8, 8, facing, verticalWalls, horizontalWalls);
        mouseControl(8, 8, 7, 8, facing);
        wallSet(7, 8, facing, verticalWalls, horizontalWalls);
        mouseControl(7, 8, 7, 7, facing);
        wallSet(7, 7, facing, verticalWalls, horizontalWalls);
        mouseControl(7, 7, 8, 7, facing);
    }
    else if (x == 7 && y == 8)
    {
        mouseControl(x, y, 8, 8, facing);
        wallSet(8, 8, facing, verticalWalls, horizontalWalls);
        mouseControl(8, 8, 8, 7, facing);
        wallSet(8, 7, facing, verticalWalls, horizontalWalls);
        mouseControl(8, 7, 7, 7, facing);
        wallSet(7, 7, facing, verticalWalls, horizontalWalls);
        mouseControl(7, 7, 7, 8, facing);
    }
    else
    {
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

// std::vector<std::vector<int>> getDirections(int facing) {
//     if (facing == 0) {
//         return {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
//     }
//     else if (facing == 1) {
//         return {{1, 0}, {0, -1}, {0, 1}, {-1, 0}};
//     }
//     else if (facing == 2) {
//         return {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};
//     }
//     else {
//         return {{-1, 0}, {0, 1}, {0, -1}, {1, 0}};
//     }
// }

void exploreMaze(std::pair<int, int> curr, int &facing)
{
    int x = curr.first;
    int y = curr.second;

    int wallCount = wallSet(x, y, facing, verticalWalls, horizontalWalls);

    std::vector<std::vector<int>> distances = floodFill(gridSize, centers, verticalWalls, horizontalWalls);

    for (int i = 0; i < gridSize; i++)
    {
        for (int j = 0; j < gridSize; j++)
        {
            API::setText(j, i, std::to_string(distances[i][j]));
        }
    }

    // START-1.1

    bool hasPathToCenter = false;
    std::deque<std::pair<int, int>> pathFromCell;

    for (const auto &path : pathsToCenter)
    {
        if (path.front().first == x && path.front().second == y)
        {
            pathFromCell = path;
            hasPathToCenter = true;
            break;
        }
    }
    
    // log(std::to_string(x) + " " + std::to_string(y));    
    // log(std::to_string(hasPathToCenter));
    // log("-----------");

    if (hasPathToCenter)
    {
        std::deque<std::pair<int, int>> temp = currentPath;
    
        if (temp.size() == 0)
        {
            return;
        }
        temp.pop_back();
        for (const auto &cell : pathFromCell)
        {
            temp.push_back(cell);
        }

        // if (currPathToGoal == temp) {
        //     log("True");
        //     return;
        // }

        API::clearAllColor();
        for (const auto &newCell : temp)
        {
            API::setColor(newCell.first, newCell.second, 'G');
        }
        allPaths.insert(temp);

        // currPathToGoal = temp;

        for (const auto &cell : temp)
        {
            bool hasPath = false;
            std::deque<std::pair<int, int>> tempDeque;
            std::deque<std::pair<int, int>> ptc;
            for (const auto &currPath : pathsToCenter)
            {
                if (currPath.front().first == cell.first && currPath.front().second == cell.second)
                {
                    hasPath = true;
                    ptc = currPath;
                    break;
                }
            }

            bool cellFound = false;
            for (const auto &cellInTemp : temp)
            {
                if (cellInTemp.first == cell.first && cellInTemp.second == cell.second)
                {
                    cellFound = true;
                }
                if (cellFound)
                {
                    tempDeque.push_back(cellInTemp);
                }
            }

            if (hasPath)
            {
                if (tempDeque.size() <= ptc.size())
                {
                    pathsToCenter.erase(ptc);
                }
            }
            pathsToCenter.insert(tempDeque);
        }
        return;
    } 

    // END-1.1

    if ((x == cx1 || x == cx2) && (y == cy1 || y == cy2))
    {
        visited.insert({x, y});
        exploreCenters(x, y, facing, distances, verticalWalls, horizontalWalls);
        API::setColor(x, y, 'R');
        allPaths.insert(currentPath);
        // currPathToGoal = currentPath; 
        API::clearAllColor();
        // int i = 0;
        for (const auto &cell : currentPath)
        {
            API::setColor(cell.first, cell.second, 'Y');
            // API::setText(cell.first, cell.second, std::to_string(i));
            // i++;

            // START-1.2

            bool cellFound = false;
            std::deque<std::pair<int, int>> tempQ;
            for (const auto &currCell : currentPath)
            {
                if (currCell == cell)
                {
                    cellFound = true;
                }
                if (cellFound == true)
                {
                    tempQ.push_back(currCell);
                }
            }
            pathsToCenter.insert(tempQ);

            // END-1.2
        }
        return;
    }

    // directions = getDirections(facing);          //OPTIMIZE-1

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

        if (((distances[y][x] > distances[ny][nx]) && (distances[y][x] - distances[ny][nx] > 0)) && (isCellAccessible(x, y, nx, ny, verticalWalls, horizontalWalls)))
        {
            std::pair<int, int> next = {nx, ny};
            for (const auto &cell : currentPath)
            {
                if (cell == next)
                {
                    while (currentPath.size() != 0 && currentPath.back() != next)
                    {
                        currentPath.pop_back();
                    }
                    if (currentPath.size() != 0)
                    {
                        currentPath.pop_back();
                    }
                    break;
                }
            }

            currentPath.push_back(next);

            API::setColor(nx, ny, 'b');

            mouseControl(x, y, nx, ny, facing);

            visited.insert({x, y});

            exploreMaze(next, facing);
            break;
        }


    }

    // bool inCurrentPath = false;
    // for (const auto &cell : currentPath)
    // {
    //     if (cell.first == x && cell.second == y)
    //     {
    //         log(std::to_string(x) + " " + std::to_string(y));
    //         inCurrentPath = true;
    //         break;
    //     }
    // }

    if (currentPath.size() == 0 || (currentPath.back().first == x && currentPath.back().second == y))
    {
        return;
    }

    int fromX = currentPath.back().first;
    int fromY = currentPath.back().second;

    currentPath.pop_back();

    if (currentPath.size() == 0)
    {
        return;
    }
    int toX = currentPath.back().first;
    int toY = currentPath.back().second;

    mouseControl(fromX, fromY, toX, toY, facing);

    distances = floodFill(gridSize, centers, verticalWalls, horizontalWalls);

    for (int i = 0; i < gridSize; i++)
    {
        for (int j = 0; j < gridSize; j++)
        {
            API::setText(j, i, std::to_string(distances[i][j]));
        }
    }

    // directions = getDirections(facing);                  //OPTIMIZE-1
    for (const auto &direction : directions)
    {
        int dx = direction[0];
        int dy = direction[1];
        int nx = toX + dx;
        int ny = toY + dy;

        if (nx < 0 || nx >= gridSize || ny < 0 || ny >= gridSize)
        {
            continue;
        }

        if (distances[toY][toX] > distances[ny][nx] && !visited.count({nx, ny}) && isCellAccessible(toX, toY, nx, ny, verticalWalls, horizontalWalls))
        {
            std::pair<int, int> nextCell = {nx, ny};
            currentPath.push_back(nextCell);
            API::setColor(toX, toY, 'y');
            mouseControl(toX, toY, nx, ny, facing);
            API::setColor(nx, ny, 'b');
            exploreMaze(nextCell, facing);
            // return;
            break;
        }
    }
    return;
}

int main(int argc, char *argv[])
{
    log("Running...");
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "START");

    exploreMaze(start, facing);

    log("EXPLORED");

    int shortestPathSize = 999;
    std::deque<std::pair<int, int>> shortestPath;

    for (const auto &path : allPaths)
    {
        if (path.size() < shortestPathSize)
        {
            shortestPath = path;
            shortestPathSize = path.size();
        }
    }

    // int i = shortestPathSize - 1;
    for (const auto &cell : shortestPath)
    {
        int cx = cell.first;
        int cy = cell.second;
        // API::setText(cx, cy, std::to_string(i));
        // i--;
        API::setColor(cx, cy, 'C');
    }
}
