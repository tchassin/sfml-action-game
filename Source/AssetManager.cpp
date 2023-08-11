#include "AssetManager.h"

#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

#include "Animation.h"

AssetManager::AssetManager()
{
    // Load all textures
    loadTexture("Knight.png", "Knight");

    // Load all animations
    loadAnimations("Knight.json", "Knight");
}

AssetManager::~AssetManager()
{
    for (auto [path, texture] : m_textures)
        delete texture;
}

const sf::Texture* AssetManager::getTexture(const std::string& localTexturePath) const
{
    const auto result = m_textures.find(localTexturePath);

    return result != m_textures.end() ? result->second : nullptr;
}

const std::unordered_map<std::string, const AnimationData*>* AssetManager::getAnimations(const std::string& localPath) const
{
    const auto result = m_animations.find(localPath);

    return result != m_animations.end() ? &result->second : nullptr;
}

bool AssetManager::loadAnimations(const std::string& localPath, const std::string& assetName)
{
    std::ifstream file("Assets/Sprites/" + localPath);
    if (file.bad())
        return false;

    try
    {
        const nlohmann::json json = nlohmann::json::parse(file);

        std::unordered_map<std::string, const AnimationData*> animations;
        const nlohmann::json jsonAnimations = json.at("animations");
        for (const auto& jsonAnimation : jsonAnimations)
        {
            const std::string name = jsonAnimation.at("name");
            const bool isLooping = jsonAnimation.at("looping");
            std::vector<AnimationFrameData> frames;
            for (const auto& jsonFrame : jsonAnimation.at("keyframes"))
            {
                const f32 duration = jsonFrame.at("duration") * 0.001f;
                const i32 x = jsonFrame.at("x");
                const i32 y = jsonFrame.at("y");
                const i32 width = jsonFrame.at("width");
                const i32 height = jsonFrame.at("height");
                const i32 atlasX = jsonFrame.at("atlasX");
                const i32 atlasY = jsonFrame.at("atlasY");
                frames.emplace_back(duration, sf::Vector2f(-x, -y), sf::IntRect(atlasX, atlasY, width, height));
            }

            animations.emplace(name, new AnimationData(name, frames, isLooping));
        }

        m_animations[assetName] = animations;

        return true;
    }
    catch (nlohmann::json::parse_error& ex)
    {
        std::cerr << "parse error at byte " << ex.byte << std::endl;

        return false;
    }
}

bool AssetManager::loadTexture(const std::string& localPath, const std::string& assetName)
{
    auto* texture = new sf::Texture();
    const std::string localPathWithExtension = "Assets/Sprites/" + localPath;

    if (!texture->loadFromFile(localPathWithExtension))
    {
        delete texture;

        return false;
    }

    m_textures.emplace(assetName, texture);

    return true;
}
