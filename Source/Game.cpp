#include "Game.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include "AssetManager.h"
#include "Level.h"
#include "PlayerInput.h"

Game::Game()
{
    m_assetManager = new AssetManager();
    m_currentLevel = new Level(this);
}

Game::~Game()
{
    delete m_assetManager;
    delete m_currentLevel;
}

void Game::draw(sf::RenderWindow& window) const
{
    window.clear();
    window.draw(*m_currentLevel);
    window.display();
}

void Game::update(sf::Time deltaTime)
{
    PlayerInput::update();

    m_currentLevel->update(deltaTime);
}

bool Game::loadLevel(const std::string& levelName)
{
    return m_currentLevel->loadMap(levelName);
}
