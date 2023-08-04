#include "Game.h"


Game::Game()
{
    loadTexture("Knight");
}

Game::~Game()
{
    for (auto [path, texture] : m_textures)
        delete texture;
}

const sf::Texture* Game::getTexture(const std::string& localTexturePath) const
{
    const auto result = m_textures.find(localTexturePath);

    return result != m_textures.end() ? result->second : nullptr;
}

bool Game::loadTexture(const std::string& localTexturePath)
{
    auto* texture = new sf::Texture();
    const std::string localPathWithExtension = "Assets/Sprites/" + localTexturePath + ".png";

    if (!texture->loadFromFile(localPathWithExtension))
    {
        delete texture;

        return false;
    }

    m_textures.emplace(localTexturePath, texture);

    return true;
}

