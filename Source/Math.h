#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "Types.h"

f32 dotProduct(const sf::Vector2f& v0, const sf::Vector2f& v1)
{
    return v0.x * v1.x + v0.y * v1.y;
}

template<class Rect>
Rect flipRect(const Rect& rect)
{
    Rect flipped = rect;
    flipped.left = -rect.left - rect.width;

    return flipped;
}
