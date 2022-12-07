#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>


int main()
{
    sf::RenderWindow okno(sf::VideoMode(800, 600, 32), "TEST");//, sf::Style::Fullscreen);
    okno.setFramerateLimit(60);

    sf::Texture carTexture;
    carTexture.loadFromFile("car.png");

    sf::Sprite car;
    car.setTexture(carTexture);
    car.setOrigin(car.getPosition().x +16, car.getPosition().y + 41);
    car.setPosition(400-16, 300-41);
    float carSpeed = 10;

    sf::Event event;
    while(okno.isOpen())
    {
        while(okno.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                okno.close();
        }
        sf::Vector2f carPos = sf::Vector2f(car.getPosition());
        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(okno));
        sf::Vector2f velocity = sf::Vector2f(mousePos.x-carPos.x, mousePos.y-carPos.y);
        float velocityDist = std::sqrt(std::pow(velocity.x, 2) + std::pow(velocity.y, 2));
        sf::Vector2f step = velocity / velocityDist;

        float carAngle = std::atan2(velocity.x, velocity.y) * 180/M_PI * (-1) + 180;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && velocityDist >= 41)
            car.move(carSpeed*step);

        car.setRotation(carAngle);

        okno.clear(sf::Color::Black);
        okno.draw(car);
        okno.display();
    }


    return 0;
}
