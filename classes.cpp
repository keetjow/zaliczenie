#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "classes.h"
#include <math.h>

//Entity drawing setup
Entity::Entity(sf::RenderWindow &windowRefBase): window(windowRefBase){};
void Entity::draw()
{
    this->window.draw(this->body);
}


//Player constructor
Player::Player(sf::RenderWindow &windowRef, sf::Vector2f &mousePosRef, std::vector<Projectile*> &projectilesRef)
 :Entity(windowRef), mousePos(mousePosRef), projectiles(projectilesRef)
{
    //Body setup
    this->texture.loadFromFile("media/spr_character.png");
    this->body.setTexture(this->texture);
    this->body.setTextureRect(sf::IntRect(0, 0, 16, 16));
    this->body.setOrigin(this->body.getPosition().x + 8, this->body.getPosition().y + 8);
    this->body.setPosition(this->window.getSize().x/2, this->window.getSize().y/2);
    this->body.setScale(2, 2);
    //Stats setup
    this->movementSpeed = 4.f;
    this->hp = 50.f;
    this->attackPower = 10.f;
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
        this->shoot();
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


//Enemies constructors
Enemy::Enemy(sf::RenderWindow &windowRef, Entity* targetRef)
    :Entity(windowRef), target(targetRef){}

BlueGhost::BlueGhost(sf::RenderWindow &windowRef, Entity* targetRef, sf::Vector2f startPos) : Enemy(windowRef, targetRef)
{
    //Body setup
    this->texture.loadFromFile("media/spr_ghost.png");
    this->body.setTexture(this->texture);
    this->body.setTextureRect(sf::IntRect(0, 0, 16, 16));
    this->body.setOrigin(this->body.getPosition().x + 8, this->body.getPosition().y + 8);
    this->body.setScale(2, 2);
    this->body.setPosition(startPos);
    //Stats setup
    this->movementSpeed = 3.f;
    this->hp = 10.f;
    this->attackPower = 10.f;
}

PurpleGhost::PurpleGhost(sf::RenderWindow &windowRef, Entity* targetRef, sf::Vector2f startPos) : Enemy(windowRef, targetRef)
{
    //Body setup
    this->texture.loadFromFile("media/spr_ghost_hard.png");
    this->body.setTexture(this->texture);
    this->body.setTextureRect(sf::IntRect(0, 0, 16, 16));
    this->body.setOrigin(this->body.getPosition().x + 8, this->body.getPosition().y + 8);
    this->body.setScale(2, 2);
    this->body.setPosition(startPos);
    //Stats setup
    this->movementSpeed = 2.f;
    this->hp = 20.f;
    this->attackPower = 15.f;
}

Slime::Slime(sf::RenderWindow &windowRef, Entity* targetRef, sf::Vector2f startPos) : Enemy(windowRef, targetRef)
{
    //Body setup
    this->texture.loadFromFile("media/spr_blob.png");
    this->body.setTexture(this->texture);
    this->body.setTextureRect(sf::IntRect(0, 0, 16, 16));
    this->body.setOrigin(this->body.getPosition().x + 8, this->body.getPosition().y + 8);
    this->body.setScale(2, 2);
    this->body.setPosition(startPos);
    //Stats setup
    this->movementSpeed = 1.5f;
    this->hp = 50.f;
    this->attackPower = 30.f;
}

//Enemy update
void Enemy::update()
{
    this->velocity = this->target->body.getPosition() - this->body.getPosition();
    float dist = std::sqrt(std::pow(velocity.x, 2) + std::pow(velocity.y, 2));
    sf::Vector2f velocityN = this->velocity/dist;
    this->body.move(velocityN * this->movementSpeed);
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

//Projectile methods
void Projectile::update()
{
    this->body.move(this->velocityN * this->speed);
}

void Projectile::draw()
{
    this->window.draw(this->body);
}
