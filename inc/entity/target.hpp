#ifndef ENTITY_TARGET_HPP
#define ENTITY_TARGET_HPP

/**
 * @file target.hpp
 *
 * @brief Target entity declaration.
 *
 * @details
 * This class represents a target that the robot must reach.
 *
 * @version 1.0
 */

/* Project includes */
#include "common/common.hpp"

/* Declarations */
class Target {
public:
    /**
     * @brief Construct a target with a configurable size.
     *
     * @param radius Target radius.
     */
    explicit Target(float radius);

    /**
     * @brief Set target position.
     *
     * @param position New target position.
     */
    void setPosition(const Vector2Dim& position);

    /**
     * @brief Get target position.
     *
     * @return Constant reference to target position.
     */
    const Vector2Dim& getPosition() const;

    /**
     * @brief Get target radius.
     *
     * @return Target radius.
     */
    float getRadius() const;

private:
    /**
     * @brief Center position of the taget.
     */
    Vector2Dim m_position;

    /**
     * @brief Radius of the target.
     */
    float m_radius;
};

#endif
