#pragma once

#include<unordered_map>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

class AnimationData;

class AssetManager
{
public:
    AssetManager();
    virtual ~AssetManager();

public:
    const sf::Texture* getTexture(const std::string& localTexturePath) const;
    const std::unordered_map<std::string, const AnimationData*>* getAnimations(const std::string& localPath) const;

private:
    bool loadTexture(const std::string& localPath, const std::string& assetName);
    bool loadAnimations(const std::string& localPath, const std::string& assetName);

private:
    std::unordered_map<std::string, const sf::Texture*> m_textures;
    std::unordered_map < std::string, std::unordered_map<std::string, const AnimationData*>> m_animations;
};

