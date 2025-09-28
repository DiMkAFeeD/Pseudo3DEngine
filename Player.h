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
	float sensity = 25;

	sf::ConvexShape playerShape;

	sf::Angle FOV = sf::degrees(90);

public:
	void Move();
	void Rotation(sf::RenderWindow& window);
	void drawMiniMap(sf::RenderWindow& window);

	sf::Vector2f getPosition() { return position; }
	sf::Angle getRotation() { return rotation; }
	sf::Angle getFOV() { return FOV; }
};