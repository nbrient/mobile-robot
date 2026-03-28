/**
 * @file mapGenerator.cpp
 *
 * @brief Procedural map generator implementation.
 *
 * @details
 * This file generates a 2D environment filled with circular obstacles.
 *
 * Obstacles are created using random positions and radii, then validated
 * before insertion.
 *
 * @par Generation strategy
 * @verbatim
 * for each requested obstacle:
 *     generate random center
 *     generate random radius
 *     validate obstacle boundaries
 *     validate obstacle overlap
 *     insert if valid
 * @endverbatim
 *
 * @par Constraints
 * - Obstacles must stay inside the map
 * - Obstacles must not overlap
 * - Generation attempts are limited
 *
 * @version 1.0
 */

/* Project includes */
#include "map/mapGenerator.hpp"

/* Implementation */
MapGenerator::MapGenerator(unsigned int seed) : m_rng(seed) {
}

void MapGenerator::generateNewObstacle(Map& map, const Config& config) {
    map.clearObstacles();

    /* Create random floating point */
    std::uniform_real_distribution<float> xDist(0.0f, config.mapWidth);
    std::uniform_real_distribution<float> yDist(0.0f, config.mapHeight);
    std::uniform_real_distribution<float> radiusDist(config.obstacleMinSize, config.obstacleMaxSize);

    std::size_t created = 0u;
    std::size_t attempts = 0u;
    const std::size_t maxAttempts = config.obstacleCount * 10u;

    while (created < config.obstacleCount && attempts < maxAttempts) {
        attempts++;

        Obstacle obstacle;
        /* Create random obstacle */
        obstacle.center = {xDist(m_rng), yDist(m_rng)};
        obstacle.radius = radiusDist(m_rng);

        /* Check if obstacle is valid */
        if (isObstacleValid(map, obstacle)) {
            map.addObstacle(obstacle);
            created++;
        }
    }
}

bool MapGenerator::isObstacleValid(const Map& map, const Obstacle& obstacle) const {
    /* Check if obstacle is in the map */
    if (obstacle.center.x - obstacle.radius < 0.0f || obstacle.center.x + obstacle.radius > map.getWidth()) {
        return false;
    }
    if (obstacle.center.y - obstacle.radius < 0.0f || obstacle.center.y + obstacle.radius > map.getHeight()) {
        return false;
    }

    /* Check obstacle not touching an other obstacle */
    for (const auto& other : map.getObstacles()) {
        const float dx = obstacle.center.x - other.center.x;
        const float dy = obstacle.center.y - other.center.y;
        const float minDist = obstacle.radius + other.radius + 10.0f;

        if ((dx * dx + dy * dy) < (minDist * minDist)) {
            return false;
        }
    }

    return true;
}