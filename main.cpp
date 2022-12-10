#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "classes.h"

int main()
{
    //Window setup
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8.f;
    sf::RenderWindow gameWindow(sf::VideoMode(1280, 720, 32), "Projekt na zaliczenie", sf::Style::Fullscreen, settings);
    gameWindow.setFramerateLimit(60);
    gameWindow.setVerticalSyncEnabled(true);

    //Projectiles setup
    std::vector<Projectile*> projectiles;

    //Player setup
    sf::Vector2f mousePos;
    Player player(gameWindow, mousePos, projectiles);

    //Main loop
    while(gameWindow.isOpen())
    {
        //Event loop
        sf::Event event;
        while(gameWindow.pollEvent(event))
        {
            if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                gameWindow.close();
        }

        //Update mouse position
        mousePos = sf::Vector2f(sf::Mouse::getPosition(gameWindow));

        //Game logic
        player.update();

        for(int i = 0; i < projectiles.size(); i++)
            projectiles[i]->update();

        //Drawing sprites
        gameWindow.clear();

        player.draw();

        for(int i = 0; i < projectiles.size(); i++)
            projectiles[i]->draw();

        //Final frame display
        gameWindow.display();
    }
    return 0;
}
