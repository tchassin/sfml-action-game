#pragma once

#include <string>

#ifdef _DEBUG
#include <SFML/Graphics/RectangleShape.hpp>
#endif // _DEBUG
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

    const sf::FloatRect& getHitBox() const { return m_hitBox; }
    void setHitBox(const sf::FloatRect& hitBox);

protected:
    // Inherited via Drawable
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
#ifdef _DEBUG
    sf::RectangleShape m_hitBoxDebugRect;
#endif // _DEBUG

private:
    Game* m_owner = nullptr;
    const Level* m_level = nullptr;
    sf::Sprite m_sprite;
    // Axis-Aligned Bounding Boxes (AABB) used to detect incoming collisions
    sf::FloatRect m_hitBox;
};

