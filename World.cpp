#include "World.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

void World::init(std::string filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << filePath << "\n";
        return;
    }

    json j;
    file >> j;

    for (auto& obj : j) {
        Object o;

        // позиция
        o.position.x = obj["position"]["X"].get<float>();
        o.position.y = obj["position"]["Y"].get<float>();

        // цвет
        int r = obj["color"]["R"].get<int>();
        int g = obj["color"]["G"].get<int>();
        int b = obj["color"]["B"].get<int>();
        o.color = sf::Color(r, g, b);

        std::string type = obj["type"].get<std::string>();

        if (type == "square") {
            float w = obj["size"]["X"].get<float>();
            float h = obj["size"]["Y"].get<float>();

            o.VA.setPrimitiveType(sf::PrimitiveType::LineStrip);
            o.VA.append(sf::Vertex({ o.position.x, o.position.y }, o.color));
            o.VA.append(sf::Vertex({ o.position.x + w, o.position.y }, o.color));
            o.VA.append(sf::Vertex({ o.position.x + w, o.position.y + h }, o.color));
            o.VA.append(sf::Vertex({ o.position.x, o.position.y + h }, o.color));
            o.VA.append(sf::Vertex({ o.position.x, o.position.y }, o.color));
        }
        else if (type == "circle") {
            float radius = obj["radius"].get<float>();

            o.VA.setPrimitiveType(sf::PrimitiveType::LineStrip);
            const int points = obj["points"];
            for (int i = 0; i <= points; i++) {
                float angle = i * 2 * 3.14159f / points;
                float x = o.position.x + cos(angle) * radius;
                float y = o.position.y + sin(angle) * radius;
                o.VA.append(sf::Vertex({ x, y }, o.color));
            }
        }
        else if (type == "custom") {
            o.VA.setPrimitiveType(sf::PrimitiveType::LineStrip);
            for (auto& v : obj["VA"]) {
                float x = v["X"].get<float>() + o.position.x;
                float y = v["Y"].get<float>() + o.position.y;
                o.VA.append(sf::Vertex({ x, y }, o.color));
            }
        }
        else {
            std::cerr << "Неизвестный тип: " << type << "\n";
            continue;
        }

        objects.push_back(std::move(o));
    }
}

void World::drawMiniMap(sf::RenderWindow& window)
{
    for (auto obj : objects) {
        sf::VertexArray VA = obj.VA;
        for (size_t i = 0; i < VA.getVertexCount(); i++) {
            VA[i].position += obj.position;
        }
        window.draw(VA);
    }
}

float World::castRay(float length, sf::Angle angle, sf::Vector2f origin) {
    sf::Vector2f dir(std::cos(angle.asRadians()), std::sin(angle.asRadians()));

    float minT = length;
    bool hit = false;

    for (auto& obj : objects) {
        for (unsigned i = 0; i < obj.VA.getVertexCount(); i++) {
            sf::Vector2f a = obj.VA[i].position + obj.position;
            sf::Vector2f b = obj.VA[(i + 1) % obj.VA.getVertexCount()].position + obj.position;

            sf::Vector2f v1 = origin - a;
            sf::Vector2f v2 = b - a;
            sf::Vector2f v3(-dir.y, dir.x);

            float dot = v2.x * v3.x + v2.y * v3.y;
            if (std::abs(dot) < 1e-6f) continue;

            float t1 = (v2.x * v1.y - v2.y * v1.x) / dot;
            float t2 = (v1.x * v3.x + v1.y * v3.y) / dot;

            if (t1 >= 0 && t1 <= minT && t2 >= 0 && t2 <= 1) {
                minT = t1;
                hit = true;
            }
        }
    }

    return hit ? minT : length;
}

void World::drawView(Player& player, sf::RenderWindow& window)
{
    sf::VertexArray line(sf::PrimitiveType::Lines, 2);
    line[0].position = player.getPosition();
    line[0].color = sf::Color::Red;

    sf::Angle FOV = player.getFOV();
    sf::Angle startAngle = player.getRotation() - FOV / 2;
    int numLines = window.getSize().x;
    sf::Angle stepAngle = FOV / numLines;
    float distanceView = 100;

    for (int i = 0; i < numLines; i++) {
        sf::Angle angleRay = startAngle + stepAngle * i;

        float distance = castRay(distanceView, angleRay, player.getPosition());
        if (distance < 0.001f) distance = 0.001f;

        sf::Vertex endV;
        endV.position = player.getPosition() + sf::Vector2f(cos(angleRay.asRadians()) * distance, 
            sin(angleRay.asRadians()) * distance);
        endV.color = sf::Color::Red;
        line[1] = endV;

        window.draw(line); 

        sf::VertexArray viewLine(sf::PrimitiveType::Lines, 2);

        float stepX = static_cast<float>(window.getSize().x) / numLines;
        float xPos = i * stepX;

        float yCenter = window.getSize().y / 2;

        sf::Vertex topVert, bottomVert;
        topVert.position.x = bottomVert.position.x = xPos;

        std::uint8_t shadeInt = static_cast<std::uint8_t>(std::max(0.f, 255.f * (1 - distance / distanceView)));
        topVert.color = bottomVert.color = sf::Color(shadeInt, shadeInt, shadeInt);

        float deltaAngle = (angleRay - player.getRotation()).asRadians();
        float correctedDistance = distance * cos(deltaAngle);

        correctedDistance = std::max(0.001f, correctedDistance);

        float distanceToProjectionPlane = (window.getSize().x / 2) / tan(FOV.asRadians() / 2);
        float wallHeight = (distanceToProjectionPlane / correctedDistance) * 10;

        topVert.position.y = yCenter - wallHeight;
        bottomVert.position.y = yCenter + wallHeight;

        viewLine[0] = topVert;
        viewLine[1] = bottomVert;

        if(distance != distanceView)
        window.draw(viewLine);
    }
}
