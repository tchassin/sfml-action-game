#pragma once

class AssetManager;

class Game
{
public:
    Game();
    virtual ~Game();

public:
    const AssetManager* getAssetManager() const { return m_assetManager; }

private:
    AssetManager* m_assetManager;
};

