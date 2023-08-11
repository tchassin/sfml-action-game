#include "Character.h"

#include "Config.h"
#include "Game.h"
#include "Level.h"
#include "AssetManager.h"

namespace
{
    constexpr f32 GRAVITY = 9.81f * 100.0f * 2.0f;
    const std::string IDLE_ANIMATION = "Idle";
    const std::string RUN_ANIMATION = "Run";
    const std::string JUMP_ANIMATION = "Jump";
    const std::string FALL_ANIMATION = "Fall";
}

Character::Character(Game* owner, const std::string& spriteName, const std::string& animationName)
    : base(owner, spriteName)
{
    m_animationController.setTarget(&getSprite());
    m_animationController.setAnimations(*getOwner()->getAssetManager()->getAnimations(animationName));
    m_animationController.play(IDLE_ANIMATION);

#ifdef _DEBUG
    m_feetBoxRect.setFillColor(sf::Color::Transparent);
    m_feetBoxRect.setOutlineThickness(1);
    m_feetBoxRect.setOutlineColor(sf::Color::Magenta);
    m_feetBoxRect.setSize(sf::Vector2f(8, 2));

    m_hitBoxRect.setFillColor(sf::Color::Transparent);
    m_hitBoxRect.setOutlineThickness(1);
    m_hitBoxRect.setOutlineColor(sf::Color::White);
    m_hitBoxRect.setSize(sf::Vector2f(10, 18));
#endif // _DEBUG
}

Character::~Character()
{
}

void Character::update(sf::Time deltaTime)
{
    m_animationController.update(deltaTime);
}

void Character::startJumping()
{
    m_state = State::Jumping;
    m_verticalVelocity = -m_jumpSpeed;
    m_animationController.play(JUMP_ANIMATION);
}

void Character::move(sf::Vector2f offset, sf::Time deltaTime)
{
    // Flip sprite to match current direction
    if (offset.x < 0.0f && !isFlipped())
        getTransform().setScale(-1, 1);
    else if (offset.x > 0.0f && isFlipped())
        getTransform().setScale(1, 1);

    // Handle jumping and falling
    offset.x *= isGrounded() ? m_runningSpeed : m_horizontalAirSpeed;
    const f32 minJumpDuration = m_minJumpHeight / m_jumpSpeed - m_jumpSpeed / GRAVITY;

    // Force minimum jump duration for better game feel
    if (isJumping() && (m_airTime < minJumpDuration || offset.y < 0))
    {
        offset.y = -m_jumpSpeed;
        // Check if the jump is ending
        const f32 maxJumpDuration = m_maxJumpHeight / m_jumpSpeed - m_jumpSpeed / GRAVITY;
        if (m_airTime >= maxJumpDuration)
            offset.y += GRAVITY * deltaTime.asSeconds();
    }
    else
    {
        // Decelerate if the character is not jumping
        offset.y += std::min(m_maxFallSpeed, m_verticalVelocity + deltaTime.asSeconds() * GRAVITY);
    }

    m_verticalVelocity = offset.y;

    // Update state based on velocity after jumping is over
    if (!isGrounded() && m_verticalVelocity > -m_jumpSpeed)
    {
        m_state = (m_verticalVelocity > 0) ? State::Falling : State::Floating;
            m_animationController.play(FALL_ANIMATION);
    }

    const sf::Vector2f position = getTransform().getPosition();
    const sf::Vector2f frameOffset = offset * deltaTime.asSeconds();

    SweepResult collision;
    const sf::FloatRect feetBox(position + sf::Vector2f(-4, 0), sf::Vector2f(8, 1));
    const sf::FloatRect hitBox(position + sf::Vector2f(-5, -18), sf::Vector2f(10, 18));
    if (!isJumping() && !isFloating())
    {
        // Check if the character is grounded or in the air
        m_state = getContaingLevel()->checkForCollisions(feetBox) ? State::Grounded : State::Falling;
        // Check for collisions only if the character going down up to pass through platforms
        collision = getContaingLevel()->sweepForCollisions(hitBox, frameOffset);
    }

    // Handle collisions by sliding against the collided edge
    sf::Vector2f targetPosition;
    if (collision.hitTime < INFINITY && collision.hitNormal.y < 0)
    {
        const f32 dotProduct = (frameOffset.x * collision.hitNormal.y + frameOffset.y * collision.hitNormal.x);
        const sf::Vector2f slideOffset = sf::Vector2f(collision.hitNormal.y, collision.hitNormal.x) * dotProduct;
        const sf::Vector2f hitPoint = position + frameOffset * collision.hitTime;
        // There should be more complex checks to make sure this does not cause additional collisions
        // but since the level geometry is simple enough, it is not necessary
        targetPosition = hitPoint + slideOffset * (1.0f - collision.hitTime);
    }
    else
    {
        targetPosition = position + frameOffset;
    }

    // Move the character while staying on screen
    targetPosition.x = std::clamp(targetPosition.x, 0.0f, (f32)VIEWPORT_WIDTH);
    getTransform().setPosition(targetPosition);

    // Update air time
    if (isGrounded())
    {
        m_airTime = 0.0f;
        m_verticalVelocity = 0.0f;
        if (offset.x != 0)
            m_animationController.play(RUN_ANIMATION);
        else
            m_animationController.play(IDLE_ANIMATION);
    }
    else
    {
        m_airTime += deltaTime.asSeconds();
    }

#ifdef _DEBUG
    m_feetBoxRect.setPosition(feetBox.getPosition());
    m_feetBoxRect.setOutlineColor(isGrounded() ? sf::Color::Blue : isFalling() ? sf::Color::Red : sf::Color::Yellow);
    m_hitBoxRect.setPosition(hitBox.getPosition());
#endif // _DEBUG
}

#ifdef _DEBUG
void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    base::draw(target, states);
    target.draw(m_feetBoxRect, states);
    target.draw(m_hitBoxRect, states);
}
#endif // _DEBUG
