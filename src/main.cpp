/**
 * @file main.cpp
 *
 * @brief Application entry point.
 *
 * @details
 * This file initializes the configuration, creates the application object
 * and starts the main loop.
 *
 * @par Execution flow
 * @verbatim
 * load configuration
 * create application
 * run application
 * @endverbatim
 *
 * @version 1.0
 */

/* System includes */
#include <exception>
#include <iostream>

/* Project includes */
#include "app/application.hpp"
#include "app/config.hpp"

/* Implementation */
int main(int argc, char* argv[]) {
    try {
        /* Default config path */
        std::string configPath = "config/config.json";

        /* Get config path (passing with argument) */
        if (argc > 1) {
            configPath = argv[1];
        }

        /* Load config */
        const Config config = Config::loadFromFile(configPath);

        Application app(config);
        app.run();
    } catch (const std::exception& exception) {
        std::cerr << "Error: " << exception.what() << std::endl;
        return 1;
    }

    return 0;
}