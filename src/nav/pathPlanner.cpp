/**
 * @file pathPlanner.cpp
 *
 * @brief Grid-based path planner implementation.
 *
 * @details
 * This file implements a simple BFS planner on an occupancy grid.
 *
 * @version 1.0
 */

/* System includes */
#include <stdio.h>

#include <algorithm>
#include <queue>
#include <vector>

/* Project includes */
#include "common/common.hpp"
#include "nav/pathPlanner.hpp"

/* Implementation */
namespace {
/**
 * @brief Additional margin for wall.
 *
 */
static constexpr float additionalWallMargin{4.0f};

/**
 * @brief Additional margin for wall.
 *
 */
static constexpr float obstacleAdditionalMargin{8.0f};

/**
 * @brief Check if two cells are the same.
 *
 * @param left Left cell.
 * @param right Right cell.
 * @return bool True if cells are equal, false otherwise.
 */
bool areCellsEqual(const GridCell& left, const GridCell& right) {
    return (left.x == right.x) && (left.y == right.y);
}
}  // namespace

PathPlanner::PathPlanner(float cellSize) : m_cellSize(cellSize) {
}

std::vector<Vector2Dim> PathPlanner::computePath(const Map& map, float robotRadius, const Vector2Dim& start,
                                                 const Vector2Dim& target) const {
    /* Calculates grid size */
    const int gridWidth = static_cast<int>(map.getWidth() / m_cellSize);
    const int gridHeight = static_cast<int>(map.getHeight() / m_cellSize);

    /* Converts start point and target point to grid cell */
    const GridCell startCell = worldToGrid(start);
    const GridCell targetCell = worldToGrid(target);

    /* Check start and target are on grid */
    if ((startCell.x < 0) || (startCell.x >= gridWidth) || (startCell.y < 0) || (startCell.y >= gridHeight)) {
        return {};
    }

    if ((targetCell.x < 0) || (targetCell.x >= gridWidth) || (targetCell.y < 0) || (targetCell.y >= gridHeight)) {
        return {};
    }

    /* Check that start and target are not blocked */
    if (isCellBlocked(map, robotRadius, startCell.x, startCell.y) ||
        isCellBlocked(map, robotRadius, targetCell.x, targetCell.y)) {
        return {};
    }

    /* Converts a 2D grid into a unique linear index */
    auto indexCellInLinear = [gridWidth](int x, int y) -> int { return (y * gridWidth) + x; };

    const int cellCount = gridWidth * gridHeight;

    /* BFS working buffers */
    /* Visited cells */
    std::vector<bool> visited(static_cast<std::size_t>(cellCount), false);
    std::vector<int> parent(static_cast<std::size_t>(cellCount), -1);

    /* BFS (Breadth first search) queue initialization with start cell */
    std::queue<GridCell> queue;
    queue.push(startCell);
    visited[static_cast<std::size_t>(indexCellInLinear(startCell.x, startCell.y))] = true;

    /* 4-connected neighborhood offsets */
    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    bool found = false;

    /* While cells to explore */
    while (!queue.empty()) {
        const GridCell current = queue.front();
        queue.pop();

        /* Stop awhen target is reached */
        if (areCellsEqual(current, targetCell)) {
            found = true;
            break;
        }

        /* Explore neighboring cells to find target */
        for (int direction = 0; direction < 4; direction++) {
            const int nextX = current.x + dx[direction];
            const int nextY = current.y + dy[direction];

            /* Ignre cells out of range grid */
            if ((nextX < 0) || (nextX >= gridWidth) || (nextY < 0) || (nextY >= gridHeight)) {
                continue;
            }

            /* Ignore blocked cells */
            if (isCellBlocked(map, robotRadius, nextX, nextY)) {
                continue;
            }

            const int nextIndex = indexCellInLinear(nextX, nextY);

            /* Ignore already visited cells */
            if (visited[static_cast<std::size_t>(nextIndex)]) {
                continue;
            }

            visited[static_cast<std::size_t>(nextIndex)] = true;
            parent[static_cast<std::size_t>(nextIndex)] = indexCellInLinear(current.x, current.y);

            queue.push({nextX, nextY});
        }
    }

    /* No path found */
    if (!found) {
        return {};
    }

    /* Reconstruct path backwards from target to start */
    std::vector<GridCell> reversedCells;
    GridCell current = targetCell;

    while (!areCellsEqual(current, startCell)) {
        reversedCells.push_back(current);

        const int currentIndex = indexCellInLinear(current.x, current.y);
        const int parentIndex = parent[static_cast<std::size_t>(currentIndex)];

        /* Safety check: invalid parent means broken reconstruction */
        if (parentIndex < 0) {
            return {};
        }

        current.x = parentIndex % gridWidth;
        current.y = parentIndex / gridWidth;
    }

    reversedCells.push_back(startCell);
    /* Put in the right order */
    std::reverse(reversedCells.begin(), reversedCells.end());

    /* Convert grid cells back to world-space waypoints */
    std::vector<Vector2Dim> path;
    path.reserve(reversedCells.size());

    /* Convert cells to position */
    for (const GridCell& cell : reversedCells) {
        path.push_back(gridToWorld(cell));
    }

    /* Add target as final waypoint */
    path.push_back(target);

    return path;
}

bool PathPlanner::isCellBlocked(const Map& map, float robotRadius, int cellX, int cellY) const {
    const Vector2Dim position = gridToWorld({cellX, cellY});

    /* Disbale robot to close to the wall */
    const float wallMargin = robotRadius + additionalWallMargin;

    /* Check proximity to wall of the map */
    if ((position.x - wallMargin) < 0.0f || (position.y - wallMargin) < 0.0f ||
        (position.x + wallMargin) > map.getWidth() || (position.y + wallMargin) > map.getHeight()) {
        return true;
    }

    /* Check proximity of the cell to obstacles */
    for (const auto& obstacle : map.getObstacles()) {
        const float minDistance = robotRadius + obstacle.radius + obstacleAdditionalMargin;
        if (distanceSquared(position, obstacle.center) < (minDistance * minDistance)) {
            return true;
        }
    }

    return false;
}

GridCell PathPlanner::worldToGrid(const Vector2Dim& position) const {
    return {static_cast<int>(position.x / m_cellSize), static_cast<int>(position.y / m_cellSize)};
}

Vector2Dim PathPlanner::gridToWorld(const GridCell& cell) const {
    /* Take cell center */
    return {(static_cast<float>(cell.x) + 0.5f) * m_cellSize, (static_cast<float>(cell.y) + 0.5f) * m_cellSize};
}
