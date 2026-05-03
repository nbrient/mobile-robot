#ifndef COMMON_HPP
#define COMMON_HPP

/**
 * @file common.hpp
 *
 * @brief Common code.
 *
 * @details This file declares data structures and functions shared across multiple modules of the application.
 *
 * @version 1.0
 */

/* Declarations */

/**
 * @brief Simple 2D vector.
 *
 * @details
 * Used to represent positions in the map.
 */
struct Vector2Dim {
    /**
     * @brief Horizontal coordinate.
     */
    float x{0.0f};

    /**
     * @brief Vertical coordinate.
     */
    float y{0.0f};
};

/**
 * @brief Grid cell coordinate.
 */
struct GridCell {
    int x;
    int y;
};

/**
 * @brief Supported path planner types.
 */
enum class PathPlannerType { BFS, A_STAR };

/**
 * @brief Compute squared distance between two points.
 *
 * @param first First point.
 * @param second Second point.
 *
 * @return Squared distance.
 */
inline float distanceSquared(const Vector2Dim& first, const Vector2Dim& second) {
    const float dx = first.x - second.x;
    const float dy = first.y - second.y;

    return (dx * dx) + (dy * dy);
}

#endif
