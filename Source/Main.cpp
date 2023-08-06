#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <windows.h>

#include "Game.h"
#include "Character.h"
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
    window.setVerticalSyncEnabled(true);

    Game game;
    Character knight(&game, "Knight", sf::IntRect(0, 0, 24, 24), sf::Vector2f(12, 24));
    knight.getTransform().setPosition(sf::Vector2f(100, 100 ));
    knight.setRunningSpeed(120.0f);
    knight.setHorizontalAirSpeed(120.0f);
    knight.setJumpSpeed(360.0f);
    knight.setFallSpeed(480.0f);
    knight.setJumpHeight(120.0f);

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

        const f32 speed = 64.0f * deltaTime.asSeconds();

        sf::Vector2f movement;
        if (PlayerInput::isButtonDown(PlayerInput::Button::Left))
            movement.x -= 1.0f;
        if (PlayerInput::isButtonDown(PlayerInput::Button::Right))
            movement.x += 1.0f;
        if (PlayerInput::isButtonDown(PlayerInput::Button::A))
            movement.y -= 1.0f;

        knight.move(movement, deltaTime);

        window.clear();
        window.draw(knight);
        window.display();
    }

    return 0;
}