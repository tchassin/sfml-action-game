#include "Game.h"

#include "AssetManager.h"

Game::Game()
{
    m_assetManager = new AssetManager();
}

Game::~Game()
{
    delete m_assetManager;
}