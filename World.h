#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class World {
private:
	struct Object {
		sf::Vector2f position;
		sf::Color color;
		sf::VertexArray VA;
	};
	std::vector<Object> objects;

public:
	void init(std::string filePath);
	void drawMiniMap(sf::RenderWindow& window);
};
