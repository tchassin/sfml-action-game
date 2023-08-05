#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <windows.h>

#include "Game.h"
#include "GameObject.h"
#include "PlayerInput.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    window.setFramerateLimit(60);

    Game game;
    GameObject gameObject(&game, "Knight", sf::IntRect(0, 0, 24, 24), sf::Vector2f(12, 12));
    gameObject.getTransform().setPosition(sf::Vector2f(100, 100 ));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        PlayerInput::update();

        sf::Time deltaTime = clock.getElapsedTime();
        clock.restart();

        const float speed = 64.0f * deltaTime.asSeconds();

        if (PlayerInput::isButtonDown(PlayerInput::Button::Left))
            gameObject.getSprite().move(sf::Vector2f(-speed, 0));

        if (PlayerInput::isButtonDown(PlayerInput::Button::Right))
            gameObject.getSprite().move(sf::Vector2f(speed, 0));

        if (PlayerInput::isButtonPressed(PlayerInput::Button::Left))
            gameObject.getSprite().setScale(sf::Vector2f(-1, 1));
        if (PlayerInput::isButtonPressed(PlayerInput::Button::Right))
            gameObject.getSprite().setScale(sf::Vector2f(1, 1));

        window.clear();
        window.draw(gameObject);
        window.display();
    }

    return 0;
}