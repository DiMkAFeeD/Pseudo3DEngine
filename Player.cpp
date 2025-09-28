#include "Player.h"

void Player::Move()
{
	float deltaTime = clock.restart().asSeconds();
	speed = defaultSpeed * deltaTime;

	float rot = sf::Angle(rotation + sf::degrees(0)).asRadians();
	float sideRot = sf::Angle(rotation + sf::degrees(90)).asRadians();

	float dX = 0, dY = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		dX += cos(rot) * speed; // W
		dY += sin(rot) * speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		dX -= cos(rot) * speed; // S
		dY -= sin(rot) * speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		dX += cos(sideRot) * speed; // D
		dY += sin(sideRot) * speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		dY -= sin(sideRot) * speed;
		dX -= cos(sideRot) * speed; // A
	}

	position.x += dX;
	position.y += dY;
}

void Player::Rotation(sf::RenderWindow& window)
{
	sf::Vector2i center(window.getSize().x / 2, window.getSize().y / 2);
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);

	float dx = static_cast<float>(mousePos.x - center.x);
	float deltaTime = clock2.restart().asSeconds();
	rotation += sf::degrees(dx * deltaTime * sensity);

	sf::Mouse::setPosition(center, window);
}


void Player::drawMiniMap(sf::RenderWindow& window) {
	playerShape.setFillColor(sf::Color::Green);
	playerShape.setPointCount(3);
	playerShape.setPoint(0, sf::Vector2f(0, 10));
	playerShape.setPoint(1, sf::Vector2f(5, -5));
	playerShape.setPoint(2, sf::Vector2f(-5, -5));

	playerShape.setPosition(position);
	playerShape.setRotation(rotation - sf::degrees(90));
	window.draw(playerShape);
}