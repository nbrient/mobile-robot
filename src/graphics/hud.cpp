/**
 * @file hud.cpp
 *
 * @brief Heads-up display implementation.
 *
 * @details
 * Implements a simple HUD used to display:
 * - current mode
 * - controls
 * - number of reached targets
 * - temporary target reached feedback
 *
 * @version 1.0
 */

/* System includes */
#include <string>

/* Third-party includes */
#include <SFML/Graphics.hpp>

/* Project includes */
#include "graphics/hud.hpp"

/* Implementation */
Hud::Hud() : m_isInitialized(false) {
}

bool Hud::initializeHud(const std::string& fontPath) {
    m_isInitialized = m_font.loadFromFile(fontPath);
    return m_isInitialized;
}

void Hud::renderHud(sf::RenderWindow& window, Mode mode, int targetReachedCount, bool targetReached) const {
    if (!m_isInitialized) {
        return;
    }

    sf::Text text;
    text.setFont(m_font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);
    text.setPosition(10.0f, 10.0f);

    std::string modeText = "Mode: ";
    if (mode == Mode::Manual) {
        modeText += "Manual";
    } else if (mode == Mode::Auto) {
        modeText += "Auto";
    } else {
        modeText += "Pause";
    }

    text.setString(modeText + "\nTargets reached: " + std::to_string(targetReachedCount) +
                   "\nControls:" + "\n  ZQSD / arrows: move" + "\n  R: regenerate map + target" +
                   "\n  T: regenerate target" + "\n  P: pause" + "\n  Esc: quit");

    window.draw(text);

    if (targetReached) {
        sf::Text reachedText;
        reachedText.setFont(m_font);
        reachedText.setCharacterSize(24);
        reachedText.setFillColor(sf::Color::Green);
        reachedText.setPosition(10.0f, 180.0f);
        reachedText.setString("Target reached");
        window.draw(reachedText);
    }
}
