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
static constexpr float LINEAR_SPEED = 200.0f;
static constexpr float ANGULAR_SPEED = 2.5f;

/* Implementation */
Robot::Robot()
    : m_position{100.0f, 100.0f},
      m_angle(0.0f),
      m_radius(15.0f),
      m_forward(false),
      m_backward(false),
      m_turnLeft(false),
      m_turnRight(false) {
}

void Robot::update(float dt) {
    if (m_turnLeft) {
        m_angle -= ANGULAR_SPEED * dt;
    }

    if (m_turnRight) {
        m_angle += ANGULAR_SPEED * dt;
    }

    float direction = 0.0f;

    if (m_forward) {
        direction += 1.0f;
    }

    if (m_backward) {
        direction -= 1.0f;
    }

    m_position.x += std::cos(m_angle) * direction * LINEAR_SPEED * dt;
    m_position.y += std::sin(m_angle) * direction * LINEAR_SPEED * dt;
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
