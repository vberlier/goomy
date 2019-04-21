#include "main.h"
#include <SFML/Graphics.hpp>

void ExampleSystem::update(Engine &engine) {
    sf::CircleShape shape(50.f);
    shape.setFillColor(sf::Color(100, 250, 50));
    engine.get<Window>().draw(shape);
}
