#include <SFML/Graphics.hpp>
#include "World.h"

int main()
{
    std::setlocale(LC_ALL, "RU");
    sf::RenderWindow window(sf::VideoMode({ 1000, 600 }), "Pseudo 3D Engine!");

    World world;
    world.init("assets/map.json");

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        world.drawMiniMap(window);
        window.display();
    }
}