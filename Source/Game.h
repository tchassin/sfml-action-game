#pragma once

#include<unordered_map>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

class Game
{
public:
    Game();
    virtual ~Game();

public:
    const sf::Texture* getTexture(const std::string& localTexturePath) const;

private:
    bool loadTexture(const std::string& localTexturePath);

private:
    std::unordered_map<std::string, sf::Texture*> m_textures;
};

