#include "GameObject.h"

#include "Game.h"
#include "AssetManager.h"

#include <SFML/Graphics/RenderTarget.hpp>

GameObject::GameObject(Game* owner, const std::string& spritePath)
    : m_owner(owner)
{
    m_sprite.setTexture(*m_owner->getAssetManager()->getTexture(spritePath));
#ifdef _DEBUG
    m_hitBoxDebugRect.setFillColor(sf::Color::Transparent);
    m_hitBoxDebugRect.setOutlineThickness(1);
    m_hitBoxDebugRect.setOutlineColor(sf::Color::White);
#endif // _DEBUG

}

GameObject::GameObject(Game* owner, const std::string& spritePath, sf::IntRect spriteRect, sf::Vector2f pivot)
    : m_owner(owner)
{
    m_sprite.setTexture(*m_owner->getAssetManager()->getTexture(spritePath));
    m_sprite.setTextureRect(spriteRect);
    m_sprite.setOrigin(pivot);
#ifdef _DEBUG
    m_hitBoxDebugRect.setFillColor(sf::Color::Transparent);
    m_hitBoxDebugRect.setOutlineThickness(1);
    m_hitBoxDebugRect.setOutlineColor(sf::Color::White);
#endif // _DEBUG
}

GameObject::~GameObject()
{
}

void GameObject::setHitBox(const sf::FloatRect& hitBox)
{
    m_hitBox = hitBox;
#ifdef _DEBUG
    m_hitBoxDebugRect.setSize(hitBox.getSize());
#endif // _DEBUG
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (isDestroyed())
        return;

    target.draw(m_sprite, states);
#ifdef _DEBUG
    target.draw(m_hitBoxDebugRect, states);
#endif // _DEBUG
}
