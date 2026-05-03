#ifndef NAV_A_STAR_PATH_PLANNER_HPP
#define NAV_A_STAR_PATH_PLANNER_HPP

/**
 * @file aStarPathPlanner.hpp
 *
 * @brief A star path planner declaration.
 *
 * @details
 * This file declares an A* planner operating on the same 2D grid as the BFS planner.
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
 * @brief A* path planner on a 2D grid.
 *
 * @details
 * The planner uses a Manhattan distance heuristic with a 4-connected grid, so it keeps the same movement model as the
 * BFS planner while reducing the number of explored cells.
 */
class AStarPathPlanner : public PathPlanner {
public:
    /**
     * @brief Construct an A* planner with a fixed cell size.
     *
     * @param cellSize Grid cell size in world units.
     */
    explicit AStarPathPlanner(float cellSize);

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
