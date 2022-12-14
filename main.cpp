#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "classes.h"
#include <cstdlib>
#include <time.h>

int main()
{
    //Window setup
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8.f;
    sf::RenderWindow gameWindow(sf::VideoMode(1280, 720, 32), "Projekt na zaliczenie", sf::Style::Fullscreen, settings);
    gameWindow.setFramerateLimit(60);
    gameWindow.setVerticalSyncEnabled(true);
    float dt;
    sf::Clock dt_clock;

    //Projectiles setup
    std::vector<Projectile*> projectiles;

    //Enemies setup
    std::vector<Enemy*> enemies;
    sf::Clock enemySpawnRate;
    srand(time(NULL));
    float spawnRate = 3;

    //Player setup
    sf::Vector2f mousePos;
    Entity* player = new Player(gameWindow, mousePos, projectiles);

    //Main loop
    while(gameWindow.isOpen())
    {
        dt = dt_clock.restart().asSeconds();
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
        player->update(dt);
        //Spawning enemies
        if(enemySpawnRate.getElapsedTime().asSeconds() > spawnRate)
        {
            sf::Vector2f randomPosition(rand()%1281, rand()%721);
            int randomNumber = rand()%3;
            switch(randomNumber)
            {
                case 0:
                {
                    enemies.push_back(new BlueGhost(gameWindow, player, randomPosition));
                    break;
                }
                case 1:
                {
                    enemies.push_back(new PurpleGhost(gameWindow, player, randomPosition));
                    break;
                }
                case 2:
                {
                    enemies.push_back(new Slime(gameWindow, player, randomPosition));
                    break;
                }
            }
            enemySpawnRate.restart();
        }

        for(int i = 0; i < projectiles.size(); i++)
        {
            projectiles[i]->update(dt);
            if(projectiles[i]->body.getPosition().x > 1280 || projectiles[i]->body.getPosition().x < 0 || projectiles[i]->body.getPosition().y > 720 || projectiles[i]->body.getPosition().y < 0)
            {
                delete projectiles[i];
                projectiles.erase(projectiles.begin() + i);
            }
        }

        for(int i = 0; i < enemies.size(); i++)
        {
            enemies[i]->update(dt);
            for(int j = 0; j < projectiles.size(); j++)
            {
                if(enemies[i]->body.getGlobalBounds().intersects(projectiles[j]->body.getGlobalBounds()))
                {
                    enemies[i]->takeDamage(projectiles[j]->damage);
                    if(!enemies[i]->alive)
                    {
                        delete enemies[i];
                        enemies.erase(enemies.begin() + i);
                    }
                    delete projectiles[j];
                    projectiles.erase(projectiles.begin() + j);
                    break;
                }
            }
        }
        if(player->hp < 0)
            gameWindow.close();


        //Drawing sprites
        gameWindow.clear();

        player->draw();

        for(int i = 0; i < projectiles.size(); i++)
            projectiles[i]->draw();

        for(int i = 0; i < enemies.size(); i++)
            enemies[i]->draw();

        //Final frame display
        gameWindow.display();
    }
    return 0;
}
