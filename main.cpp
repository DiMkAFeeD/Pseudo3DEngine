#include <SFML/Graphics.hpp>
#include "World.h"
#include "Player.h"

int main()
{
    std::setlocale(LC_ALL, "RU");
    sf::RenderWindow window(sf::VideoMode({ 1000, 600 }), "Pseudo 3D Engine!");

    World world;
    world.init("assets/map.json");

    Player player;

    sf::Mouse::setPosition(window.getPosition() + sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2));

    sf::View view;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        view.setSize((sf::Vector2f)window.getSize());
        view.setCenter(sf::Vector2f(window.getSize().x/2, window.getSize().y / 2));

        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) player.Rotation(window);
        player.Move();

        window.setView(view);
        window.clear();

        world.drawView(player, window);

        world.drawMiniMap(window);
        player.drawMiniMap(window);

        window.display();
    }
}