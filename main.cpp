#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <time.h>

class Bullet
{
public:
    sf::Texture bulletTexture;
    sf::Sprite bulletBody;

    sf::Vector2f velocity;
    float speed = 10.f;

    Bullet(sf::Vector2f velocity, sf::Vector2f position)
    {
        this->bulletTexture.loadFromFile("spr_orange.png");
        this->bulletBody.setTexture(this->bulletTexture);
        this->bulletBody.setOrigin(8.f, 8.f);
        this->velocity = velocity;
        this->bulletBody.setPosition(position);
    }
};

class Enemy
{
public:
    sf::Texture enemyTexture;
    sf::Sprite enemyBody;

    float speed = 2.f;

    sf::Vector2f velocity(sf::Vector2f playerPos)
    {
        sf::Vector2f vel = playerPos - sf::Vector2f(this->enemyBody.getPosition().x, this->enemyBody.getPosition().y);
        float dif = std::sqrt(std::pow(vel.x, 2) + std::pow(vel.y, 2));
        return vel/dif;
    }

    Enemy()
    {
        int randomnumber = rand()%3;
        switch(randomnumber)
        {
        case 0:
            {
                this->enemyTexture.loadFromFile("spr_ghost_hard.png");
                break;
            }
        case 1:
            {
                this->enemyTexture.loadFromFile("spr_ghost.png");
                break;
            }
        case 2:
            {
                this->enemyTexture.loadFromFile("spr_blob.png");
                break;
            }
        }
        this->enemyBody.setTexture(enemyTexture);
        this->enemyBody.setTextureRect(sf::IntRect(0, 0, 16, 16));
        this->enemyBody.setPosition(rand()%1280, rand()%720);
    }
};






int main()
{
    srand(time(NULL));
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8.f;

    sf::RenderWindow okno(sf::VideoMode(1280, 720, 32), "Testowa gra", sf::Style::Fullscreen, settings);
    okno.setFramerateLimit(60);
    okno.setVerticalSyncEnabled(true);

    sf::Texture playerTexture;
    playerTexture.loadFromFile("player.png");
    sf::IntRect playerAnim(0, 0, 16, 16);


    sf::Sprite player;
    player.setTexture(playerTexture);
    player.setTextureRect(playerAnim);
    player.setOrigin(player.getPosition().x + 8, player.getPosition().y + 8);
    player.setPosition(okno.getSize().x/2, okno.getSize().y/2);
    player.scale(2, 2);
    float playerSpeed = 5.f;
    float fireRate = 100;

    sf::Clock shootClock;
    sf::Clock enemySpawnClock;

    std::vector<Enemy*> enemies;
    float enemySpawnRate = 1;

    std::vector<Bullet*> bullets;

    sf::Event event;
    while(okno.isOpen())
    {
        while(okno.pollEvent(event))
        {
            if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                okno.close();
        }
        sf::Vector2f playerPos = sf::Vector2f(player.getPosition());
        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(okno));
        sf::Vector2f velocity = mousePos - playerPos;
        float velocityLength = std::sqrt(std::pow(velocity.x, 2) + std::pow(velocity.y, 2));

        float playerAngle = std::atan2(velocity.x, velocity.y) * 180/M_PI;
        player.setRotation(-playerAngle);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            player.move(0.f, -1.f*playerSpeed);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            player.move(0.f, 1.f*playerSpeed);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            player.move(-1.f*playerSpeed, 0.f);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            player.move(1.f*playerSpeed, 0.f);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            fireRate = 50;
        else
            fireRate = 200;

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && shootClock.getElapsedTime().asMilliseconds() > fireRate)
        {
            bullets.push_back(new Bullet(velocity/velocityLength, playerPos));
            shootClock.restart();
        }

        if(enemySpawnClock.getElapsedTime().asSeconds() > 1)
        {
            enemies.push_back(new Enemy);
            enemySpawnClock.restart();
        }

        for(int i = 0; i < bullets.size(); i++)
        {
            bullets[i]->bulletBody.move(bullets[i]->velocity * bullets[i]->speed);
        }

        for(int i = 0; i < enemies.size(); i++)
        {
            enemies[i]->enemyBody.move(enemies[i]->velocity(playerPos) * enemies[i]->speed);
        }


        okno.clear(sf::Color::Black);

        for(int i = 0; i < bullets.size(); i++)
        {
            okno.draw(bullets[i]->bulletBody);
        }

        for(int i = 0; i < enemies.size(); i++)
        {
            okno.draw(enemies[i]->enemyBody);
        }

        okno.draw(player);
        okno.display();
    }


    return 0;
}
