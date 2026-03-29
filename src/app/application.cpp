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
Application::Application(const Config& config)
    : m_config(config),
      m_window(sf::VideoMode({config.windowWidth, config.windowHeight}), "Robot Map - First version"),
      m_map(config.mapWidth, config.mapHeight),
      m_robot(config.robotSize, config.robotLineDirectionSize),
      m_mapGenerator(config.randomSeed) {
    m_window.setFramerateLimit(60);
    m_mapGenerator.generateNewObstacle(m_map, m_config);
}

void Application::run() {
    sf::Event event;
    float dt = 1.0f / 60.0f;

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

        m_robot.update(dt);

        /* Update window */
        m_window.clear();
        m_renderer.renderMap(m_window, m_map, m_robot);
        m_window.display();
    }
}
