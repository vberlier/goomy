#include "main.h"

void Display::onUpdate(Engine &engine) {
    auto &window = engine.get<Window>();
    auto &resourceManager = engine.get<ResourceManager>();

    sf::Text text(stats(engine), resourceManager.getDefaultFont());
    text.setFillColor(color);
    text.setCharacterSize(16);

    window.draw(text);
}

void Display::onUpdate(Engine &engine, Particle &particle) {
    sf::CircleShape circle(6);
    circle.setPosition(particle.x - circle.getRadius(),
                       particle.y - circle.getRadius());
    circle.setFillColor(color);

    engine.get<Window>().draw(circle);
}

std::string Display::stats(Engine &engine) {
    using namespace std::chrono;

    auto elapsed = duration_cast<seconds>(engine.getAge()).count();

    auto frameDuration =
        duration_cast<milliseconds>(engine.getFrameDuration()).count();

    return "Elapsed: " + std::to_string(elapsed) + "s" +
           "\nFrame duration: " + std::to_string(frameDuration) + "ms" +
           "\nFrames per second: " +
           std::to_string((int)(1 / engine.deltaTime())) +
           "\nEntity count: " + std::to_string(engine.getEntityCount());
}