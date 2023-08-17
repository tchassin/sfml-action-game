#include "Level.h"

#include <algorithm>

#include "Orc.h"
#include "PlayerCharacter.h"
#include "Game.h"
#include "Physics.h"

Level::Level(Game* game)
{
    m_playerCharacter = new PlayerCharacter(game);
    m_playerCharacter->getTransform().setPosition(sf::Vector2f(120, 100));
    m_playerCharacter->moveToLevel(this);

    auto* orc = new Orc(game);
    orc->getTransform().setPosition(sf::Vector2f(120, 100));
    orc->moveToLevel(this);
    m_enemies.push_back(orc);

    orc = new Orc(game);
    orc->getTransform().setPosition(sf::Vector2f(32, 100));
    orc->setPatrolArea(4, 60);
    orc->moveToLevel(this);
    m_enemies.push_back(orc);

    orc = new Orc(game);
    orc->getTransform().setPosition(sf::Vector2f(196, 100));
    orc->setPatrolArea(180, 236);
    orc->moveToLevel(this);
    m_enemies.push_back(orc);

    orc = new Orc(game);
    orc->getTransform().setPosition(sf::Vector2f(120, 52));
    orc->setPatrolArea(84, 156);
    orc->moveToLevel(this);
    m_enemies.push_back(orc);
}

Level::~Level()
{
    clearMapLayers();

    delete m_playerCharacter;

    for (auto* gameObject : m_gameObjects)
        delete gameObject;

    for (auto* enemy : m_enemies)
        delete enemy;
}

void Level::clearMapLayers()
{
    if (m_skyTiles != nullptr)
        delete m_skyTiles;
    if (m_backgroundTiles != nullptr)
        delete m_backgroundTiles;
    if (m_foregroundTiles != nullptr)
        delete m_foregroundTiles;
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Draw map
    if (m_skyTiles != nullptr)
        target.draw(*m_skyTiles, states);
    if (m_backgroundTiles != nullptr)
        target.draw(*m_backgroundTiles, states);
    if (m_foregroundTiles != nullptr)
        target.draw(*m_foregroundTiles, states);

#ifdef _DEBUG
    for (const auto& rectangleShape : m_debugRects)
        target.draw(rectangleShape, states);
#endif // _DEBUG

    // Draw characters
    for (auto* enemy : m_enemies)
        target.draw(*enemy, states);

    // Draw player
    if (m_playerCharacter != nullptr)
        target.draw(*m_playerCharacter, states);

    // Draw other objects like bullets or pick-ups last
    for (auto* gameObject : m_gameObjects)
        target.draw(*gameObject, states);
}

void Level::update(sf::Time deltaTime)
{
    m_playerCharacter->update(deltaTime);

    for (Character* enemy : m_enemies)
        enemy->update(deltaTime);

    for (GameObject* gameObject : m_gameObjects)
        gameObject->update(deltaTime);

    // After updating each character's position, resolve hurtbox collisions
    resolvePlayerAttack();
    resolveEnemyAttacks();
}

bool Level::loadMap(const std::string& mapName)
{
    if (!m_map.load(mapName))
        return false;

    clearMapLayers();

    m_skyTiles = new MapLayer(m_map, 0);
    m_backgroundTiles = new MapLayer(m_map, 1);
    m_foregroundTiles = new MapLayer(m_map, 2);

#ifdef _DEBUG
    m_debugRects.clear();
    for (const auto& layer : m_map.getLayers())
    {
        if (layer->getType() != tmx::Layer::Type::Object)
            continue;

        const std::vector<tmx::Object>& objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();
        for (const auto& object : objects)
        {
            const sf::Vector2f position(object.getAABB().left, object.getAABB().top);
            const sf::Vector2f size(object.getAABB().width, object.getAABB().height);
            sf::RectangleShape& debugRect = m_debugRects.emplace_back(size);
            debugRect.setPosition(position);
            debugRect.setFillColor(sf::Color::Transparent);
            debugRect.setOutlineColor(sf::Color::Cyan);
            debugRect.setOutlineThickness(1);
        }
    }
#endif // _DEBUG
    return true;
}

bool Level::checkForCollisions(const sf::FloatRect& box) const
{
    for (const auto& layer : m_map.getLayers())
    {
        if (layer->getType() != tmx::Layer::Type::Object)
            continue;

        const std::vector<tmx::Object>& objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();
        for (const auto& object : objects)
        {
            if (areRectsIntersecting(box, object.getAABB()))
                return true;
        }
    }

    return false;
}

SweepResult Level::sweepForCollisions(const sf::FloatRect& box, const sf::Vector2f movement) const
{
    SweepResult firstCollision;
    firstCollision.hitTime = INFINITY;

    for (const auto& layer : m_map.getLayers())
    {
        if (layer->getType() != tmx::Layer::Type::Object)
            continue;

        const std::vector<tmx::Object>& objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();
        for (const auto& object : objects)
        {

            const SweepResult collision = sweepAABB(box, object.getAABB(), movement);
            if (collision.hitTime == INFINITY)
                continue;

            if (collision.hitTime < firstCollision.hitTime)
                firstCollision = collision;
        }
    }

    return firstCollision;
}

bool Level::resolvePlayerAttack()
{
    bool hasHit = false;

    const sf::Vector2f playerPosition = m_playerCharacter->getTransform().getPosition();
    std::vector<sf::FloatRect> playerHurtBoxes = m_playerCharacter->getHurtBoxes();
    for (u32 i = 0; i < playerHurtBoxes.size(); i++)
        playerHurtBoxes[i] = sf::FloatRect(playerHurtBoxes[i].getPosition() + playerPosition, playerHurtBoxes[i].getSize());

    for (Character* enemy : m_enemies)
    {
        const sf::Vector2f enemyPosition = enemy->getTransform().getPosition();
        const sf::FloatRect enemyHitBox = sf::FloatRect(enemy->getHitBox().getPosition() + enemyPosition, enemy->getHitBox().getSize());
        for (const sf::FloatRect& playerHurtBox : playerHurtBoxes)
        {
            if (!areRectsIntersecting(playerHurtBox, enemyHitBox))
                continue;

            enemy->destroy();
            hasHit = true;
            break;
        }
    }

    if (!hasHit)
        return false;

    // Delete all game objects flagged for destruction
    for (int i = 0; i < m_enemies.size(); i++)
    {
        if (!m_enemies[i]->isDestroyed())
            continue;

        delete m_enemies[i];
        m_enemies[i] = nullptr;
    }

    const auto removed = std::remove(m_enemies.begin(), m_enemies.end(), nullptr);
    if (removed != m_enemies.end())
        m_enemies.erase(removed, m_enemies.end());

    return true;
}

bool Level::resolveEnemyAttacks()
{
    const sf::Vector2f playerPosition = m_playerCharacter->getTransform().getPosition();
    const sf::FloatRect playerHitBox(m_playerCharacter->getHitBox().getPosition() + playerPosition, m_playerCharacter->getHitBox().getSize());
    for (const Character* enemy : m_enemies)
    {
        const sf::Vector2f enemyPosition = enemy->getTransform().getPosition();

        std::vector<sf::FloatRect> hurtBoxes = enemy->getHurtBoxes();
        for (const sf::FloatRect& hurtBox : hurtBoxes)
        {
            const sf::FloatRect enemyHurtBox(hurtBox.getPosition() + enemyPosition, hurtBox.getSize());
            if (areRectsIntersecting(enemyHurtBox, playerHitBox))
                return true;
        }
    }

    return false;
}
