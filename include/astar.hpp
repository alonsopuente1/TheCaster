#pragma once

#include <cmath>
#include <vector>
#include <stack>
#include <float.h>
#include <exception>

#include "castengine/map.hpp"

struct Node
{
    int x, y;
    int parentX, parentY;
    float gCost, hCost, fCost;
};

inline bool operator < (const Node& lhs, const Node& rhs)
{//We need to overload "<" to put our struct into a set
    return lhs.fCost < rhs.fCost;
}

static bool isValid(int x, int y, const CastEngine::Map& map) 
{ 
    return x >= 0 && x < map.GetWidth() && y >= 0 && y < map.GetHeight() && map[x + y * map.GetWidth()] == 0;
}


static bool isDestination(int x, int y, Node dest) 
{
    if (x == dest.x && y == dest.y) 
        return true;

    return false;
}

static double calculateH(int x, int y, Node dest) 
{
    double H = (sqrt((x - dest.x)*(x - dest.x)
        + (y - dest.y)*(y - dest.y)));

        return H;
}

static std::vector<Node> makePath(std::vector<std::vector<Node>>& map, Node dest);
static std::vector<Node> aStar(Node start, Node dest, const CastEngine::Map& map)
{
    std::vector<Node> path{};

    if (!isValid(start.x, start.y, map) || !isValid(dest.x, dest.y, map)) 
    {
        return path;
    }

    if(isDestination(start.x, start.y, dest)) 
    {
        return path;
    }

    std::vector<std::vector<bool>> closedList(map.GetWidth(), std::vector<bool>(map.GetHeight(), false));
    std::vector<std::vector<Node>> allMap(map.GetWidth(), std::vector<Node>(map.GetHeight()));

    for (int x = 0; x < map.GetWidth(); x++) 
    {
        for (int y = 0; y < map.GetHeight(); y++) 
        {
            allMap[x][y].fCost = FLT_MAX;
            allMap[x][y].gCost = FLT_MAX;
            allMap[x][y].hCost = FLT_MAX;
            allMap[x][y].parentX = -1;
            allMap[x][y].parentY = -1;
            allMap[x][y].x = x;
            allMap[x][y].y = y;
        }
    }

    allMap[start.x][start.y].fCost = 0.0;
    allMap[start.x][start.y].gCost = 0.0;
    allMap[start.x][start.y].hCost = 0.0;
    allMap[start.x][start.y].parentX = start.x;
    allMap[start.x][start.y].parentY = start.y;

    std::vector<Node> openList;
    openList.push_back(allMap[start.x][start.y]);

    while(!openList.empty() && static_cast<int>(openList.size()) < map.GetWidth() * map.GetHeight())
    {
        Node node;

        do {
                //This do-while loop could be replaced with extracting the first
                //element from a set, but you'd have to make the openList a set.
                //To be completely honest, I don't remember the reason why I do
                //it with a vector, but for now it's still an option, although
                //not as good as a set performance wise.
                float temp = FLT_MAX;
                std::vector<Node>::iterator itNode;
                for (std::vector<Node>::iterator it = openList.begin();
                    it != openList.end(); it = std::next(it)) {
                    Node n = *it;
                    if (n.fCost < temp) {
                        temp = n.fCost;
                        itNode = it;
                    }
                }
                node = *itNode;
                openList.erase(itNode);
            } while (isValid(node.x, node.y, map) == false);

        int x = node.x, y = node.y;
        closedList[x][y] = true;

        //For each neighbour starting from North-West to South-East
        for (int newX = -1; newX <= 1; newX++) {
            for (int newY = -1; newY <= 1; newY++) {
                double gNew, hNew, fNew;
                if (isValid(x + newX, y + newY, map)) {
                    if (isDestination(x + newX, y + newY, dest))
                    {
                        //Destination found - make path
                        allMap[x + newX][y + newY].parentX = x;
                        allMap[x + newX][y + newY].parentY = y;
                        return makePath(allMap, dest);
                    }
                    else if (closedList[x + newX][y + newY] == false)
                    {
                        gNew = node.gCost + 1.0;
                        hNew = calculateH(x + newX, y + newY, dest);
                        fNew = gNew + hNew;
                        // Check if this path is better than the one already present
                        if (allMap[x + newX][y + newY].fCost == FLT_MAX ||
                            allMap[x + newX][y + newY].fCost > fNew)
                        {
                            // Update the details of this neighbour node
                            allMap[x + newX][y + newY].fCost = fNew;
                            allMap[x + newX][y + newY].gCost = gNew;
                            allMap[x + newX][y + newY].hCost = hNew;
                            allMap[x + newX][y + newY].parentX = x;
                            allMap[x + newX][y + newY].parentY = y;
                            openList.emplace_back(allMap[x + newX][y + newY]);
                        }
                    }
                }
            }
        }
    }
    
    return std::vector<Node>();
}

static std::vector<Node> makePath(std::vector<std::vector<Node>>& map, Node dest) 
{
    try {
        int x = dest.x;
        int y = dest.y;
        std::stack<Node> path;
        std::vector<Node> usablePath;

        while (!(map[x][y].parentX == x && map[x][y].parentY == y)
        && map[x][y].x != -1 && map[x][y].y != -1) 
        {
            path.push(map[x][y]);
            int tempX = map[x][y].parentX;
            int tempY = map[x][y].parentY;
            x = tempX;
            y = tempY;

        }
        path.push(map[x][y]);

        while (!path.empty()) {
            Node top = path.top();
            path.pop();
            usablePath.emplace_back(top);
        }
        return usablePath;
    }
    catch(const std::exception& e){
        return std::vector<Node>();
    }
}
