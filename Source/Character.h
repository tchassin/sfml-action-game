#pragma once

#include "GameObject.h"

#include <SFML/System/Time.hpp>

#include "Types.h"
#include "Animation.h"

class Character : public GameObject
{
    using base = GameObject;

    enum class State : u8
    {
        // The character is on the ground
        Grounded,
        // The character is going upward at full speed
        Jumping,
        // The character is going upward but losing momentum
        Floating,
        // The character is going downward
        Falling,
    };

public:
    Character(Game* owner, const std::string& spriteName, const std::string& animationName);
    virtual ~Character();

public:
    virtual void update(sf::Time deltaTime);
    void jump();
    void move(sf::Vector2f offset, sf::Time deltaTime);

    bool isFlipped() const { return getTransform().getScale().x < 0.0f; }
    bool isGrounded() const { return m_state == State::Grounded; }
    bool isFalling() const { return m_state == State::Falling; }
    bool isFloating() const { return m_state == State::Floating; }
    bool isJumping() const { return m_state == State::Jumping; }

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

    f32 getMinJumpHeight() const { return m_minJumpHeight; }
    void setMinJumpHeight(f32 height) { m_minJumpHeight = height; }

protected:
    void updateHitBoxes();

#ifdef _DEBUG
    // Inherited via Drawable
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
#endif // _DEBUG

private:
    AnimationController m_animationController;

    f32 m_airTime = 0.0f;
    f32 m_runningSpeed = 0.0f;
    f32 m_horizontalAirSpeed = 0.0f;
    f32 m_jumpSpeed = 0.0f;
    f32 m_verticalVelocity = 0.0f;
    f32 m_maxFallSpeed = 0.0f;
    f32 m_minJumpHeight = 0.0f;
    f32 m_maxJumpHeight = 0.0f;

    // Axis-Aligned Bounding Box (AABB) used to detect ground
    sf::FloatRect m_feetBox;
    // Axis-Aligned Bounding Boxes (AABB) used to detect attack hits
    // N.B.: There can be multiple to represent more complex shapes
    std::vector<sf::FloatRect> m_hurtBoxes;
    State m_state = State::Falling;

#ifdef _DEBUG
    sf::RectangleShape m_feetBoxDebugRect;
    std::vector<sf::RectangleShape> m_hurtBoxDebugRects;
#endif // _DEBUG
};
