#pragma once

#include <tmxlite/SFMLOrthogonalLayer.hpp>

#include "Physics.h"

#ifdef _DEBUG
#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>
#endif // _DEBUG

class Character;
class Game;
class GameObject;
class PlayerCharacter;

class Level : public sf::Drawable
{
    using base = sf::Drawable;
public:
    Level(Game* game);
    ~Level();

    void update(sf::Time deltaTime);

    bool loadMap(const std::string& mapName);

    // Check a bounding box against the level geometry
    bool checkForCollisions(const sf::FloatRect& box) const;
    // Check if a moving bounding box will intersect with the level geometry
    SweepResult sweepForCollisions(const sf::FloatRect& box, const sf::Vector2f movement) const;

    PlayerCharacter* getPlayerCharacter() { return m_playerCharacter; }
    const PlayerCharacter* getPlayerCharacter() const { return m_playerCharacter; }

    const std::vector<Character*>& getEnemies() const { return m_enemies; }
    const std::vector<GameObject*>& getGameObjects() const { return m_gameObjects; }

protected:
    void clearMapLayers();

    // Inherited via Drawable
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    // Check the player hurtboxes against enemy hitboxes and return if any enemy was hit
    // N.B.: This will check all collisions will all enemies
    bool resolvePlayerAttack();
    // Check the enemy hurtboxes against player hitbox and return if the player was hit
    // N.B.: The player can only be hit once per frame
    bool resolveEnemyAttacks();

private:
    tmx::Map m_map;
    MapLayer* m_skyTiles = nullptr;
    MapLayer* m_backgroundTiles = nullptr;
    MapLayer* m_foregroundTiles = nullptr;
    PlayerCharacter* m_playerCharacter = nullptr;
    std::vector<GameObject*> m_gameObjects;
    std::vector<Character*> m_enemies;
#ifdef _DEBUG
    std::vector<sf::RectangleShape> m_debugRects;
#endif // _DEBUG
};

