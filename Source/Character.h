#pragma once

#include "GameObject.h"

#include <SFML/System/Time.hpp>

#include "Types.h"
#include "Animation.h"

class Character : public GameObject
{
    using base = GameObject;

public:
    Character(Game* owner, const std::string& spriteName, const std::string& animationName);
    virtual ~Character();

public:
    void update(sf::Time deltaTime);
    void move(sf::Vector2f offset, sf::Time deltaTime);

    bool isFlipped() const { return getTransform().getScale().x < 0.0f; }
    bool isGrounded() const { return m_airTime == 0.0f; }
    bool isFalling() const { return !isGrounded() && m_verticalVelocity > -m_jumpSpeed; }

    sf::Vector2f getForwardDirection() const { return sf::Vector2f(isFlipped() ? -1 : 1, 0); }

    f32 getRunningSpeed() const { return m_runningSpeed; }
    void setRunningSpeed(f32 speed) { m_runningSpeed = speed; }

    f32 getHorizontalAirSpeed() const { return m_horizontalAirSpeed; }
    void setHorizontalAirSpeed(f32 speed) { m_horizontalAirSpeed = speed; }

    f32 getJumpSpeed() const { return m_jumpSpeed; }
    void setJumpSpeed(f32 speed) { m_jumpSpeed = speed; }

    f32 getFallSpeed() const { return m_maxFallSpeed; }
    void setFallSpeed(f32 speed) { m_maxFallSpeed = speed; }

    f32 getJumpHeight() const { return m_maxJumpHeight; }
    void setJumpHeight(f32 height) { m_maxJumpHeight = height; }

private:
    AnimationController m_animationController;
    f32 m_airTime = 0.0f;
    f32 m_runningSpeed = 0.0f;
    f32 m_horizontalAirSpeed = 0.0f;
    f32 m_jumpSpeed = 0.0f;
    f32 m_verticalVelocity = 0.0f;
    f32 m_maxFallSpeed = 0.0f;
    f32 m_maxJumpHeight = 0.0f;
};

