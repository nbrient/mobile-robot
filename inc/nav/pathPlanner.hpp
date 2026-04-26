#ifndef NAV_PATH_PLANNER_HPP
#define NAV_PATH_PLANNER_HPP

/**
 * @file pathPlanner.hpp
 *
 * @brief Common path planner base class declaration.
 *
 * @details
 * This file declares the PathPlanner base class used to share common grid
 * utilities between several path planning algorithms.
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
 * @brief Common grid based path planner.
 *
 * @details
 * This class stores the grid cell size and provides helper methods used by
 * derived path planners. The path search algorithm itself is implemented by
 * child classes.
 */
class PathPlanner {
public:
    /**
     * @brief Construct a path planner with a fixed grid cell size.
     *
     * @param cellSize Grid cell size in world units.
     */
    explicit PathPlanner(float cellSize);

    /**
     * @brief Virtual destructor.
     */
    virtual ~PathPlanner() = default;

    /**
     * @brief Compute a path from start position to target position.
     *
     * @param map Current map.
     * @param robotRadius Current robot radius.
     * @param start Start world position.
     * @param target Target world position.
     *
     * @return Sequence of waypoints in world coordinates.
     */
    virtual std::vector<Vector2Dim> computePath(const Map& map, float robotRadius, const Vector2Dim& start,
                                                const Vector2Dim& target) const = 0;

protected:
    /**
     * @brief Check if a grid cell is blocked for the robot.
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

protected:
    /**
     * @brief Grid cell size in world units.
     */
    float m_cellSize;
};

#endif
