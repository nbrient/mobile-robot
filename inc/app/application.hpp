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
#include <vector>

/* Project includes */
#include "app/config.hpp"
#include "common/mode.hpp"
#include "entity/robot.hpp"
#include "entity/target.hpp"
#include "graphics/hud.hpp"
#include "graphics/renderer.hpp"
#include "map/map.hpp"
#include "map/mapGenerator.hpp"
#include "nav/pathPlanner.hpp"

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

    /**
     * @brief Reset the map and target.
     * Triggered by pressing "r"
     *
     */
    void regenerateMapAndTarget();

    /**
     * @brief Handle single key pressed actions.
     *
     * Managed keys:
     * - Escape: close application
     * - P: toggle pause/manual mode
     * - R: regenerate map and target
     * - T: regenerate target only
     * - M: pass to manual/auto mode
     *
     * @param event SFML event received from the window.
     */
    void handleSingleKeyActions(const sf::Event& event);

    /**
     * @brief Recompute the current path from robot to target.
     */
    void recomputePath();

    /**
     * @brief Update robot behavior in auto mode by following waypoints.
     *
     * @details
     * The robot follows intermediate waypoints from the planner, but the final
     * objective is always the exact target position.
     *
     * Intermediate waypoints can be skipped when reached.
     * The last waypoint is never fully consumed until the real target is reached.
     *
     * @param dt Time step in seconds.
     */
    void updateAutoMode(float dt);

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
     * @brief Game mode.
     */
    Mode m_mode;

    /**
     * @brief Target reached counter.
     */
    int m_targetReachedCount;

    /**
     * @brief Display flag.
     */
    bool m_showTargetReached;

    /**
     * @brief Counter to display 'target reached' message.
     */
    float m_targetReachedTimer;

    /**
     * @brief Hud instance.
     */
    Hud m_hud;

    /**
     * @brief Grid-based path planner.
     */
    PathPlanner m_pathPlanner;

    /**
     * @brief Current sequence of waypoints.
     */
    std::vector<Vector2Dim> m_currentPath;

    /**
     * @brief Current waypoint index in the path.
     */
    std::size_t m_currentWayPointIndex;
};

#endif
