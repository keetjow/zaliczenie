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
    sf::Sprite bullet;

    sf::Vector2f velocity;
    float speed = 10.f;

    Bullet()
    {
        this->bulletTexture.loadFromFile("spr_orange.png");
        this->bullet.setTexture(this->bulletTexture);
        this->bullet.setOrigin(8.f, 8.f);
    }
};

class Enemy
{

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

    sf::Clock clock;

    Bullet originBullet;
    std::vector<Bullet> bullets;

    Bullet b1;


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

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && clock.getElapsedTime().asMilliseconds() > fireRate)
        {
            b1.bullet.setPosition(playerPos);
            b1.velocity = velocity/velocityLength;
            bullets.push_back(Bullet(b1));
            clock.restart();
        }

        for(int i = 0; i < bullets.size(); i++)
        {
            bullets[i].bullet.move(bullets[i].velocity * bullets[i].speed);
        }


        okno.clear(sf::Color::Black);

        for(int i = 0; i < bullets.size(); i++)
        {
            okno.draw(bullets[i].bullet);
        }

        okno.draw(player);
        okno.display();
    }


    return 0;
}
