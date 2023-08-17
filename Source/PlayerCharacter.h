#pragma once

#include "Character.h"

class PlayerCharacter : public Character
{
    using base = Character;

public:
    PlayerCharacter(Game* owner);

public:
    virtual void update(sf::Time deltaTime) override;

private:
    // The time between two consecutive attacks
    f32 m_attackWaitTime = 0.0f;
    // The remaining time before the character can attack again
    f32 m_remainingAttackWaitTime = 0.0f;
};
