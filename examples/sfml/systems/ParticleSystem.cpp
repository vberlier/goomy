#include "main.h"

#include <SFML/Graphics.hpp>

void ParticleSystem::onUpdate(Engine &engine, Particle &particle) {
    sf::CircleShape circle(6);
    circle.setPosition(particle.x - circle.getRadius(),
                       particle.y - circle.getRadius());
    circle.setFillColor(engine.get<Overlay>().color);

    engine.get<Window>().draw(circle);
}
void ParticleSystem::onClick(Engine &engine, sf::Event &event) {
    engine.entity()
        .with<Particle>(event.mouseButton.x, event.mouseButton.y)
        .with<Age>(std::chrono::seconds(3));
}
