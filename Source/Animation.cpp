#include "Animation.h"

#include <SFML/Graphics/Sprite.hpp>

AnimationFrameData::AnimationFrameData(f32 duration, sf::Vector2f position, sf::IntRect textureRect)
    : m_position(position)
    , m_textureRect(textureRect)
    , m_duration(duration)
{
}

void AnimationFrameData::apply(sf::Sprite* target) const
{
    _ASSERT_EXPR(target != nullptr, "Target sprite is null!");
    target->setOrigin(m_position);
    target->setTextureRect(m_textureRect);
}

AnimationData::AnimationData(const std::vector<AnimationFrameData>& frames, bool isLooping)
    : m_frames(frames)
    , m_isLooping(isLooping)
{
}

f32 AnimationData::getDuration() const
{
    f32 duration = 0;
    for (const auto& frameData : m_frames)
        duration += frameData.getDuration();

    return duration;
}


Animation::Animation(const AnimationData* data)
    : m_data(data)
{
}

void Animation::update(sf::Time deltaTime, sf::Sprite* target)
{
    if (isOver())
        return;

    m_time += deltaTime.asSeconds();

    const u32 lastFrameIndex = m_frameIndex;
    f32 frameDuration = getCurrentFrame().getDuration();

    while (m_time > frameDuration)
    {
        m_frameIndex++;

        if (isOver())
            break;

        m_time -= frameDuration;

        frameDuration = getCurrentFrame().getDuration();
    }

    if (lastFrameIndex != m_frameIndex)
        getCurrentFrame().apply(target);
}

AnimationController::AnimationController(sf::Sprite* target,
        const std::unordered_map<std::string, const AnimationData*>& animations)
    : m_animations(animations)
    , m_target(target)
    , m_currentAnimation()
{
}

void AnimationController::update(sf::Time deltaTime)
{
    m_currentAnimation.update(deltaTime, m_target);
}

const AnimationData* AnimationController::getAnimationData(const std::string& animationName) const
{
    const auto result = m_animations.find(animationName);

    return result != m_animations.end() ? result->second : nullptr;
}

bool AnimationController::PlayAnimation(const std::string& animationName)
{
    const AnimationData* animationData = getAnimationData(animationName);

    if (animationData == nullptr)
        return false;

    m_currentAnimation.reset(animationData);
    m_currentAnimation.getCurrentFrame().apply(m_target);

    return true;
}
