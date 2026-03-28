/**
 * @file map.cpp
 *
 * @brief Map container implementation.
 *
 * @details
 * This file implements the Map class, which stores environment dimensions
 * and the obstacle list used by the renderer and generator.
 *
 * @version 1.0
 */

/* Project includes */
#include "map/map.hpp"

/* Implementation */
Map::Map(float width, float height) : m_width(width), m_height(height) {
}

void Map::clearObstacles() {
    /* Clear obstacle vector */
    m_obstacles.clear();
}

void Map::addObstacle(const Obstacle& obstacle) {
    /* Add obstacle to the vector */
    m_obstacles.push_back(obstacle);
}

float Map::getWidth() const {
    return m_width;
}

float Map::getHeight() const {
    return m_height;
}

const std::vector<Obstacle>& Map::getObstacles() const {
    return m_obstacles;
}