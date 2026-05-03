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
 *   },
 *   "robot": {
 *     "robotSize": 10.0,
 *     "robotLineDirectionSize": 15.0,
 *     "defaultX" : 100.0,
 *     "defaultY" : 100.0
 *   },
 *   "target": {
 *     "size": 10.0
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
            config.m_windowWidth = window.at("width").get<unsigned int>();
        }

        if (window.contains("height")) {
            config.m_windowHeight = window.at("height").get<unsigned int>();
        }
    }

    if (jsonData.contains("map")) {
        const auto& map = jsonData.at("map");

        if (map.contains("width")) {
            config.m_mapWidth = map.at("width").get<float>();
        }

        if (map.contains("height")) {
            config.m_mapHeight = map.at("height").get<float>();
        }

        if (map.contains("obstacleCount")) {
            config.m_obstacleCount = map.at("obstacleCount").get<std::size_t>();
        }

        if (map.contains("obstacleMinSize")) {
            config.m_obstacleMinSize = map.at("obstacleMinSize").get<float>();
        }

        if (map.contains("obstacleMaxSize")) {
            config.m_obstacleMaxSize = map.at("obstacleMaxSize").get<float>();
        }

        if (map.contains("randomSeed")) {
            config.m_randomSeed = map.at("randomSeed").get<unsigned int>();
        }
    }

    if (jsonData.contains("robot")) {
        const auto& robot = jsonData.at("robot");

        if (robot.contains("size")) {
            config.m_robotSize = robot.at("size").get<float>();
        }

        if (robot.contains("robotLineDirectionSize")) {
            config.m_robotLineDirectionSize = robot.at("robotLineDirectionSize").get<float>();
        }

        if (robot.contains("defaultX")) {
            config.m_defaultRobotX = robot.at("defaultX").get<float>();
        }

        if (robot.contains("defaultY")) {
            config.m_defaultRobotY = robot.at("defaultY").get<float>();
        }
    }

    if (jsonData.contains("target")) {
        const auto& target = jsonData.at("target");

        if (target.contains("size")) {
            config.m_targetSize = target.at("size").get<float>();
        }
    }

    if (jsonData.contains("pathPlannerType")) {
        const std::string& plannerTypeString = jsonData.at("pathPlannerType").get<std::string>();
        if (plannerTypeString == "A_STAR") {
            config.m_pathPlannerType = PathPlannerType::A_STAR;
        } else {
            config.m_pathPlannerType = PathPlannerType::BFS;
        }
    } else {
        config.m_pathPlannerType = PathPlannerType::BFS;
    }

    return config;
}
