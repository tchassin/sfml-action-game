#pragma once

#include <vector>

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "Types.h"
#include <unordered_map>
#include <string>

namespace sf
{
    class Sprite;
}

class AnimationFrameData
{
public:
    AnimationFrameData(f32 duration, sf::Vector2f position, sf::IntRect textureRect);

public:
    void apply(sf::Sprite* target) const;

public:
    f32 getDuration() const { return m_duration; }

private:
    f32 m_duration = 0;
    sf::Vector2f m_position;
    sf::IntRect m_textureRect;
    // TODO: Add hitboxes
};

class AnimationData
{
public:
    AnimationData(const std::string& name, const std::vector<AnimationFrameData>& frames, bool isLooping);

public:
    const std::string& getName() const { return m_name; }

    bool isLooping() const { return m_isLooping; }

    u32 getFrameCount() const { return m_frames.size(); }
    const AnimationFrameData& getFrame(u32 index) const { return m_frames[index % m_frames.size()]; }

    f32 getDuration() const;

private:
    std::string m_name;
    std::vector<AnimationFrameData> m_frames;
    bool m_isLooping = false;
};

class Animation
{
public:
    Animation() {}
    Animation(const AnimationData* data);

public:
    void update(sf::Time deltaTime, sf::Sprite* target);

    void reset() { m_frameIndex = 0; m_time = 0; }
    void reset(const AnimationData* data) { m_data = data; reset(); }

    bool isOver() const { return !m_data->isLooping() && m_frameIndex + 1 == m_data->getFrameCount(); }

    const AnimationData* getAnimationData() const { return m_data; }
    const AnimationFrameData& getCurrentFrame() const { return m_data->getFrame(m_frameIndex); }

private:
    const AnimationData* m_data = nullptr;
    u32 m_frameIndex = 0;
    f32 m_time = 0;
};

class AnimationController
{
public:
    AnimationController() {}
    AnimationController(sf::Sprite* target,
        const std::unordered_map<std::string, const AnimationData*>& animations);

    void setTarget(sf::Sprite* target) { m_target = target; }
    void setAnimations(const std::unordered_map<std::string, const AnimationData*>& animations) { m_animations = animations; }

    void update(sf::Time deltaTime);

    const AnimationData* getAnimationData(const std::string& animationName) const;
    const AnimationData* getCurrentAnimationData() const { return m_currentAnimation.getAnimationData(); }
    bool play(const std::string& animationName);
    bool isPlaying(const std::string& animationName) const;

private:
    sf::Sprite* m_target = nullptr;
    std::unordered_map<std::string, const AnimationData*> m_animations;
    Animation m_currentAnimation;
};

