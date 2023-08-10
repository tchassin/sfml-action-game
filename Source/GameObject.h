#pragma once

#include <string>

#include <SFML/Graphics/Sprite.hpp>

class Game;
class Level;

class GameObject : public sf::Drawable
{
public:
    GameObject(Game* owner, const std::string& spritePath);
    GameObject(Game* owner, const std::string& spritePath, sf::IntRect spriteRect, sf::Vector2f pivot);
    ~GameObject();

public:

    const Game* getOwner() const { return m_owner; }

    sf::Transformable& getTransform() { return m_sprite; }
    const sf::Transformable& getTransform() const { return m_sprite; }

    sf::Sprite& getSprite() { return m_sprite; }
    const sf::Sprite& getSprite() const { return m_sprite; }

    void moveToLevel(const Level* level) { m_level = level; }
    const Level* getContaingLevel() { return m_level; }

protected:
    // Inherited via Drawable
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    Game* m_owner = nullptr;
    const Level* m_level = nullptr;
    sf::Sprite m_sprite;
};

