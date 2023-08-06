#pragma once

#include <string>

#include <SFML/Graphics/Sprite.hpp>

class Game;

class GameObject : public sf::Drawable
{
public:
    GameObject(Game* owner, const std::string& spritePath);
    GameObject(Game* owner, const std::string& spritePath, sf::IntRect spriteRect, sf::Vector2f pivot);
    ~GameObject();

public:
    // Inherited via Drawable
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    const Game* getOwner() const { return m_owner; }

    sf::Transformable& getTransform() { return m_sprite; }
    const sf::Transformable& getTransform() const { return m_sprite; }

    sf::Sprite& getSprite() { return m_sprite; }
    const sf::Sprite& getSprite() const { return m_sprite; }

private:
    Game* m_owner;
    sf::Sprite m_sprite;
};

