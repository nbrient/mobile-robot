/**
 * @file renderer.cpp
 *
 * @brief Rendering module implementation.
 *
 * @details
 * This file implements all drawing operations for the map view.
 *
 * The rendering sequence is:
 * - draw the map background
 * - draw all generated obstacles
 *
 * @version 1.0
 */

/* Other includes */
#include <SFML/Graphics.hpp>

/* Project includes */
#include "graphics/renderer.hpp"

/* Implementation */
void Renderer::renderMap(sf::RenderWindow& window, const Map& map) {
    /* Create and draw map */
    drawMap(window, map);
    /* Create and draw obstacles */
    drawObstacles(window, map);
}

void Renderer::drawMap(sf::RenderWindow& window, const Map& map) {
    sf::RectangleShape rect({map.getWidth(), map.getHeight()});
    rect.setPosition({0.0f, 0.0f});
    rect.setFillColor(sf::Color(30, 30, 30));
    rect.setOutlineThickness(2.0f);
    rect.setOutlineColor(sf::Color::White);

    window.draw(rect);
}

void Renderer::drawObstacles(sf::RenderWindow& window, const Map& map) {
    for (const auto& obstacle : map.getObstacles()) {
        sf::CircleShape shape(obstacle.radius);
        shape.setOrigin({obstacle.radius, obstacle.radius});
        shape.setPosition({obstacle.center.x, obstacle.center.y});
        shape.setFillColor(sf::Color(180, 70, 70));

        window.draw(shape);
    }
}