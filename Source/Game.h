#pragma once

#include <string>

#include <SFML/System/Time.hpp>

namespace sf
{
    class RenderWindow;
}

class AssetManager;
class Level;

class Game
{
public:
    Game();
    virtual ~Game();

public:
    // Draw the game to the render window.
    // N.B.: Should always be called AFTER update()
    void draw(sf::RenderWindow& window) const;

    // Update the main game loop
    void update(sf::Time deltaTime);

    // Load the level with the corresponding name and set it active
    bool loadLevel(const std::string& levelName);

    const AssetManager* getAssetManager() const { return m_assetManager; }

private:
    AssetManager* m_assetManager;
    Level* m_currentLevel;
};

