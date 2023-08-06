#include "Character.h"

#include "Game.h"
#include "AssetManager.h"

namespace
{
    constexpr f32 GRAVITY = 9.81f * 100.0f * 2.0f;
    constexpr f32 GROUND_HEIGHT = 200.0f;
}

Character::Character(Game* owner, const std::string& spriteName, const std::string& animationName)
    : base(owner, spriteName)
{
    m_animationController.setTarget(&getSprite());
    m_animationController.setAnimations(*getOwner()->getAssetManager()->getAnimations(animationName));
    m_animationController.PlayAnimation("Idle");
}

Character::~Character()
{
}

void Character::update(sf::Time deltaTime)
{
    m_animationController.update(deltaTime);
}

void Character::move(sf::Vector2f offset, sf::Time deltaTime)
{
    // Flip sprite to match current direction
    if (offset.x < 0.0f && !isFlipped())
        getTransform().setScale(-1, 1);
    else if (offset.x > 0.0f && isFlipped())
        getTransform().setScale(1, 1);

    const sf::Vector2f position = getTransform().getPosition();
    const f32 maxJumpDuration = m_maxJumpHeight / m_jumpSpeed - m_jumpSpeed / GRAVITY;

    // Handle jumping and falling
    offset.x *= isGrounded() ? m_runningSpeed : m_horizontalAirSpeed;
    if (!isFalling() && offset.y < 0.0f)
    {
        offset.y = -m_jumpSpeed;
        if (m_airTime >= maxJumpDuration)
            offset.y += GRAVITY * deltaTime.asSeconds();
    }
    else
    {
        offset.y = std::min(m_maxFallSpeed, m_verticalVelocity + deltaTime.asSeconds() * GRAVITY);
    }

    m_verticalVelocity = offset.y;

    // Move the character while staying on screen
    sf::Vector2f targetPosition = position + offset * deltaTime.asSeconds();
    targetPosition.x = std::clamp(targetPosition.x, 0.0f, 200.0f);
    targetPosition.y = std::clamp(targetPosition.y, 0.0f, GROUND_HEIGHT);
    getTransform().setPosition(targetPosition);

    // Check if the character is in the air
    if (abs(getTransform().getPosition().y - GROUND_HEIGHT) < 1.0f)
    {
        m_airTime = 0.0f;
        m_verticalVelocity = 0.0f;
    }
    else
    {
        m_airTime += deltaTime.asSeconds();
    }
}

