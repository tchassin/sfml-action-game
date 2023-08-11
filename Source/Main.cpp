#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <windows.h>

#include "Config.h"
#include "Game.h"

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

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Action Game");
    window.setFramerateLimit(FRAMERATE);
    window.setVerticalSyncEnabled(true);

    sf::View view(sf::FloatRect(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT));
    window.setView(view);

    Game game;
    game.loadLevel("Assets/Data/Start.tmx");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Time deltaTime = clock.getElapsedTime();
        clock.restart();

        game.update(deltaTime);
        game.draw(window);
    }

    return 0;
}