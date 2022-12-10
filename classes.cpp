#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "classes.h"
#include <math.h>


//Player constructor
Player::Player(sf::RenderWindow &windowRef, sf::Vector2f &mousePosRef, std::vector<Projectile*> &projectilesRef)
:window(windowRef), mousePos(mousePosRef), projectiles(projectilesRef)
{

    this->texture.loadFromFile("media/spr_character.png");
    this->body.setTexture(this->texture);
    this->body.setTextureRect(sf::IntRect(0, 0, 16, 16));
    this->body.setOrigin(this->body.getPosition().x + 8, this->body.getPosition().y + 8);
    this->body.setPosition(this->window.getPosition().x, this->window.getPosition().y);
    this->body.setScale(2, 2);
}

//Player methods
void Player::update()
{
    //Movement
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        this->body.move(0.f, -1.f*this->movementSpeed);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        this->body.move(0.f, 1.f*this->movementSpeed);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        this->body.move(-1.f*this->movementSpeed, 0.f);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        this->body.move(1.f*this->movementSpeed, 0.f);

    //Looking at mouse direction
    float angle = std::atan2(this->mousePos.x - this->body.getPosition().x, this->mousePos.y - this->body.getPosition().y) * 180/M_PI;
    this->body.setRotation(-angle);

    //Shooting
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        this->shoot();
    }
}

void Player::shoot()
{
    if(this->shootTimer.getElapsedTime().asMilliseconds() > this->fireRate)
    {
        sf::Vector2f velocity = this->mousePos - this->body.getPosition();
        this->projectiles.push_back(new Projectile(this->window, this->body.getPosition(), velocity));
        this->shootTimer.restart();
    }

}

void Player::draw()
{
    this->window.draw(this->body);
}

//Projectile constructor
Projectile::Projectile(sf::RenderWindow &windowRef, sf::Vector2f startPos, sf::Vector2f velocity)
    :window(windowRef)
{
    this->texture.loadFromFile("media/projectiles.png");
    this->body.setTexture(this->texture);
    this->body.setTextureRect(sf::IntRect(16, 8, 8, 8));
    this->body.setOrigin(this->body.getPosition().x + 4, this->body.getPosition().y + 4);
    this->body.setPosition(startPos);
    this->velocity = velocity;
    float dist = std::sqrt(std::pow(velocity.x, 2) + std::pow(velocity.y, 2));
    this->velocityN = velocity/dist;
}

void Projectile::update()
{
    this->body.move(this->velocityN * this->speed);
}

void Projectile::draw()
{
    this->window.draw(this->body);
}
