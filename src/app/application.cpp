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

/* Implementation */
Application::Application(const Config& config)
    : m_config(config),
      m_window(sf::VideoMode({config.windowWidth, config.windowHeight}), "Robot Map - First version"),
      m_map(config.mapWidth, config.mapHeight),
      m_mapGenerator(config.randomSeed) {
    m_window.setFramerateLimit(60);
    m_mapGenerator.generateNewObstacle(m_map, m_config);
}

void Application::run() {
    sf::Event event;

    /* While window open */
    while (m_window.isOpen()) {
        /* Get event on top of the event queue */
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
            }
        }

        /* Update window */
        m_window.clear();
        m_renderer.renderMap(m_window, m_map);
        m_window.display();
    }
}