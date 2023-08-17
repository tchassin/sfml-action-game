#include "Orc.h"

Orc::Orc(Game* owner)
    : base(owner, "Orc", "Orc")
{
    setRunningSpeed(40.0f);
    setHorizontalAirSpeed(40.0f);
    setJumpSpeed(240.0f);
    setFallSpeed(360.0f);
    setMinJumpHeight(32.0f);
    setJumpHeight(80.0f);
}

void Orc::update(sf::Time deltaTime)
{
    base::update(deltaTime);

    const sf::Vector2f position = getTransform().getPosition();
    const sf::Vector2f forward = getForwardDirection();
    const f32 frameMovement = forward.x * getRunningSpeed() * deltaTime.asSeconds();
    const f32 positionNextFrame = position.x + frameMovement;

    if (positionNextFrame <= m_patrolLeft)
        move(sf::Vector2f(1.0f, 0.0f), deltaTime);
    else if (positionNextFrame >= m_patrolRight)
        move(sf::Vector2f(-1.0f, 0.0f), deltaTime);
    else
        move(forward, deltaTime);
}
