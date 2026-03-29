/**
 * @file config.cpp
 *
 * @brief Configuration loader implementation.
 *
 * @details
 * This file implements the JSON configuration loading logic for the
 * application.
 *
 * The JSON file is parsed and mapped into a Config structure.
 * If some fields are missing, default values are preserved.
 *
 * @par Expected JSON structure
 * @verbatim
 * {
 *   "window": {
 *     "width": 1000,
 *     "height": 800
 *   },
 *   "map": {
 *     "width": 1000.0,
 *     "height": 800.0,
 *     "obstacleCount": 10,
 *     "obstacleMinSize": 5.0,
 *     "obstacleMaxSize": 10.0,
 *     "randomSeed": 10
 *   }
 *   "robot": {
 *     "robotSize": 10.0,
 *     "robotLineDirectionSize": 10
 *   }
 * }
 * @endverbatim
 *
 * @version 1.0
 */

/* System includes */
#include <fstream>
#include <stdexcept>

/* Other includes */
#include <nlohmann/json.hpp>

/* Project includes */
#include "app/config.hpp"

/* Implementation */
Config Config::loadFromFile(const std::string& filePath) {
    /* Open input stream */
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Failed to open config file: " + filePath);
    }

    /* Create json object */
    nlohmann::json jsonData;
    inputFile >> jsonData;

    Config config;

    /* Update default config values if existing */
    if (jsonData.contains("window")) {
        const auto& window = jsonData.at("window");

        if (window.contains("width")) {
            config.windowWidth = window.at("width").get<unsigned int>();
        }

        if (window.contains("height")) {
            config.windowHeight = window.at("height").get<unsigned int>();
        }
    }

    if (jsonData.contains("map")) {
        const auto& map = jsonData.at("map");

        if (map.contains("width")) {
            config.mapWidth = map.at("width").get<float>();
        }

        if (map.contains("height")) {
            config.mapHeight = map.at("height").get<float>();
        }

        if (map.contains("obstacleCount")) {
            config.obstacleCount = map.at("obstacleCount").get<std::size_t>();
        }

        if (map.contains("obstacleMinSize")) {
            config.obstacleMinSize = map.at("obstacleMinSize").get<float>();
        }

        if (map.contains("obstacleMaxSize")) {
            config.obstacleMaxSize = map.at("obstacleMaxSize").get<float>();
        }

        if (map.contains("randomSeed")) {
            config.randomSeed = map.at("randomSeed").get<unsigned int>();
        }
    }

    if (jsonData.contains("robot")) {
        const auto& robot = jsonData.at("robot");

        if (robot.contains("size")) {
            config.robotSize = robot.at("size").get<float>();
        }

        if (robot.contains("robotLineDirectionSize")) {
            config.robotLineDirectionSize = robot.at("robotLineDirectionSize").get<float>();
        }
    }

    return config;
}
