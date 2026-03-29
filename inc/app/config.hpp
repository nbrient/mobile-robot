#ifndef APP_CONFIG_HPP
#define APP_CONFIG_HPP
/**
 * @file config.hpp
 *
 * @brief Application configuration declaration.
 *
 * @details This file declares the Config structure used to store parameters required at application startup.
 *
 *
 * The configuration can be loaded from a JSON file.
 *
 * @version 1.0
 */

/* System includes */
#include <cstddef>
#include <string>

/* Declarations */

/**
 * @brief Application configuration container.
 *
 * @details This structure stores runtime parameters used by the application to initialize the window and generate the
 * map.
 */
struct Config {
    /**
     * @brief Window width in pixels.
     */
    unsigned int windowWidth{1000u};

    /**
     * @brief Window height in pixels.
     */
    unsigned int windowHeight{800u};

    /**
     * @brief Map width in world units.
     */
    float mapWidth{1000.0f};

    /**
     * @brief Map height in world units.
     */
    float mapHeight{800.0f};

    /**
     * @brief Number of obstacles to generate.
     */
    std::size_t obstacleCount{10u};

    /**
     * @brief Minimum size of generated obstacles.
     */
    float obstacleMinSize{5.0f};

    /**
     * @brief Maximum size of generated obstacles.
     */
    float obstacleMaxSize{10.0f};

    /**
     * @brief Seed used by the random generator.
     */
    unsigned int randomSeed{10u};

    /**
     * @brief Robot size.
     */
    float robotSize{10.0f};

    /**
     * @brief Length of the robot direction line.
     */
    float robotLineDirectionSize{10.0f};

    /**
     * @brief Load configuration from a JSON file.
     *
     * @details Missing JSON fields keep their default values.
     *
     * @param filePath Path to the JSON configuration file.
     *
     * @return Loaded configuration structure.
     *
     * @throw std::runtime_error If the file cannot be opened.
     */
    static Config loadFromFile(const std::string& filePath);
};

#endif
