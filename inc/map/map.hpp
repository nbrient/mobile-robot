#ifndef MAP_MAP_HPP
#define MAP_MAP_HPP

/**
 * @file map.hpp
 *
 * @brief Map container declaration.
 *
 * @details This file declares the Map class, which stores the 2D environment dimensions and the obstacle list.
 *
 * @version 1.0
 */

/* System includes */
#include <vector>

/* Project includes */
#include "map/obstacle.hpp"

/* Declarations */

/**
 * @brief 2D environment container.
 *
 * @details
 * The map stores:
 * - its width
 * - its height
 * - the set of generated circular obstacles
 */
class Map {
public:
    /**
     * @brief Construct a map with fixed dimensions.
     *
     * @param width Map width.
     * @param height Map height.
     */
    Map(float width, float height);

    /**
     * @brief Remove all obstacles from the map.
     */
    void clearObstacles();

    /**
     * @brief Add one obstacle to the map.
     *
     * @param obstacle Obstacle to insert.
     */
    void addObstacle(const Obstacle& obstacle);

    /**
     * @brief Get the map width.
     *
     * @return Map width.
     */
    float getWidth() const;

    /**
     * @brief Get the map height.
     *
     * @return Map height.
     */
    float getHeight() const;

    /**
     * @brief Get the obstacle list.
     *
     * @return Constant reference to the obstacle vector.
     */
    const std::vector<Obstacle>& getObstacles() const;

private:
    /**
     * @brief Map width.
     */
    float m_width;

    /**
     * @brief Map height.
     */
    float m_height;

    /**
     * @brief Stored obstacles.
     */
    std::vector<Obstacle> m_obstacles;
};

#endif
