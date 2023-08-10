#pragma once

#include <tmxlite/SFMLOrthogonalLayer.hpp>

#include "Physics.h"

#ifdef _DEBUG
#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>
#endif // _DEBUG

class Character;
class Game;

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

    Character* getPlayerCharacter() { return m_playerCharacter; }
    const Character* getPlayerCharacter() const { return m_playerCharacter; }

protected:
    void clearMapLayers();

    // Inherited via Drawable
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    tmx::Map m_map;
    MapLayer* m_skyTiles = nullptr;
    MapLayer* m_backgroundTiles = nullptr;
    MapLayer* m_foregroundTiles = nullptr;
    Character* m_playerCharacter = nullptr;
#ifdef _DEBUG
    std::vector<sf::RectangleShape> m_debugRects;
#endif // _DEBUG
};

