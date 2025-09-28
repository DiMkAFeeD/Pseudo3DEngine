#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Player.h"

class World {
private:
	struct Object {
		sf::Vector2f position;
		sf::Color color;
		sf::VertexArray VA;
	};
	std::vector<Object> objects;

	float castRay(float lenght, sf::Angle angle, sf::Vector2f origin);
public:
	void init(std::string filePath);
	void drawMiniMap(sf::RenderWindow& window);
	void drawView(Player& player, sf::RenderWindow& window);
};
