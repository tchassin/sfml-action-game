#pragma once

#include "Character.h"

class PlayerCharacter : public Character
{
    using base = Character;

public:
    PlayerCharacter(Game* owner, const std::string& spriteName, const std::string& animationName)
        : base(owner, spriteName, animationName)
    {}

public:
    virtual void update(sf::Time deltaTime) override;

private:
};
