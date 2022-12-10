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
        float hp = 5.f;
        float movementSpeed = 10.f;
        float attackPower = 10.f;

        sf::Texture texture;
        sf::Sprite body;

    public:
        virtual void draw() = 0;
        virtual void update() = 0;
};

class Player : public Entity
{
    public:
        float fireRate = 200;
        sf::RenderWindow &window;
        sf::Vector2f &mousePos;
        std::vector<Projectile*> &projectiles;
        sf::Clock shootTimer;
        Player(sf::RenderWindow &windowRef, sf::Vector2f &mousePosRef, std::vector<Projectile*> &projectiles);
        virtual void update();
        virtual void draw();
        void shoot();
};





