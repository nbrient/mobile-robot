#ifndef GRAPHICS_HUD_HPP
#define GRAPHICS_HUD_HPP

/**
 * @file hud.hpp
 *
 * @brief Heads-up display declaration.
 *
 * @details This file declares a simple HUD used to display runtime information.
 *
 * @version 1.0
 */

/* System includes */
#include <string>

/* Third-party includes */
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

/* Project includes */
#include "common/mode.hpp"

/* Declarations */

class Hud {
public:
    /**
     * @brief Construct a new Hud object
     *
     */
    Hud();

    /**
     * @brief Initialize Hud object.
     *
     * @param fontPath Path of the font
     * @return bool True if hud has been initialized, false otherwise
     */
    bool initializeHud(const std::string& fontPath);

    /**
     * @brief Render the hud.
     *
     * @param window Target render window.
     * @param mode Game mode.
     * @param targetReachedCount Number of target reached.
     * @param targetReached Bool if a target has been reached.
     */
    void renderHud(sf::RenderWindow& window, Mode mode, int targetReachedCount, bool targetReached) const;

private:
    /**
     * @brief Hud font.
     */
    sf::Font m_font;

    /**
     * @brief Initialization state.
     */
    bool m_isInitialized;
};

#endif
