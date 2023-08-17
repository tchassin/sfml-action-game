#pragma once

#include "Character.h"
#include "Config.h"

class Game;

class Orc : public Character
{
    using base = Character;

public:
    Orc(Game* owner);

public:
    virtual void update(sf::Time deltaTime) override;

    void setPatrolArea(f32 left, f32 right)
        { m_patrolLeft = std::min(left, right); m_patrolRight = std::max(left, right); }

private:
    f32 m_patrolLeft = 0.0f;
    f32 m_patrolRight = VIEWPORT_WIDTH;
};

