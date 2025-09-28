#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Player.h" // ���������� ����� ������

class SkyBox {
private:
    sf::Texture skyboxTex;
    sf::Sprite skyboxSprite;
    sf::Color floorColor;
    bool isFloor = false;

public:
    SkyBox(const std::string& texPath) : skyboxSprite(skyboxTex) {
        skyboxTex.loadFromFile(texPath);
        skyboxTex.setRepeated(true); // �������� ���������� ��������
        skyboxSprite.setTexture(skyboxTex);
    }

    SkyBox(const std::string& texPath, sf::Color floorColor)
        : floorColor(floorColor), isFloor(true), skyboxSprite(skyboxTex)
    {
        skyboxTex.loadFromFile(texPath);
        skyboxTex.setRepeated(true);
        skyboxSprite.setTexture(skyboxTex);
    }

    // ������� ���������, ����������� ������� ������
    void draw(sf::RenderWindow& window, Player& player) {
        sf::Vector2u winSize = window.getSize();

        // ������������ ������ �� ������ ����, ������ ��������������� ��������
        float scaleX = static_cast<float>(winSize.x) / skyboxTex.getSize().x;
        float scaleY = static_cast<float>(winSize.y) / skyboxTex.getSize().y / 2;
        skyboxSprite.setScale(sf::Vector2f(scaleX, scaleY));

        // �������� �������� �� X � ����������� �� �������� ������
        float rotationDegrees = player.getRotation().asDegrees();
        // ����������� ���� � �������� [0, 360)
        while (rotationDegrees < 0) rotationDegrees += 360.f;
        while (rotationDegrees >= 360.f) rotationDegrees -= 360.f;

        // ��������� �������� ��������
        float offsetX = (rotationDegrees / 360.f) * skyboxTex.getSize().x;
        skyboxSprite.setTextureRect(sf::IntRect({ {static_cast<int>(offsetX), 0}, {(int)skyboxTex.getSize().x, (int)skyboxTex.getSize().y}}));

        window.draw(skyboxSprite);

        // ������ ���, ���� �����
        if (isFloor) {
            sf::RectangleShape floor(sf::Vector2f(winSize.x, winSize.y / 2.f));
            floor.setPosition({ 0, winSize.y / 2.f });
            floor.setFillColor(floorColor);
            window.draw(floor);
        }
    }
};
