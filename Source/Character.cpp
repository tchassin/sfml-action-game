#include "Character.h"

#include "AssetManager.h"
#include "Config.h"
#include "Game.h"
#include "Level.h"
#include "Math.h"

namespace
{
    constexpr f32 GRAVITY = 9.81f * 100.0f * 2.0f;

    const std::string IDLE_ANIMATION = "Idle";
    const std::string RUN_ANIMATION = "Run";
    const std::string JUMP_ANIMATION = "Jump";
    const std::string FALL_ANIMATION = "Fall";
    const std::string ATTACK_ANIMATION = "Attack";
}

Character::Character(Game* owner, const std::string& spriteName, const std::string& animationName)
    : base(owner, spriteName)
{
    m_animationController.setTarget(&getSprite());
    m_animationController.setAnimations(*getOwner()->getAssetManager()->getAnimations(animationName));
    m_animationController.play(IDLE_ANIMATION);

    updateHitBoxes();

#ifdef _DEBUG
    m_feetBoxDebugRect.setFillColor(sf::Color::Transparent);
    m_feetBoxDebugRect.setOutlineThickness(1);
    m_feetBoxDebugRect.setOutlineColor(sf::Color::Blue);
#endif // _DEBUG
}

Character::~Character()
{
}

void Character::update(sf::Time deltaTime)
{
    const bool hasChangeFrame = m_animationController.update(deltaTime);

    if (isAttacking() && !m_animationController.isPlaying(ATTACK_ANIMATION))
    {
        m_state = State::Grounded;
        m_animationController.play(IDLE_ANIMATION);
    }
    else if (!hasChangeFrame)
    {
        return;
    }

    updateHitBoxes();
}

void Character::jump()
{
    m_state = State::Jumping;
    m_verticalVelocity = -m_jumpSpeed;
    m_animationController.play(JUMP_ANIMATION);
}

bool Character::attack()
{
    if (!isGrounded())
        return false;

    m_state = State::Attacking;
    m_animationController.play(ATTACK_ANIMATION);

    return true;
}

void Character::move(sf::Vector2f offset, sf::Time deltaTime)
{
    if (isAttacking())
        return;

    // Flip sprite to match current direction
    if (offset.x < 0.0f && !isFlipped())
    {
        getTransform().setScale(-1, 1);
        flipHitBoxes();
    }
    else if (offset.x > 0.0f && isFlipped())
    {
        getTransform().setScale(1, 1);
        flipHitBoxes();
    }

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
    if (!isJumping() && !isFloating())
    {
        // Check if the character is grounded or in the air
        const sf::FloatRect currentFeetBox(position + m_feetBox.getPosition(), m_feetBox.getSize());
        m_state = getContaingLevel()->checkForCollisions(currentFeetBox) ? State::Grounded : State::Falling;
        // Check for collisions only if the character going down up to pass through platforms
        const sf::FloatRect currentHitBox(position + getHitBox().getPosition(), getHitBox().getSize());
        collision = getContaingLevel()->sweepForCollisions(currentHitBox, frameOffset);
    }

    // Handle collisions by sliding against the collided edge
    sf::Vector2f targetPosition;
    if (collision.hitTime < INFINITY && collision.hitNormal.y < 0)
    {
        const sf::Vector2f slidingDirection(collision.hitNormal.y, collision.hitNormal.x);
        const f32 slidingLength = dotProduct(frameOffset, slidingDirection);
        const sf::Vector2f slidingOffset = slidingDirection * slidingLength;
        const sf::Vector2f hitPoint = position + frameOffset * collision.hitTime;
        // There should be more complex checks to make sure this does not cause additional collisions
        // but since the level geometry is simple enough, it is not necessary
        targetPosition = hitPoint + slidingOffset * (1.0f - collision.hitTime);
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
    m_hitBoxDebugRect.setPosition(position + getHitBox().getPosition());
    m_feetBoxDebugRect.setPosition(position + m_feetBox.getPosition());
    for (u8 i = 0; i < m_hurtBoxes.size(); i++)
        m_hurtBoxDebugRects[i].setPosition(position + m_hurtBoxes[i].getPosition());
#endif // _DEBUG
}

void Character::updateHitBoxes()
{
    const AnimationFrameData& currentFrame = m_animationController.getCurrentFrame();
    setHitBox(currentFrame.getHitBox());
    m_feetBox = currentFrame.getFeetBox();
    m_hurtBoxes = currentFrame.getHurtBoxes();

    if (isFlipped())
        flipHitBoxes();

#ifdef _DEBUG
    m_feetBoxDebugRect.setSize(m_feetBox.getSize());

    const sf::Vector2f position = getTransform().getPosition();

    m_hurtBoxDebugRects.clear();
    for (const auto& hurtBox : m_hurtBoxes)
    {
        sf::RectangleShape& debugRect = m_hurtBoxDebugRects.emplace_back(hurtBox.getSize());
        debugRect.setFillColor(sf::Color::Transparent);
        debugRect.setOutlineThickness(1);
        debugRect.setOutlineColor(sf::Color::Red);
        debugRect.setPosition(position + hurtBox.getPosition());
    }
#endif // _DEBUG
}

void Character::flipHitBoxes()
{
    setHitBox(flipRect(getHitBox()));
    m_feetBox = flipRect(m_feetBox);

    for (auto& hurtBox : m_hurtBoxes)
        hurtBox = flipRect(hurtBox);
}

#ifdef _DEBUG

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    base::draw(target, states);
    target.draw(m_feetBoxDebugRect, states);
    for (const auto& hurtBoxDebugRect : m_hurtBoxDebugRects)
        target.draw(hurtBoxDebugRect, states);
}
#endif // _DEBUG
