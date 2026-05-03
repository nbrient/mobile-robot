/**
 * @file pathPlanner.cpp
 *
 * @brief Common grid-based path planner implementation.
 *
 * @details
 * This file implements the grid utilities shared by all path planners.
 *
 * @version 1.0
 */

/* Project includes */
#include "nav/pathPlanner.hpp"

/* Implementation */
namespace {

/**
 * @brief Additional margin for wall.
 *
 */
static constexpr float ADDITIONAL_WALL_MARGIN = 4.0f;

/**
 * @brief Additional margin used to keep the robot away from obstacles.
 */
static constexpr float OBSTACLE_ADDITIONAL_MARGIN = 8.0f;

}  // namespace

/* Implementation */
PathPlanner::PathPlanner(float cellSize) : m_cellSize(cellSize) {
}

bool PathPlanner::isCellBlocked(const Map& map, float robotRadius, int cellX, int cellY) const {
    const Vector2Dim position = gridToWorld({cellX, cellY});

    /* Disbale robot to close to the wall */
    const float wallMargin = robotRadius + ADDITIONAL_WALL_MARGIN;

    /* Check proximity to wall of the map */
    if ((position.x - wallMargin) < 0.0f || (position.y - wallMargin) < 0.0f ||
        (position.x + wallMargin) > map.getWidth() || (position.y + wallMargin) > map.getHeight()) {
        return true;
    }

    /* Check proximity of the cell to obstacles */
    for (const auto& obstacle : map.getObstacles()) {
        const float minDistance = robotRadius + obstacle.m_radius + OBSTACLE_ADDITIONAL_MARGIN;

        if (distanceSquared(position, obstacle.m_center) < (minDistance * minDistance)) {
            return true;
        }
    }

    return false;
}

GridCell PathPlanner::worldToGrid(const Vector2Dim& position) const {
    return {static_cast<int>(position.x / m_cellSize), static_cast<int>(position.y / m_cellSize)};
}

Vector2Dim PathPlanner::gridToWorld(const GridCell& cell) const {
    /* Take cell m_center */
    return {(static_cast<float>(cell.x) + 0.5f) * m_cellSize, (static_cast<float>(cell.y) + 0.5f) * m_cellSize};
}

bool PathPlanner::areCellsEqual(const GridCell& left, const GridCell& right) const {
    return (left.x == right.x) && (left.y == right.y);
}
