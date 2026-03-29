#ifndef GRAPHICS_RENDERER_HPP
#define GRAPHICS_RENDERER_HPP

/**
 * @file renderer.hpp
 *
 * @brief Rendering module declaration.
 *
 * @details This file declares the Renderer class used to draw the map and its obstacles using SFML.
 *
 * @version 1.0
 */

/* Other includes */
#include <SFML/Graphics/RenderWindow.hpp>

/* Project includes */
#include "entity/robot.hpp"
#include "map/map.hpp"

/* Declarations */

/**
 * @brief Rendering helper.
 *
 * @details
 * The renderer is responsible for converting map data into 2D map.
 */
class Renderer {
public:
    /**
     * @brief Render the map totally.
     *
     * @param window Target render window.
     * @param map Map to draw.
     * @param robot Robot to draw.
     */
    void renderMap(sf::RenderWindow& window, const Map& map, const Robot& robot);

private:
    /**
     * @brief Draw the map.
     *
     * @param window Target render window.
     * @param map Map to draw.
     */
    void drawMap(sf::RenderWindow& window, const Map& map);

    /**
     * @brief Draw all obstacles.
     *
     * @param window Target render window.
     * @param map Map containing obstacles.
     */
    void drawObstacles(sf::RenderWindow& window, const Map& map);

    /**
     * @brief Draw the robot.
     *
     * @param window Target render window.
     * @param robot Robot to draw.
     */
    void drawRobot(sf::RenderWindow& window, const Robot& robot);
};

#endif
