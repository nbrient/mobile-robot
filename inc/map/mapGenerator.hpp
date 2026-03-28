#ifndef MAP_MAPGENERATOR_HPP
#define MAP_MAPGENERATOR_HPP

/**
 * @file mapGenerator.hpp
 *
 * @brief Procedural map generator declaration.
 *
 * @details This file declares the MapGenerator class, responsible for creating a map with randomly positioned
 * obstacles.
 *
 * @version 1.0
 */

/* System includes */
#include <random>

/* Project includes */
#include "app/config.hpp"
#include "map/map.hpp"

/* Declarations */

/**
 * @brief Obstacle generator.
 *
 * @details This class generate circular obstacles according to the parameter in the Config structure.
 */
class MapGenerator {
public:
    /**
     * @brief Construct a generator with a fixed seed.
     *
     * @param seed Seed used by the pseudo-random engine.
     */
    explicit MapGenerator(unsigned int seed);

    /**
     * @brief Generate a new obstacle set for the given map.
     *
     * @param map Map to populate.
     * @param config Generation parameters.
     */
    void generateNewObstacle(Map& map, const Config& config);

private:
    /**
     * @brief Check if an obstacle can be inserted in the map.
     *
     * @param map Current map state.
     * @param obstacle Candidate obstacle.
     *
     * @return True if the obstacle is valid, false otherwise.
     */
    bool isObstacleValid(const Map& map, const Obstacle& obstacle) const;

private:
    /**
     * @brief Pseudo-random engine.
     */
    std::mt19937 m_rng;
};

#endif