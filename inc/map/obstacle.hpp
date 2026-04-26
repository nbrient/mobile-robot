#ifndef MAP_OBSTACLE_HPP
#define MAP_OBSTACLE_HPP

/**
 * @file obstacle.hpp
 *
 * @brief Obstacle data declaration.
 *
 * @details This file defines the Obstacle structure used to represent circular obstacles inside the map.
 *
 * @version 1.0
 */

/* Project includes */
#include "common/common.hpp"

/* Declarations */

/**
 * @brief Circular obstacle description.
 *
 * @details
 * Obstacles are defined by a center position and a radius.
 */
struct Obstacle {
    /**
     * @brief Center position of the obstacle.
     */
    Vector2Dim m_center;

    /**
     * @brief Radius of the obstacle.
     */
    float m_radius{20.0f};
};

#endif
