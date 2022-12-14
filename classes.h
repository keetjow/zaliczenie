#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>

class Projectile
{
    public:
        float speed = 1000;
        float damage;
        sf::Texture texture;
        sf::Sprite body;
        sf::RenderWindow &window;
        sf::Vector2f velocity;
        sf::Vector2f velocityN;
        Projectile(sf::RenderWindow &windowRef, sf::Vector2f startPos, sf::Vector2f velocity, float damageAmount);
        void update(float &dt);
        void draw();
        //debug
        //~Projectile();
};

class Entity
{
    protected:
        sf::RenderWindow &window;
        sf::Texture texture;

    public:
        float hp;
        float movementSpeed;
        float attackPower;
        sf::Sprite body;
        void draw();
        virtual void update(float &dt) = 0;
        Entity(sf::RenderWindow &windowRefBase);
        bool alive = true;
};

class Player : public Entity
{
    public:
        float fireRate = 200;
        sf::Vector2f &mousePos;
        std::vector<Projectile*> &projectiles;
        sf::Clock shootTimer;
        sf::Clock abilityCooldown;
        Player(sf::RenderWindow &windowRef, sf::Vector2f &mousePosRef, std::vector<Projectile*> &projectilesRef);
        void update(float &dt);
        void shoot();
        void useAbility();
};

class Enemy : public Entity
{
    public:
        Entity* target;
        void update(float &dt);
        bool knockbackState = false;
        sf::Clock knockbackTimer;
        Enemy(sf::RenderWindow &windowRef, Entity* targetRef);
        sf::Vector2f velocity;
        sf::Vector2f moveVector;
        void takeDamage(float damageReceiveAmount);
        void punch(Entity* punchTarget);
        //debug
        //~Enemy();
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
