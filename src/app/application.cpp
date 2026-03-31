/**
 * @file application.cpp
 *
 * @brief Main application implementation.
 *
 * @details This file implements the Application class.
 *
 *
 * @verbatim
 * while application is running:
 *     poll window events
 *     clear frame buffer
 *     render map
 *     display frame
 * @endverbatim
 *
 * @version 1.0
 */

/* Other includes */
#include <SFML/Window/Event.hpp>
#include <optional>

/* Project includes */
#include "app/application.hpp"
#include "entity/robot.hpp"

/* Implementation */
const uint8_t framerateLimit{60u};

Application::Application(const Config& config)
    : m_config(config),
      m_window(sf::VideoMode({config.windowWidth, config.windowHeight}), "Robot Map - First version"),
      m_map(config.mapWidth, config.mapHeight),
      m_robot(config.robotSize, config.robotLineDirectionSize),
      m_mapGenerator(config.randomSeed) {
    m_window.setFramerateLimit(framerateLimit);
    m_mapGenerator.generateNewObstacle(m_map, m_config);
}

void Application::run() {
    sf::Event event;
    float dt = 1.0f / static_cast<float>(framerateLimit);

    /* While window open */
    while (m_window.isOpen()) {
        /* Get event on top of the event queue */
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
            }
        }

        m_robot.setForward(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z));

        m_robot.setBackward(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
                            sf::Keyboard::isKeyPressed(sf::Keyboard::S));

        m_robot.setTurnLeft(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
                            sf::Keyboard::isKeyPressed(sf::Keyboard::Q));

        m_robot.setTurnRight(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
                             sf::Keyboard::isKeyPressed(sf::Keyboard::D));

        Vector2Dim candidatePosition = m_robot.computeNextPosition(dt);

        if (isRobotPositionValid(candidatePosition)) {
            m_robot.setPosition(candidatePosition);
        }

        /* Update window */
        m_window.clear();
        m_renderer.renderMap(m_window, m_map, m_robot);
        m_window.display();
    }
}

bool Application::isRobotPositionValid(const Vector2Dim& candidatePosition) const {
    const float robotRadius = m_robot.getRadius();

    /* Check map range */
    if ((candidatePosition.x - robotRadius) < 0.0f) {
        return false;
    }

    if ((candidatePosition.y - robotRadius) < 0.0f) {
        return false;
    }

    if ((candidatePosition.x + robotRadius) > m_map.getWidth()) {
        return false;
    }

    if ((candidatePosition.y + robotRadius) > m_map.getHeight()) {
        return false;
    }

    /* Check robot collison with obstacles */
    for (const auto& obstacle : m_map.getObstacles()) {
        const float collisionDistance = robotRadius + obstacle.radius;

        /* No need to calculate simple distance, comparison on squared if more efficient */
        if (distanceSquared(candidatePosition, obstacle.center) < (collisionDistance * collisionDistance)) {
            return false;
        }
    }

    return true;
}
