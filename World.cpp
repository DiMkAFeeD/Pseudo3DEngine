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
