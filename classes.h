#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>

class Projectile
{
    public:
        float speed = 10;
        sf::Texture texture;
        sf::Sprite body;
        sf::RenderWindow &window;
        sf::Vector2f velocity;
        sf::Vector2f velocityN;
        Projectile(sf::RenderWindow &windowRef, sf::Vector2f startPos, sf::Vector2f velocity);
        void update();
        void draw();
};

class Entity
{
    protected:
        float hp;
        float movementSpeed;
        float attackPower;
        sf::RenderWindow &window;
        sf::Texture texture;

    public:
        sf::Sprite body;
        void draw();
        virtual void update() = 0;
        Entity(sf::RenderWindow &windowRefBase);
};

class Player : public Entity
{
    public:
        float fireRate = 200;
        sf::Vector2f &mousePos;
        std::vector<Projectile*> &projectiles;
        sf::Clock shootTimer;
        Player(sf::RenderWindow &windowRef, sf::Vector2f &mousePosRef, std::vector<Projectile*> &projectilesRef);
        void update();
        void shoot();
};

class Enemy : public Entity
{
    public:
        Entity* target;
        void update();
        Enemy(sf::RenderWindow &windowRef, Entity* targetRef);
        sf::Vector2f velocity;
};

class BlueGhost : public Enemy
{
    public:
    BlueGhost(sf::RenderWindow &windowRef, Entity* targetRef, sf::Vector2f startPos);
};

class PurpleGhost : public Enemy
{
    public:
    PurpleGhost(sf::RenderWindow &windowRef, Entity* targetRef, sf::Vector2f startPos);
};

class Slime : public Enemy
{
    public:
    Slime(sf::RenderWindow &windowRef, Entity* targetRef, sf::Vector2f startPos);
};





