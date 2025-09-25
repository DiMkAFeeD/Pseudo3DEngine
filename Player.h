#pragma once

#include <SFML/Graphics.hpp>

class Player {
private:
	sf::Vector2f position = sf::Vector2f(50,50);
	sf::Angle rotation = sf::degrees(180);
	
	sf::Clock clock;
	sf::Clock clock2;
	
	float defaultSpeed = 25;
	float speed = 0;
	float sensity = 15;

	sf::ConvexShape playerShape;

public:
	void Move();
	void Rotation(sf::RenderWindow& window);
	void drawMiniMap(sf::RenderWindow& window);
};