/**
 * @file robot.cpp
 *
 * @brief Robot entity implementation.
 *
 * @details
 * Implements a simple manually controlled robot.
 *
 * @version 1.0
 */

/* System includes */
#include <cmath>

/* Project includes */
#include "entity/robot.hpp"

/* Definitions */
namespace {
static constexpr float LINEAR_SPEED = 200.0f;
static constexpr float ANGULAR_SPEED = 2.5f;
}  // namespace

/* Implementation */
Robot::Robot(float size, float directionLineSize, float defaultX, float defaultY)
    : m_position{defaultX, defaultY},
      m_angle(0.0f),
      m_radius(size),
      m_directionLineSize(directionLineSize),
      m_forward(false),
      m_backward(false),
      m_turnLeft(false),
      m_turnRight(false) {
}

Vector2Dim Robot::computeNextPosition(float dt) {
    if (m_turnLeft && !m_turnRight) {
        m_angle -= ANGULAR_SPEED * dt;
    } else if (m_turnRight && !m_turnLeft) {
        m_angle += ANGULAR_SPEED * dt;
    } else {
        /* Nothing to do */
    }

    float direction = 0.0f;

    if (m_forward && !m_backward) {
        direction += 1.0f;
    } else if (m_backward && !m_forward) {
        direction -= 1.0f;
    } else {
        /* Nothing to do */
    }

    Vector2Dim nextPosition = m_position;
    nextPosition.x += std::cos(m_angle) * direction * LINEAR_SPEED * dt;
    nextPosition.y += std::sin(m_angle) * direction * LINEAR_SPEED * dt;

    return nextPosition;
}

void Robot::setForward(bool forward) {
    m_forward = forward;
}

void Robot::setBackward(bool backward) {
    m_backward = backward;
}

void Robot::setTurnLeft(bool left) {
    m_turnLeft = left;
}

void Robot::setTurnRight(bool right) {
    m_turnRight = right;
}

const Vector2Dim& Robot::getPosition() const {
    return m_position;
}

float Robot::getAngle() const {
    return m_angle;
}

float Robot::getRadius() const {
    return m_radius;
}

float Robot::getDirectionLineSize() const {
    return m_directionLineSize;
}

void Robot::setPosition(const Vector2Dim& position) {
    m_position = position;
}
