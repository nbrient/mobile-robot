/**
 * @file bfsPathPlanner.cpp
 *
 * @brief BFS path planner implementation.
 *
 * @details
 * This file implements a simple BFS planner on an occupancy grid.
 *
 * @version 1.0
 */

/* System includes */
#include <algorithm>
#include <cstdio>
#include <queue>
#include <vector>

/* Project includes */
#include "nav/bfsPathPlanner.hpp"

/* Declarations */

/* Implementation */
BfsPathPlanner::BfsPathPlanner(float cellSize) : PathPlanner(cellSize) {
}

std::vector<Vector2Dim> BfsPathPlanner::computePath(const Map& map, float robotRadius, const Vector2Dim& start,
                                                    const Vector2Dim& target) const {
    /* Calculate grid size. */
    const int gridWidth = static_cast<int>(map.getWidth() / m_cellSize);
    const int gridHeight = static_cast<int>(map.getHeight() / m_cellSize);

    std::printf("[BFS] start=(%.1f, %.1f) target=(%.1f, %.1f)\n", start.x, start.y, target.x, target.y);

    /* Convert start point and target point to grid cell. */
    const GridCell startCell = worldToGrid(start);
    const GridCell targetCell = worldToGrid(target);

    /* Check start and target are on grid. */
    if ((startCell.x < 0) || (startCell.x >= gridWidth) || (startCell.y < 0) || (startCell.y >= gridHeight)) {
        std::printf("[BFS] ERROR start outside grid\n");
        return {};
    }

    if ((targetCell.x < 0) || (targetCell.x >= gridWidth) || (targetCell.y < 0) || (targetCell.y >= gridHeight)) {
        std::printf("[BFS] ERROR target outside grid\n");
        return {};
    }

    /* Check that start and target are not blocked. */
    const bool isStartBlocked = isCellBlocked(map, robotRadius, startCell.x, startCell.y);
    const bool isTargetBlocked = isCellBlocked(map, robotRadius, targetCell.x, targetCell.y);

    if (isStartBlocked || isTargetBlocked) {
        std::printf("[BFS] ERROR blocked start=%d target=%d startCell=(%d,%d) targetCell=(%d,%d)\n",
                    isStartBlocked ? 1 : 0, isTargetBlocked ? 1 : 0, startCell.x, startCell.y, targetCell.x,
                    targetCell.y);
        return {};
    }

    /* Convert a 2D grid into a unique linear index. */
    auto indexCellInLinear = [gridWidth](int x, int y) -> int { return (y * gridWidth) + x; };

    const std::size_t cellCount = static_cast<std::size_t>(gridWidth) * static_cast<std::size_t>(gridHeight);

    /* BFS working buffers. */
    std::vector<bool> visited(cellCount, false);
    std::vector<int> parent(cellCount, -1);

    /* BFS queue initialization with start cell. */
    std::queue<GridCell> queue;
    queue.push(startCell);
    visited[static_cast<std::size_t>(indexCellInLinear(startCell.x, startCell.y))] = true;

    /* 4-connected neighborhood offsets. */
    static constexpr std::size_t DIRECTION_COUNT = 4u;
    const int dx[DIRECTION_COUNT] = {1, -1, 0, 0};
    const int dy[DIRECTION_COUNT] = {0, 0, 1, -1};

    bool found = false;

    /* While cells to explore. */
    while (!queue.empty()) {
        const GridCell current = queue.front();
        queue.pop();

        /* Stop when target is reached. */
        if (areCellsEqual(current, targetCell)) {
            found = true;
            break;
        }

        /* Explore neighboring cells to find target. */
        for (std::size_t direction = 0; direction < DIRECTION_COUNT; direction++) {
            const int nextX = current.x + dx[direction];
            const int nextY = current.y + dy[direction];

            /* Ignore cells out of grid range. */
            if ((nextX < 0) || (nextX >= gridWidth) || (nextY < 0) || (nextY >= gridHeight)) {
                continue;
            }

            /* Ignore blocked cells. */
            if (isCellBlocked(map, robotRadius, nextX, nextY)) {
                continue;
            }

            const int nextIndex = indexCellInLinear(nextX, nextY);

            /* Ignore already visited cells. */
            if (visited[static_cast<std::size_t>(nextIndex)]) {
                continue;
            }

            visited[static_cast<std::size_t>(nextIndex)] = true;
            parent[static_cast<std::size_t>(nextIndex)] = indexCellInLinear(current.x, current.y);

            queue.push({nextX, nextY});
        }
    }

    /* No path found. */
    if (!found) {
        return {};
    }

    /* Reconstruct path backwards from target to start. */
    std::vector<GridCell> reversedCells;
    GridCell current = targetCell;

    while (!areCellsEqual(current, startCell)) {
        reversedCells.push_back(current);

        const int currentIndex = indexCellInLinear(current.x, current.y);
        const int parentIndex = parent[static_cast<std::size_t>(currentIndex)];

        /* Safety check: invalid parent means broken reconstruction. */
        if (parentIndex < 0) {
            return {};
        }

        current.x = parentIndex % gridWidth;
        current.y = parentIndex / gridWidth;
    }

    reversedCells.push_back(startCell);
    std::reverse(reversedCells.begin(), reversedCells.end());

    /* Convert grid cells back to world-space waypoints. */
    std::vector<Vector2Dim> path;
    path.reserve(reversedCells.size() + 1U);

    for (const GridCell& cell : reversedCells) {
        path.push_back(gridToWorld(cell));
    }

    /* Add target as final waypoint. */
    path.push_back(target);

    return path;
}
