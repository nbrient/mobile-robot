#ifndef COMMON_TYPES_HPP
#define COMMON_TYPES_HPP

/**
 * @file types.hpp
 *
 * @brief Common shared types.
 *
 * @details This file declares data structures shared across multiple modules of the application.
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

#endif