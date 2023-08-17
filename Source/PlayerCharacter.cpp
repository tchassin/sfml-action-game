#include "PlayerCharacter.h"

#include "PlayerInput.h"

PlayerCharacter::PlayerCharacter(Game* owner)
    : base(owner, "Knight", "Knight")
{
    setRunningSpeed(120.0f);
    setHorizontalAirSpeed(120.0f);
    setJumpSpeed(240.0f);
    setFallSpeed(360.0f);
    setMinJumpHeight(32.0f);
    setJumpHeight(80.0f);
}

void PlayerCharacter::update(sf::Time deltaTime)
{
    base::update(deltaTime);

    if (m_remainingAttackWaitTime > 0.0f)
        m_remainingAttackWaitTime -= deltaTime.asSeconds();

    sf::Vector2f movement;

    if (PlayerInput::isButtonDown(PlayerInput::Button::Left))
        movement.x -= 1.0f;
    if (PlayerInput::isButtonDown(PlayerInput::Button::Right))
        movement.x += 1.0f;

    if (isGrounded())
    {
        if (PlayerInput::isButtonPressed(PlayerInput::Button::A))
        {
            jump();
            movement.y -= 1.0f;
        }
        else if (PlayerInput::isButtonPressed(PlayerInput::Button::B))
        {
            if (canAttack() && m_remainingAttackWaitTime <= 0.0f)
            {
                attack();

                m_remainingAttackWaitTime = getAnimationController().getCurrentAnimationData()->getDuration()
                    + m_attackWaitTime;
            }
        }
    }
    else if (isJumping())
    {
        if (PlayerInput::isButtonDown(PlayerInput::Button::A))
            movement.y -= 1.0f;
    }

    move(movement, deltaTime);
}
