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
      m_target(config.targetSize),
      m_rng(config.randomSeed + 123u),
      m_mapGenerator(config.randomSeed) {
    m_window.setFramerateLimit(framerateLimit);
    m_mapGenerator.generateNewObstacle(m_map, m_config);
    generateTargetPosition();
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

        /* Generate new target if target reached */
        if (isTargetReached()) {
            std::printf("Target reached\n");
            generateTargetPosition();
        }

        /* Update window */
        m_window.clear();
        m_renderer.renderMap(m_window, m_map, m_robot, m_target);
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

void Application::generateTargetPosition() {
    /* Generate random coordinates */
    std::uniform_real_distribution<float> xDist(m_target.getRadius(), m_map.getWidth() - m_target.getRadius());
    std::uniform_real_distribution<float> yDist(m_target.getRadius(), m_map.getHeight() - m_target.getRadius());

    for (int attempt = 0; attempt < 500; ++attempt) {
        const Vector2Dim candidatePosition = {xDist(m_rng), yDist(m_rng)};

        if (isTargetPositionValid(candidatePosition)) {
            m_target.setPosition(candidatePosition);
            return;
        }
    }

    /* If no valid position founded */
    m_target.setPosition({m_map.getWidth() - m_target.getRadius(), m_map.getHeight() - m_target.getRadius()});
}

bool Application::isTargetPositionValid(const Vector2Dim& candidatePosition) const {
    /* Check target no too close to the robot */
    const float minDistanceToRobot = m_robot.getRadius() + m_target.getRadius() + 50.0f;

    /* Check if target touching robot */
    if (distanceSquared(candidatePosition, m_robot.getPosition()) < (minDistanceToRobot * minDistanceToRobot)) {
        return false;
    }

    /* Check if target touching an obstacle */
    for (const auto& obstacle : m_map.getObstacles()) {
        const float collisionDistance = m_target.getRadius() + obstacle.radius;

        if (distanceSquared(candidatePosition, obstacle.center) < (collisionDistance * collisionDistance)) {
            return false;
        }
    }

    return true;
}

bool Application::isTargetReached() const {
    const float reachDistance = m_robot.getRadius() + m_target.getRadius();

    return distanceSquared(m_robot.getPosition(), m_target.getPosition()) < (reachDistance * reachDistance);
}
