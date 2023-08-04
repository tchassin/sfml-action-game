#include "GameObject.h"

#include "Game.h"

#include <SFML/Graphics/RenderTarget.hpp>

GameObject::GameObject(Game* owner, const std::string& spritePath, sf::IntRect spriteRect, sf::Vector2f pivot)
    : m_owner(owner)
{
    m_sprite.setTexture(*m_owner->getTexture(spritePath));
    m_sprite.setTextureRect(spriteRect);
    m_sprite.setOrigin(pivot);
}

GameObject::~GameObject()
{
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}
