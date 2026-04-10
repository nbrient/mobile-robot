#ifndef ENTITY_ROBOT_HPP
#define ENTITY_ROBOT_HPP

/**
 * @file robot.hpp
 *
 * @brief Robot entity declaration.
 *
 * @details This class represents a simple mobile robot controlled manually.
 *
 *
 * @version 1.0
 */

/* Project includes */
#include "common/common.hpp"

/* Declarations */
class Robot {
public:
    /**
     * @brief Construct a robot with default values.
     * @param size Robot size.
     * @param lineSize Size of direction line.
     * @param defaultX Default robot x position.
     * @param defaultY Default robot Y position.
     */
    Robot(float size, float lineSize, float defaultX, float defaultY);

    /**
     * @brief Set forward command state.
     *
     * @param forward True to move forward.
     */
    void setForward(bool forward);

    /**
     * @brief Set backward command state.
     *
     * @param backward True to move backward.
     */
    void setBackward(bool backward);

    /**
     * @brief Set left command state.
     *
     * @param left True to move backward.
     */
    void setTurnLeft(bool left);

    /**
     * @brief Set right command state.
     *
     * @param right True to move right.
     */
    void setTurnRight(bool right);

    /**
     * @brief Get robot position.
     *
     * @return Constant reference to robot position.
     */
    const Vector2Dim& getPosition() const;

    /**
     * @brief Get robot orientation angle.
     *
     * @return Robot angle in radians.
     */
    float getAngle() const;

    /**
     * @brief Get robot radius.
     *
     * @return Robot radius.
     */
    float getRadius() const;

    /**
     * @brief Get robot direction line size.
     *
     * @return Robot direction line size.
     */
    float getDirectionLineSize() const;

    /**
     * @brief Compute the next requested position.
     *
     * @param dt Time step in seconds.
     *
     * @return Candidate robot position.
     */
    Vector2Dim computeNextPosition(float dt);

    /**
     * @brief Apply a validated position.
     *
     * @param position New robot position.
     */
    void setPosition(const Vector2Dim& position);

private:
    /**
     * @brief Current position.
     */
    Vector2Dim m_position;

    /**
     * @brief Current robotangle.
     */
    float m_angle;

    /**
     * @brief Current robot size.
     */
    float m_radius;

    /**
     * @brief Direction line size.
     */
    float m_directionLineSize;

    /**
     * @brief Forward command state.
     */
    bool m_forward;

    /**
     * @brief backward command state.
     */
    bool m_backward;

    /**
     * @brief Left tuen command state.
     */
    bool m_turnLeft;

    /**
     * @brief Right turn command state.
     */
    bool m_turnRight;
};

#endif
