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
 *     collision
 *     user input actions
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
namespace {

/**
 * @brief Frame rate limit.
 *
 */
const uint8_t FRAME_RATE_LIMIT{60u};

/**
 * @brief Timer to display message when target is reached.
 *
 */
const uint8_t TIMER_TARGET_REACHED_DISPLAY{1u};

/**
 * @brief Threashold angle to go forward.
 *
 */
const float AUTO_ANGLE_THRESHOLD{0.15f};

/**
 * @brief Cell size for path planner.
 *
 */
const float cellSize{20.0f};

/**
 * @brief Additional safety margin applied to target generation near borders.
 */
static constexpr float TARGET_BORDER_SAFETY_MARGIN_OFFSET = 12.0f;

/**
 * @brief Additional safety margin applied between target and obstacles.
 */
static constexpr float TARGET_OBSTACLE_SAFETY_MARGIN = 12.0f;

/**
 * @brief Additional safety margin applied between target and robot.
 */
static constexpr float TARGET_REACHED_MARGIN = 12.0f;

/**
 * @brief Additional minimum distance required between robot and target.
 */
static constexpr float TARGET_MIN_DISTANCE_FROM_ROBOT_OFFSET = 50.0f;

/**
 * @brief Maximum number of attempts when generating a target.
 */
static constexpr int TARGET_GENERATION_MAX_ATTEMPTS = 500;

/**
 * @brief Distance threshold to consider a waypoint reached in auto mode.
 */
static constexpr float AUTO_WAYPOINT_REACHED_OFFSET = 8.0f;

/**
 * @brief Look-ahead distance to aim at the next waypoint in auto mode.
 */
static constexpr float AUTO_LOOKAHEAD_DISTANCE = 35.0f;

/**
 * @brief Large angle threshold to consider a significant turn in auto mode.
 */
static constexpr float AUTO_LARGE_ANGLE_THRESHOLD = 0.50f;

}  // namespace

Application::Application(const Config& config)
    : m_config(config),
      m_window(sf::VideoMode({config.windowWidth, config.windowHeight}), "Robot Map - First version"),
      m_map(config.mapWidth, config.mapHeight),
      m_robot(config.robotSize, config.robotLineDirectionSize, config.defaultRobotX, config.defaultRobotY),
      m_target(config.targetSize),
      m_rng(config.randomSeed + 123u),
      m_mapGenerator(config.randomSeed),
      m_targetReachedCount(0),
      m_showTargetReached(false),
      m_targetReachedTimer(0.0f),
      m_mode(Mode::Manual),
      m_pathPlanner(cellSize),
      m_currentWayPointIndex(0U) {
    m_window.setFramerateLimit(FRAME_RATE_LIMIT);
    m_mapGenerator.generateNewObstacle(m_map, m_config, m_robot.getPosition(), m_robot.getRadius());
    generateTargetPosition();

    /* Put a valid font path here on your machine */
    m_hud.initializeHud("../assets/arial.ttf");
}

void Application::run() {
    sf::Event event;
    float dt{1.0f / static_cast<float>(FRAME_RATE_LIMIT)};
    bool displayAutoPath{false};

    /* While window open */
    while (m_window.isOpen()) {
        /* Get event on top of the event queue */
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
            }
            /* Handle hud actions */
            handleSingleKeyActions(event);
        }

        if (m_mode == Mode::Manual) {
            m_robot.setForward(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
                               sf::Keyboard::isKeyPressed(sf::Keyboard::Z));

            m_robot.setBackward(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
                                sf::Keyboard::isKeyPressed(sf::Keyboard::S));

            m_robot.setTurnLeft(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
                                sf::Keyboard::isKeyPressed(sf::Keyboard::Q));

            m_robot.setTurnRight(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
                                 sf::Keyboard::isKeyPressed(sf::Keyboard::D));

            displayAutoPath = false;
        } else if (m_mode == Mode::Auto) {
            updateAutoMode(dt);
            displayAutoPath = true;
        } else {
            m_robot.setForward(false);
            m_robot.setBackward(false);
            m_robot.setTurnLeft(false);
            m_robot.setTurnRight(false);
            displayAutoPath = false;
        }

        /* Check candidate position */
        Vector2Dim candidatePosition = m_robot.computeNextPosition(dt);

        if (isRobotPositionValid(candidatePosition)) {
            m_robot.setPosition(candidatePosition);
        } else {
            printf("[MOVE] blocked robot=(%.1f, %.1f) candidate=(%.1f, %.1f) count=%d\n", m_robot.getPosition().x,
                   m_robot.getPosition().y, candidatePosition.x, candidatePosition.y, m_targetReachedCount);

            if (m_mode == Mode::Auto) {
                recomputePath();
            }
        }

        if (isTargetReached()) {
            m_targetReachedCount++;
            m_showTargetReached = true;
            m_targetReachedTimer = TIMER_TARGET_REACHED_DISPLAY;

            printf("[TARGET] reached count=%d robot=(%.1f, %.1f) target=(%.1f, %.1f)\n", m_targetReachedCount,
                   m_robot.getPosition().x, m_robot.getPosition().y, m_target.getPosition().x,
                   m_target.getPosition().y);

            generateTargetPosition();
        }

        /* Timer reached display */
        if (m_showTargetReached) {
            m_targetReachedTimer -= dt;

            if (m_targetReachedTimer <= 0.0f) {
                m_showTargetReached = false;
                m_targetReachedTimer = 0.0f;
            }
        }

        /* Update window */
        m_window.clear();
        m_renderer.renderMap(m_window, m_map, m_robot, m_target, m_currentPath, m_currentWayPointIndex,
                             displayAutoPath);
        m_hud.renderHud(m_window, m_mode, m_targetReachedCount, m_showTargetReached);
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
        const float collisionDistance = robotRadius + obstacle.m_radius;

        /* No need to calculate simple distance, comparison on squared if more efficient */
        if (distanceSquared(candidatePosition, obstacle.m_center) < (collisionDistance * collisionDistance)) {
            return false;
        }
    }

    return true;
}

void Application::generateTargetPosition() {
    const float borderSafetyMargin = m_robot.getRadius() + TARGET_BORDER_SAFETY_MARGIN_OFFSET;

    std::uniform_real_distribution<float> xDist(borderSafetyMargin, m_map.getWidth() - borderSafetyMargin);

    std::uniform_real_distribution<float> yDist(borderSafetyMargin, m_map.getHeight() - borderSafetyMargin);

    for (int attempt = 0; attempt < TARGET_GENERATION_MAX_ATTEMPTS; ++attempt) {
        const Vector2Dim candidatePosition = {xDist(m_rng), yDist(m_rng)};

        /* Check if candidate position is valid and reachable by the robot */
        if (isTargetPositionValid(candidatePosition)) {
            const std::vector<Vector2Dim> candidatePath =
                m_pathPlanner.computePath(m_map, m_robot.getRadius(), m_robot.getPosition(), candidatePosition);

            /* If path is empty, the target is not reachable, try another position */
            if (!candidatePath.empty()) {
                m_target.setPosition(candidatePosition);

                if (m_mode == Mode::Auto) {
                    recomputePath();
                }
                return;
            }
        }
    }

    printf("[TARGET] ERROR no valid target found after %d attempts\n", TARGET_GENERATION_MAX_ATTEMPTS);
}

bool Application::isTargetPositionValid(const Vector2Dim& candidatePosition) const {
    /* Create margin to create a reachable target */
    const float borderSafetyMargin = m_robot.getRadius() + TARGET_BORDER_SAFETY_MARGIN_OFFSET;
    const float minDistanceToRobot = m_robot.getRadius() + m_target.getRadius() + TARGET_MIN_DISTANCE_FROM_ROBOT_OFFSET;

    /* Check distance to map borders */
    if ((candidatePosition.x - borderSafetyMargin) < 0.0f) {
        return false;
    }

    if ((candidatePosition.y - borderSafetyMargin) < 0.0f) {
        return false;
    }

    if ((candidatePosition.x + borderSafetyMargin) > m_map.getWidth()) {
        return false;
    }

    if ((candidatePosition.y + borderSafetyMargin) > m_map.getHeight()) {
        return false;
    }

    /* Check if target touching robot */
    if (distanceSquared(candidatePosition, m_robot.getPosition()) < (minDistanceToRobot * minDistanceToRobot)) {
        return false;
    }

    /* Check if target touching an obstacle and reachable by robot */
    for (const auto& obstacle : m_map.getObstacles()) {
        const float collisionDistance = m_robot.getRadius() + obstacle.m_radius + TARGET_OBSTACLE_SAFETY_MARGIN;

        if (distanceSquared(candidatePosition, obstacle.m_center) < (collisionDistance * collisionDistance)) {
            return false;
        }
    }

    return true;
}

bool Application::isTargetReached() const {
    const float reachDistance = m_robot.getRadius() + m_target.getRadius() - TARGET_REACHED_MARGIN;

    return distanceSquared(m_robot.getPosition(), m_target.getPosition()) < (reachDistance * reachDistance);
}

void Application::regenerateMapAndTarget() {
    m_robot.setPosition({m_config.defaultRobotX, m_config.defaultRobotY});
    m_mode = Mode::Manual;
    m_mapGenerator.generateNewObstacle(m_map, m_config, m_robot.getPosition(), m_robot.getRadius());
    generateTargetPosition();
    recomputePath();
}

void Application::handleSingleKeyActions(const sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) {
        return;
    }
    switch (event.key.code) {
        /* Close window if escape pressed */
        case sf::Keyboard::Escape:
            m_window.close();
            break;

        /* Pause game if p pressed */
        case sf::Keyboard::P:
            if (m_mode == Mode::Manual || m_mode == Mode::Auto) {
                m_mode = Mode::Pause;
            } else {
                m_mode = Mode::Manual;
            }
            break;

        /* Reload map and target if r pressed */
        case sf::Keyboard::R:
            regenerateMapAndTarget();
            break;

        /* Regenerate target position if t pressed */
        case sf::Keyboard::T:
            generateTargetPosition();
            break;

        /* Regenerate target position if t pressed */
        case sf::Keyboard::M:
            if (m_mode == Mode::Manual) {
                m_mode = Mode::Auto;
                recomputePath();
            } else if (m_mode == Mode::Auto) {
                m_mode = Mode::Manual;
            } else {
                /* Nothing to do */
            }
            break;

        default:
            break;
    }
}

void Application::recomputePath() {
    m_currentPath =
        m_pathPlanner.computePath(m_map, m_robot.getRadius(), m_robot.getPosition(), m_target.getPosition());

    /* Reset first point to reach */
    if (m_currentPath.size() > 1U) {
        m_currentWayPointIndex = 1U;
    } else {
        m_currentWayPointIndex = 0U;
    }

    printf("[PATH] recompute robot=(%.1f, %.1f) target=(%.1f, %.1f) pathSize=%zu wayPointIndex=%zu count=%d\n",
           m_robot.getPosition().x, m_robot.getPosition().y, m_target.getPosition().x, m_target.getPosition().y,
           m_currentPath.size(), m_currentWayPointIndex, m_targetReachedCount);

    if (!m_currentPath.empty()) {
        printf("[PATH] first wayPoint=(%.1f, %.1f)\n", m_currentPath[m_currentWayPointIndex].x,
               m_currentPath[m_currentWayPointIndex].y);
    } else {
        printf("[PATH] WARNING empty path\n");
        if (m_mode == Mode::Auto) {
            generateTargetPosition();
        }
    }
}

void Application::updateAutoMode(float dt) {
    (void)dt;

    /* If path is empty, stop the robot */
    if (m_currentPath.empty()) {
        m_robot.setForward(false);
        m_robot.setBackward(false);
        m_robot.setTurnLeft(false);
        m_robot.setTurnRight(false);
        return;
    }

    const Vector2Dim robotPosition = m_robot.getPosition();

    /* Skip all already reached waypoints */
    while (m_currentWayPointIndex < m_currentPath.size()) {
        const Vector2Dim currentWaypoint = m_currentPath[m_currentWayPointIndex];

        float reachDistance = m_robot.getRadius() + AUTO_WAYPOINT_REACHED_OFFSET;

        /* Last waypoint is the exact target position */
        if ((m_currentWayPointIndex + 1U) >= m_currentPath.size()) {
            reachDistance = m_robot.getRadius() + m_target.getRadius() - TARGET_REACHED_MARGIN;
        }

        if (distanceSquared(robotPosition, currentWaypoint) < (reachDistance * reachDistance)) {
            ++m_currentWayPointIndex;
        } else {
            break;
        }
    }

    /* If target is really reached, stop */
    if (isTargetReached() || m_currentWayPointIndex >= m_currentPath.size()) {
        m_robot.setForward(false);
        m_robot.setBackward(false);
        m_robot.setTurnLeft(false);
        m_robot.setTurnRight(false);
        return;
    }

    /* Calculate vector from robot to next waypoint */
    Vector2Dim nextWayPoint = m_currentPath[m_currentWayPointIndex];

    /* Optional short look-ahead */
    if ((m_currentWayPointIndex + 1U) < m_currentPath.size()) {
        const Vector2Dim currentWaypoint = m_currentPath[m_currentWayPointIndex];

        if (distanceSquared(robotPosition, currentWaypoint) < (AUTO_LOOKAHEAD_DISTANCE * AUTO_LOOKAHEAD_DISTANCE)) {
            nextWayPoint = m_currentPath[m_currentWayPointIndex + 1U];
        }
    }

    const float dx = nextWayPoint.x - robotPosition.x;
    const float dy = nextWayPoint.y - robotPosition.y;

    /* Calculate angles */
    const float nextWayPointAngle = std::atan2(dy, dx); /* Use atan2 to disable divisaion by 0 with tan (dx=0) */
    const float robotAngle = m_robot.getAngle();

    float angleDiff = nextWayPointAngle - robotAngle;

    /* Normalize angle between pi and -pi */
    while (angleDiff > 3.14159265f) {
        angleDiff -= 2.0f * 3.14159265f;
    }

    while (angleDiff < -3.14159265f) {
        angleDiff += 2.0f * 3.14159265f;
    }

    /* Turn in right direction or forward if in threshold */
    if (angleDiff > AUTO_ANGLE_THRESHOLD) {
        /* Rotate first if too far from newt waymoint angle */
        if (angleDiff > AUTO_LARGE_ANGLE_THRESHOLD) {
            m_robot.setForward(false);
        } else {
            m_robot.setForward(true);
        }

        m_robot.setTurnLeft(false);
        m_robot.setTurnRight(true);
    } else if (angleDiff < -AUTO_ANGLE_THRESHOLD) {
        /* Rotate first if too far from newt waymoint angle */
        if (angleDiff < -AUTO_LARGE_ANGLE_THRESHOLD) {
            m_robot.setForward(false);
        } else {
            m_robot.setForward(true);
        }

        m_robot.setTurnLeft(true);
        m_robot.setTurnRight(false);
    } else {
        /* Well aligned: go forward */
        m_robot.setForward(true);
        m_robot.setTurnLeft(false);
        m_robot.setTurnRight(false);
    }
}
