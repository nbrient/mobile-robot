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
#include <cmath>

/* Project includes */
#include "graphics/renderer.hpp"

/* Implementation */
void Renderer::renderMap(sf::RenderWindow& window, const Map& map, const Robot& robot, const Target& target,
                         const std::vector<Vector2Dim>& path, std::size_t currentWayPointIndex, bool displayPath) {
    /* Create and draw map */
    drawMap(window, map);

    /* Create and draw obstacles */
    drawObstacles(window, map);

    /* Create and draw robot */
    drawRobot(window, robot);

    /* Create and draw target */
    drawTarget(window, target);

    /* Draw planned path */
    if (displayPath) {
        drawPath(window, path, currentWayPointIndex);
    }
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

void Renderer::drawRobot(sf::RenderWindow& window, const Robot& robot) {
    /* Circle shape */
    sf::CircleShape shape(robot.getRadius());
    shape.setOrigin(robot.getRadius(), robot.getRadius());
    shape.setPosition(robot.getPosition().x, robot.getPosition().y);
    shape.setFillColor(sf::Color::Blue);

    window.draw(shape);

    /* Show its direction */
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(robot.getPosition().x, robot.getPosition().y)),
        sf::Vertex(sf::Vector2f(robot.getPosition().x + std::cos(robot.getAngle()) * robot.getDirectionLineSize(),
                                robot.getPosition().y + std::sin(robot.getAngle()) * robot.getDirectionLineSize()))};
    window.draw(line, 2, sf::Lines);
}

void Renderer::drawTarget(sf::RenderWindow& window, const Target& target) {
    /* Circle shape */
    sf::CircleShape shape(target.getRadius());
    shape.setOrigin(target.getRadius(), target.getRadius());
    shape.setPosition(target.getPosition().x, target.getPosition().y);
    shape.setFillColor(sf::Color::Green);

    window.draw(shape);
}

void Renderer::drawPath(sf::RenderWindow& window, const std::vector<Vector2Dim>& path,
                        std::size_t currentWayPointIndex) {
    /* Nothing to draw if not enough waypoint */
    if (path.size() < 2U) {
        return;
    }

    /* Draw line segments and waypoints */
    for (std::size_t index = 0U; index < path.size(); ++index) {
        /* Draw waypoint */
        sf::CircleShape waypointShape(4.0f);
        waypointShape.setOrigin(4.0f, 4.0f);
        waypointShape.setPosition(path[index].x, path[index].y);

        if (index == currentWayPointIndex) {
            waypointShape.setFillColor(sf::Color::Cyan);
        } else {
            waypointShape.setFillColor(sf::Color::Yellow);
        }

        window.draw(waypointShape);

        /* Draw segment to next waypoint */
        if ((index + 1U) < path.size()) {
            sf::Vertex line[] = {sf::Vertex(sf::Vector2f(path[index].x, path[index].y), sf::Color::Yellow),
                                 sf::Vertex(sf::Vector2f(path[index + 1U].x, path[index + 1U].y), sf::Color::Yellow)};

            window.draw(line, 2, sf::Lines);
        }
    }
}
