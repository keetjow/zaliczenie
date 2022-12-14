#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "classes.h"
#include <math.h>
//#include <iostream>  //memory release debug

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
    this->body.setScale(3, 3);
    //Stats setup
    this->movementSpeed = 550.f;
    this->hp = 50.f;
    this->attackPower = 10.f;
}

//Player methods
void Player::update(float &dt)
{
    //Movement
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        this->body.move(0.f, -1.f*this->movementSpeed*dt);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        this->body.move(0.f, 1.f*this->movementSpeed*dt);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        this->body.move(-1.f*this->movementSpeed*dt, 0.f);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        this->body.move(1.f*this->movementSpeed*dt, 0.f);

    //Looking at mouse direction
    float angle = std::atan2(this->mousePos.x - this->body.getPosition().x, this->mousePos.y - this->body.getPosition().y) * 180/M_PI;
    this->body.setRotation(-angle);

    //Shooting
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        this->shoot();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        this->useAbility();
}

void Player::shoot()
{
    if(this->shootTimer.getElapsedTime().asMilliseconds() > this->fireRate)
    {
        sf::Vector2f velocity = this->mousePos - this->body.getPosition();
        this->projectiles.push_back(new Projectile(this->window, this->body.getPosition(), velocity, this->attackPower));
        this->shootTimer.restart();
    }
}

void Player::useAbility()
{
    if(this->abilityCooldown.getElapsedTime().asSeconds() > 4)
    {
        for(int i = 0; i < 16; i++)
        {
            float angle = i*M_PI/8;
            sf::Vector2f velocity = sf::Vector2f(std::sin(angle), std::cos(angle)*(-1));
            this->projectiles.push_back(new Projectile(this->window, this->body.getPosition(), velocity, this->attackPower*3));
        }
        abilityCooldown.restart();
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
    this->body.setScale(3, 3);
    this->body.setPosition(startPos);
    //Stats setup
    this->movementSpeed = 250.f;
    this->hp = 10.f;
    this->maxHp = 10.f;
    this->attackPower = 10.f;
}

PurpleGhost::PurpleGhost(sf::RenderWindow &windowRef, Entity* targetRef, sf::Vector2f startPos) : Enemy(windowRef, targetRef)
{
    //Body setup
    this->texture.loadFromFile("media/spr_ghost_hard.png");
    this->body.setTexture(this->texture);
    this->body.setTextureRect(sf::IntRect(0, 0, 16, 16));
    this->body.setOrigin(this->body.getPosition().x + 8, this->body.getPosition().y + 8);
    this->body.setScale(3, 3);
    this->body.setPosition(startPos);
    //Stats setup
    this->movementSpeed = 220.f;
    this->hp = 20.f;
    this->maxHp = 20.f;
    this->attackPower = 20.f;
}

Slime::Slime(sf::RenderWindow &windowRef, Entity* targetRef, sf::Vector2f startPos) : Enemy(windowRef, targetRef)
{
    //Body setup
    this->texture.loadFromFile("media/spr_blob.png");
    this->body.setTexture(this->texture);
    this->body.setTextureRect(sf::IntRect(0, 0, 16, 16));
    this->body.setOrigin(this->body.getPosition().x + 8, this->body.getPosition().y + 8);
    this->body.setScale(6, 6);
    this->body.setPosition(startPos);
    //Stats setup
    this->movementSpeed = 190.f;
    this->hp = 50.f;
    this->maxHp = 50.f;
    this->attackPower = 30.f;
}

//Enemy update
void Enemy::update(float &dt)
{
    this->velocity = this->target->body.getPosition() - this->body.getPosition();
    float dist = std::sqrt(std::pow(velocity.x, 2) + std::pow(velocity.y, 2));
    sf::Vector2f velocityN = this->velocity/dist;
    if(dist > 10 && !this->knockbackState)
        this->moveVector = velocityN * this->movementSpeed;

    if(this->body.getGlobalBounds().intersects(this->target->body.getGlobalBounds()))
    {
        this->knockbackTimer.restart();
        this->target->hp -= this->attackPower;
        this->knockbackState = true;
        this->moveVector = velocityN * (-1.f) * this->movementSpeed;
    }
    if(knockbackTimer.getElapsedTime().asMilliseconds() > 500)
        this->knockbackState = false;

    this->body.move(this->moveVector*dt);
}

//Enemy damage managment
void Enemy::takeDamage(float damageReceiveAmount)
{
    if(this->body.getScale().x > 2.5f && this->body.getScale().y > 2.5f)
        this->body.setScale(this->body.getScale().x - this->body.getScale().x/10, this->body.getScale().y - this->body.getScale().y/10);

    this->hp -= damageReceiveAmount;
    this->movementSpeed -= 10;
    if(this->hp <= 0)
        this->alive = false;
}

//Projectile constructor
Projectile::Projectile(sf::RenderWindow &windowRef, sf::Vector2f startPos, sf::Vector2f velocity, float damageAmount)
    :window(windowRef), damage(damageAmount)
{
    this->texture.loadFromFile("media/projectiles.png");
    this->body.setTexture(this->texture);
    this->body.setTextureRect(sf::IntRect(16, 8, 8, 8));
    this->body.setOrigin(this->body.getPosition().x + 4, this->body.getPosition().y + 4);
    this->body.setPosition(startPos);
    this->body.setScale(2, 2);
    this->velocity = velocity;
    float dist = std::sqrt(std::pow(velocity.x, 2) + std::pow(velocity.y, 2));
    this->velocityN = velocity/dist;
}

//Projectile methods
void Projectile::update(float &dt)
{
    this->body.move(this->velocityN * this->speed*dt);
}

void Projectile::draw()
{
    this->window.draw(this->body);
}

float distanceBetween(sf::Vector2f a, sf::Vector2f b)
{
    sf::Vector2f vel = a - b;
    float distance = std::sqrt(std::pow(vel.x, 2) + std::pow(vel.y, 2));
    return distance;

}


//debug memory release
/*
Projectile::~Projectile()
{
    std::cout<<"Projectile out"<<std::endl;
}
Enemy::~Enemy()
{
    std::cout<<"Enemy dead"<<std::endl;
}
*/
