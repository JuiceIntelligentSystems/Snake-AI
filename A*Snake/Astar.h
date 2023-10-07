#ifndef ASTAR_H
#define ASTAR_H

#include <bits/stdc++.h>

struct cell
{
    int parent_i, parent_j;
    double f, g, h;
};

typedef std::pair<int, int> Pair;
typedef std::pair<double, std::pair<int, int>> pPair;

class Astar
{
public:
    int aStar_trace_path[ASTAR_GRID_ROWS][ASTAR_GRID_COLS] = {0};

    bool displayPath = false;

    void aStarSearch(int grid[][ASTAR_GRID_COLS], Pair, Pair, bool);
    void clearPath();

private:
    bool isValid(int, int);
    bool isUnblocked(int grid[][ASTAR_GRID_COLS], int, int);
    bool isDestination(int, int, Pair);
    double calculateHValue(int, int, Pair);
    void tracePath(cell cellDetails[][ASTAR_GRID_COLS], Pair);
};

bool Astar::isValid(int row, int col)
{
    return (row >= 0) && (row < ASTAR_GRID_ROWS) && (col >= 0) && (col < ASTAR_GRID_COLS);
}

bool Astar::isUnblocked(int grid[][ASTAR_GRID_COLS], int row, int col)
{
    if (grid[row][col] == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Astar::isDestination(int row, int col, Pair dest)
{
    if (row == dest.first && col == dest.second)
    {
        return true;
    }
    else
    {
        return false;
    }
}

double Astar::calculateHValue(int row, int col, Pair dest)
{
    // Just the Distance Formula
    return ((double)sqrt((row - dest.first) * (row - dest.first) + (col - dest.second) * (col - dest.second)));
}

void Astar::tracePath(cell cellDetails[][ASTAR_GRID_COLS], Pair dest)
{
    // std::cout << "\nThe Path is:\n";
    int row = dest.first;
    int col = dest.second;

    std::stack<Pair> Path;

    while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col))
    {
        Path.push(std::make_pair(row, col));
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    Path.push(std::make_pair(row, col));
    int num_step = 0;
    while (!Path.empty())
    {
        std::pair<int, int> p = Path.top();
        Path.pop();
        // std::cout << "-> (" << p.first << "," << p.second << ") ";
        aStar_trace_path[p.first][p.second] = num_step;
        num_step++;
    }
    if (Path.empty() && displayPath)
    {
        for (int i = 0; i < ASTAR_GRID_ROWS; i++)
        {
            for (int j = 0; j < ASTAR_GRID_COLS; j++)
            {
                std::cout << aStar_trace_path[i][j];
            }
            std::cout << std::endl;
        }
    }
    return;
}

void Astar::clearPath()
{
    for (int i = 0; i < ASTAR_GRID_COLS; i++)
    {
        for (int j = 0; j < ASTAR_GRID_ROWS; j++)
        {
            aStar_trace_path[i][j] = 0;
        }
    }
}

void Astar::aStarSearch(int grid[][ASTAR_GRID_COLS], Pair src, Pair dest, bool reverse)
{
    // If Source or Destination is out of range
    if (isValid(src.first, src.second) == false)
    {
        std::cout << "Source is Invalid\n";
        return;
    }
    if (isValid(dest.first, dest.second) == false)
    {
        std::cout << "Destination is Invalid\n";
        return;
    }

    // If Source or Destination are blocked
    if (isUnblocked(grid, src.first, src.second) == false ||
        isUnblocked(grid, dest.first, dest.second) == false)
    {
        std::cout << "Source or Destination is Blocked\n";
        return;
    }

    // Shouldn't happen but if the destination is teh same as the source
    if (isDestination(src.first, src.second, dest) == true)
    {
        std::cout << "Already at Destination\n";
        return;
    }

    bool closedList[ASTAR_GRID_ROWS][ASTAR_GRID_COLS];
    memset(closedList, false, sizeof(closedList));

    cell cellDetails[ASTAR_GRID_ROWS][ASTAR_GRID_COLS];

    int i, j;
    for (i = 0; i < ASTAR_GRID_ROWS; i++)
    {
        for (j = 0; j < ASTAR_GRID_COLS; j++)
        {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }

    // Init Start Node
    i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;

    std::set<pPair> openList;
    openList.insert(std::make_pair(0.0, std::make_pair(i, j)));

    bool foundDest = false;
    while (!openList.empty())
    {
        pPair p = *openList.begin();

        openList.erase(openList.begin());

        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;

        // Import 8 Cardinal Directions(successors)
        // Store 'g', 'h', and 'f' of the successors
        double gNew, hNew, fNew;

        // 1st Successor: North
        if (isValid(i - 1, j) == true)
        {
            if (isDestination(i - 1, j, dest) == true)
            {
                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;
                // std::cout << "Destination Is Found\n";
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
            else if (closedList[i - 1][j] == false && isUnblocked(grid, i - 1, j) == true)
            {
                if (reverse)
                {
                    gNew = cellDetails[i][j].g - 1.0;
                    hNew = calculateHValue(i - 1, j, dest);
                    fNew = gNew - hNew;
                }
                else
                {
                    gNew = cellDetails[i][j].g + 1.0;
                    hNew = calculateHValue(i - 1, j, dest);
                    fNew = gNew + hNew;
                }

                if (cellDetails[i - 1][j].f == FLT_MAX || cellDetails[i - 1][j].f > fNew)
                {
                    openList.insert(std::make_pair(fNew, std::make_pair(i - 1, j)));

                    cellDetails[i - 1][j].f = fNew;
                    cellDetails[i - 1][j].g = gNew;
                    cellDetails[i - 1][j].h = hNew;
                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                }
            }
        }

        // 2nd Successor: South
        if (isValid(i + 1, j) == true)
        {
            if (isDestination(i + 1, j, dest) == true)
            {
                cellDetails[i + 1][j].parent_i = i;
                cellDetails[i + 1][j].parent_j = j;
                // std::cout << "Destination Is Found\n";
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
            else if (closedList[i + 1][j] == false && isUnblocked(grid, i + 1, j) == true)
            {
                if (reverse)
                {
                    gNew = cellDetails[i][j].g - 1.0;
                    hNew = calculateHValue(i + 1, j, dest);
                    fNew = gNew - hNew;
                }
                else
                {
                    gNew = cellDetails[i][j].g + 1.0;
                    hNew = calculateHValue(i + 1, j, dest);
                    fNew = gNew + hNew;
                }

                if (cellDetails[i + 1][j].f == FLT_MAX || cellDetails[i + 1][j].f > fNew)
                {
                    openList.insert(std::make_pair(fNew, std::make_pair(i + 1, j)));

                    cellDetails[i + 1][j].f = fNew;
                    cellDetails[i + 1][j].g = gNew;
                    cellDetails[i + 1][j].h = hNew;
                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                }
            }
        }

        // 3rd Successor: East
        if (isValid(i, j + 1) == true)
        {
            if (isDestination(i, j + 1, dest) == true)
            {
                cellDetails[i][j + 1].parent_i = i;
                cellDetails[i][j + 1].parent_j = j;
                // std::cout << "Destination Is Found\n";
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
            else if (closedList[i][j + 1] == false && isUnblocked(grid, i, j + 1) == true)
            {
                if (reverse)
                {
                    gNew = cellDetails[i][j].g - 1.0;
                    hNew = calculateHValue(i, j + 1, dest);
                    fNew = gNew - hNew;
                }
                else
                {
                    gNew = cellDetails[i][j].g + 1.0;
                    hNew = calculateHValue(i, j + 1, dest);
                    fNew = gNew + hNew;
                }

                if (cellDetails[i][j + 1].f == FLT_MAX || cellDetails[i][j + 1].f > fNew)
                {
                    openList.insert(std::make_pair(fNew, std::make_pair(i, j + 1)));

                    cellDetails[i][j + 1].f = fNew;
                    cellDetails[i][j + 1].g = gNew;
                    cellDetails[i][j + 1].h = hNew;
                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                }
            }
        }

        // 4th Successor: West
        if (isValid(i, j - 1) == true)
        {
            if (isDestination(i, j - 1, dest) == true)
            {
                cellDetails[i][j - 1].parent_i = i;
                cellDetails[i][j - 1].parent_j = j;
                // std::cout << "Destination Is Found\n";
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
            else if (closedList[i][j - 1] == false && isUnblocked(grid, i, j - 1) == true)
            {
                if (reverse)
                {
                    gNew = cellDetails[i][j].g - 1.0;
                    hNew = calculateHValue(i, j - 1, dest);
                    fNew = gNew - hNew;
                }
                else
                {
                    gNew = cellDetails[i][j].g + 1.0;
                    hNew = calculateHValue(i, j - 1, dest);
                    fNew = gNew + hNew;
                }

                if (cellDetails[i][j - 1].f == FLT_MAX || cellDetails[i][j - 1].f > fNew)
                {
                    openList.insert(std::make_pair(fNew, std::make_pair(i, j - 1)));

                    cellDetails[i][j - 1].f = fNew;
                    cellDetails[i][j - 1].g = gNew;
                    cellDetails[i][j - 1].h = hNew;
                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                }
            }
        }
    }

    if (foundDest == false)
    {
        std::cout << "Failed to find Destination\n";
    }
    return;
}

#endif