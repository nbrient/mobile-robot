#ifndef APP_APPLICATION_HPP
#define APP_APPLICATION_HPP

/**
 * @file application.hpp
 *
 * @brief Main application class declaration.
 *
 * @details
 * This file declares the Application class, which owns the main window, the generated map and the rendering pipeline.
 *
 * The class is responsible for:
 * - window creation
 * - map generation at startup
 * - main loop executin
 * - event processing
 * - rendering in HMI
 *
 * @version 1.0
 */

/* Other includes */
#include <SFML/Graphics/RenderWindow.hpp>
#include <random>

/* Project includes */
#include "app/config.hpp"
#include "entity/target.hpp"
#include "graphics/renderer.hpp"
#include "map/map.hpp"
#include "map/mapGenerator.hpp"

/* Declarations */

/**
 * @brief Main application.
 *
 * @details
 * This class centralizes the program execution and contains the objects
 * required to run the simulation window.
 */
class Application {
public:
    /**
     * @brief Construct the application from a config.
     *
     * @param config Application configuration.
     */
    explicit Application(const Config& config);

    /**
     * @brief Run the application.
     */
    void run();

private:
    /**
     * @brief Application configuration.
     */
    Config m_config;

    /**
     * @brief Rendering window.
     */
    sf::RenderWindow m_window;

    /**
     * @brief Current map.
     */
    Map m_map;

    /**
     * @brief Map generator used to create the map.
     */
    MapGenerator m_mapGenerator;

    /**
     * @brief Rendering helper.
     */
    Renderer m_renderer;

    /**
     * @brief Robot instance.
     */
    Robot m_robot;

    /**
     * @brief Robot instance.
     */
    Target m_target;

    /**
     * @brief Radom number generator.
     */
    std::mt19937 m_rng;

    /**
     * @brief Check if a robot candidate position is valid.
     *
     * @param candidatePosition Candidate robot center position.
     *
     * @return True if the position is valid, flse otherwise.
     */
    bool isRobotPositionValid(const Vector2Dim& candidatePosition) const;

    /**
     * @brief Generate the target position.
     */
    void generateTargetPosition();

    /**
     * @brief Check if the target position is valid.
     */
    bool isTargetPositionValid(const Vector2Dim& candidatePosition) const;

    /**
     * @brief Check if the target has been reached.
     */
    bool isTargetReached() const;
};

#endif
