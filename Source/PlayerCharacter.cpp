#include "PlayerCharacter.h"

#include "PlayerInput.h"

void PlayerCharacter::update(sf::Time deltaTime)
{
    base::update(deltaTime);

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
    }
    else if (isJumping())
    {
        if (PlayerInput::isButtonDown(PlayerInput::Button::A))
            movement.y -= 1.0f;
    }

    move(movement, deltaTime);
}
