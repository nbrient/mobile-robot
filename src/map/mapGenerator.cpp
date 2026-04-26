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

void MapGenerator::generateNewObstacle(Map& map, const Config& config, const Vector2Dim& robotPosition,
                                       const float robotRadius) {
    map.clearObstacles();

    /* Create random floating point */
    std::uniform_real_distribution<float> xDist(0.0f, config.m_mapWidth);
    std::uniform_real_distribution<float> yDist(0.0f, config.m_mapHeight);
    std::uniform_real_distribution<float> radiusDist(config.m_obstacleMinSize, config.m_obstacleMaxSize);

    std::size_t created = 0u;
    std::size_t attempts = 0u;
    const std::size_t maxAttempts = config.m_obstacleCount * 10u;

    while (created < config.m_obstacleCount && attempts < maxAttempts) {
        attempts++;

        Obstacle obstacle;
        /* Create random obstacle */
        obstacle.m_center = {xDist(m_rng), yDist(m_rng)};
        obstacle.m_radius = radiusDist(m_rng);

        /* Check if obstacle is valid */
        if (isObstacleValid(map, obstacle, robotPosition, robotRadius)) {
            map.addObstacle(obstacle);
            created++;
        }
    }
}

bool MapGenerator::isObstacleValid(const Map& map, const Obstacle& obstacle, const Vector2Dim& robotPosition,
                                   const float robotRadius) const {
    /* Check if obstacle is in the map */
    if (obstacle.m_center.x - obstacle.m_radius < 0.0f || obstacle.m_center.x + obstacle.m_radius > map.getWidth()) {
        return false;
    }
    if (obstacle.m_center.y - obstacle.m_radius < 0.0f || obstacle.m_center.y + obstacle.m_radius > map.getHeight()) {
        return false;
    }

    /* Check obstacle not touching an other obstacle */
    for (const auto& other : map.getObstacles()) {
        const float minDist = obstacle.m_radius + other.m_radius + 10.0f;

        if (distanceSquared(obstacle.m_center, other.m_center) < (minDist * minDist)) {
            return false;
        }
    }

    const float minDist = obstacle.m_radius + robotRadius + 10.0f;

    if (distanceSquared(obstacle.m_center, robotPosition) < (minDist * minDist)) {
        return false;
    }

    return true;
}
