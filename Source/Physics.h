#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "Types.h"

struct SweepResult
{
    f32 hitTime = INFINITY;
    sf::Vector2f hitNormal;
};

// Check if two rectangles are overlapping
// N.B.: Templated for compatibility with sf::Rectangle and tmx::Rectangle
template<class RectA, class RectB>
bool areRectsIntersecting(const RectA& r1, const RectB& r2)
{
    return (r1.left + r1.width >= r2.left)
        && (r1.left <= r2.left + r2.width)
        && (r1.top + r1.height >= r2.top)
        && (r1.top <= r2.top + r2.height);
}

// Sweep an Axis-Aligned Bounding Box against an other and return the normalized collision time
// N.B.: return INFINITY if no collision happens
// N.B.2: Templated for compatibility with sf::Rectangle and tmx::Rectangle
template<class RectA, class RectB>
SweepResult sweepAABB(const RectA& box, const RectB& other, sf::Vector2f movement)
{
    SweepResult result;

    // Broadphase check to reduce calculations and avoid edge cases
    const sf::FloatRect broadphaseAABB((movement.x <= 0) ? box.left + movement.x : box.left,
        (movement.y <= 0) ? box.top + movement.y : box.top,
        (movement.x <= 0) ? box.width - movement.x : box.width + movement.x,
        (movement.y <= 0) ? box.height - movement.y : box.height + movement.y);
    if (!areRectsIntersecting(broadphaseAABB, other))
        return result;

    // Find the distance between objects (per axis)
    const f32 distanceX = (movement.x > 0.0f)
        ? other.left - (box.left + box.width)
        : (other.left + other.width) - box.left;
    const f32 distanceY = (movement.y > 0.0f)
        ? other.top - (box.top + box.height)
        : (other.top + other.height) - box.top;

    // Find time of collision (i.e. when overlappig on both axis)
    const f32 entryTimeX = (movement.x != 0.0f) ? distanceX / movement.x : -INFINITY;
    const f32 entryTimeY = (movement.y != 0.0f) ? distanceY / movement.y : -INFINITY;
    const f32 entryTime = std::max(entryTimeX, entryTimeY);

    // Return normalized collision time if any or INFINITY otherwise
    if (entryTime >= 0.0f && entryTime <= 1.0f)
    {
        result.hitTime = entryTime;

        // Set the contact surface normal
        if (entryTimeX > entryTimeY)
            result.hitNormal = (distanceX < 0.0f) ? sf::Vector2f(1, 0) : sf::Vector2f(-1, 0);
        else
            result.hitNormal = (distanceY < 0.0f) ? sf::Vector2f(0, 1) : sf::Vector2f(0, -1);
    }

    return result;
}

