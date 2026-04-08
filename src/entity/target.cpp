/**
 * @file target.cpp
 *
 * @brief Target entity implementation.
 *
 * @details
 * Implements a target that can be placed on the map.
 *
 * @version 1.0
 */

/* Project includes */
#include "entity/target.hpp"

/* Implementation */
Target::Target(float radius) : m_position{0.0f, 0.0f}, m_radius(radius) {
}

void Target::setPosition(const Vector2Dim& position) {
    m_position = position;
}

const Vector2Dim& Target::getPosition() const {
    return m_position;
}

float Target::getRadius() const {
    return m_radius;
}
