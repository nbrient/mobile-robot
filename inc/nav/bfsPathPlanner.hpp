#ifndef NAV_BFS_PATH_PLANNER_HPP
#define NAV_BFS_PATH_PLANNER_HPP

/**
 * @file bfsPathPlanner.hpp
 *
 * @brief BFS path planner declaration.
 *
 * @details
 * This file declares a simple BFS planner operating on a 2D grid built from
 * the current map and robot radius.
 *
 * @version 1.0
 */

/* System includes */
#include <vector>

/* Project includes */
#include "common/common.hpp"
#include "map/map.hpp"
#include "nav/pathPlanner.hpp"

/* Declarations */

/**
 * @brief Simple BFS path planner on a 2D grid.
 */
class BfsPathPlanner : public PathPlanner {
public:
    /**
     * @brief Construct a BFS planner with a fixed cell size.
     *
     * @param cellSize Grid cell size in world units.
     */
    explicit BfsPathPlanner(float cellSize);

    /**
     * @brief Compute a path from start to target.
     *
     * @param map Current map.
     * @param robotRadius Current robot radius.
     * @param start Start world position.
     * @param target Target world position.
     *
     * @return Sequence of waypoints.
     */
    std::vector<Vector2Dim> computePath(const Map& map, float robotRadius, const Vector2Dim& start,
                                        const Vector2Dim& target) const override;
};

#endif
