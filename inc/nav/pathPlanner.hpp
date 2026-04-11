#ifndef NAVIGATION_PATHPLANNER_HPP
#define NAVIGATION_PATHPLANNER_HPP

/**
 * @file pathPlanner.hpp
 *
 * @brief Path planner declaration.
 *
 * @details This file declares a very simple BFS planner operating on a 2D grid built from the current map and robot
 * radius.
 *
 * @version 1.0
 */

/* System includes */
#include <vector>

/* Project includes */
#include "common/common.hpp"
#include "map/map.hpp"

/* Declarations */

/**
 * @brief Grid cell coordinate.
 */
struct GridCell {
    int x;
    int y;
};

/**
 * @brief Simple BFS path planner on a 2D grid.
 */
class PathPlanner {
public:
    /**
     * @brief Construct a planner with a fixed cell size.
     *
     * @param cellSize Grid cell size in world units.
     */
    explicit PathPlanner(float cellSize);

    /**
     * @brief Compute a path from start to goal.
     *
     * @param map Current map.
     * @param robotRadius Current robot radius
     * @param start Start world position.
     * @param goal Goal world position.
     *
     * @return Sequence of waypoints.
     */
    std::vector<Vector2Dim> computePath(const Map& map, float robotRadius, const Vector2Dim& start,
                                        const Vector2Dim& goal) const;

private:
    /**
     * @brief Check if a grid cell is blocked.
     *
     * @param map Current map.
     * @param robotRadius Current robot radius.
     * @param cellX Coordinate x of cell.
     * @param cellY Coordinate y of cell.
     *
     * @return True if blocked, false otherwise.
     */
    bool isCellBlocked(const Map& map, float robotRadius, int cellX, int cellY) const;

    /**
     * @brief Convert world coordinates to grid coordinates.
     *
     * @param position World position.
     *
     * @return Converted grid cell.
     */
    GridCell worldToGrid(const Vector2Dim& position) const;

    /**
     * @brief Convert grid coordinates to world coordinates.
     *
     * @param cell Grid cell.
     *
     * @return Center of the cell in world coordinates.
     */
    Vector2Dim gridToWorld(const GridCell& cell) const;

private:
    /**
     * @brief Grid cell size.
     */
    float m_cellSize;
};

#endif
