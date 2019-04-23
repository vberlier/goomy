#include "main.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>

using namespace std::chrono;

Overlay::Overlay() : mouseX(0), mouseY(0), lastClickedX(0), lastClickedY(0) {
}

void Overlay::onUpdate(Engine &engine) {
    auto &window = engine.get<Window>();
    auto &resourceManager = engine.get<ResourceManager>();

    sf::Text text(getString(engine), resourceManager.getDefaultFont());
    text.setFillColor(sf::Color(100, 250, 50));
    text.setCharacterSize(16);

    window.draw(text);
}

void Overlay::onMouseMove(Engine &engine, sf::Event &event) {
    mouseX = event.mouseMove.x;
    mouseY = event.mouseMove.y;
}

void Overlay::onClick(Engine &engine, sf::Event &event) {
    lastClickedX = event.mouseButton.x;
    lastClickedY = event.mouseButton.y;

    auto &entity = engine.createEntity();

    if (engine.entityCount() % 5 == 4) {
        entity.set<Dummy>(duration_cast<seconds>(engine.getAge()).count());

        for (int i = engine.entityCount() - 3; i < engine.entityCount(); ++i) {
            engine.destroyEntity(engine.getEntity(i));
        }
    }
}

std::string Overlay::getString(Engine &engine) {
    auto elapsed = duration_cast<seconds>(engine.getAge()).count();

    auto frameDuration =
        duration_cast<milliseconds>(engine.getFrameDuration()).count();

    auto framesPerSecond =
        std::round((float)duration_cast<milliseconds>(seconds(1)).count() /
                   (float)frameDuration);

    auto result = "Elapsed: " + std::to_string(elapsed) + "s\n" +
                  "Frame duration: " + std::to_string(frameDuration) + "ms\n" +
                  "Frames per second: " + std::to_string((int)framesPerSecond) +
                  "\n" + "Mouse position: " + std::to_string(mouseX) + " " +
                  std::to_string(mouseY) + "\n" +
                  "Last click: " + std::to_string(lastClickedX) + " " +
                  std::to_string(lastClickedY) + "\n" +
                  "Entity count: " + std::to_string(engine.entityCount()) +
                  "\n";

    for (int i = 0; i < engine.entityCount(); ++i) {
        auto &entity = engine.getEntity(i);
        result += "\n Entity #" + std::to_string(entity.getIndex()) +
                  " with dummy " + std::to_string(entity.get<Dummy>());
    }

    return result;
}