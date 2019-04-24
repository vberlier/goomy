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

    sf::Color green(100, 250, 50);

    sf::Text text(getString(engine), resourceManager.getDefaultFont());
    text.setFillColor(green);
    text.setCharacterSize(16);

    window.draw(text);

    for (auto dummy : engine.components<Dummy>()) {
        sf::CircleShape circle(4);
        circle.setFillColor(green);
        circle.setPosition(dummy.data().x - circle.getRadius(),
                           dummy.data().y - circle.getRadius());

        window.draw(circle);
    }
}

void Overlay::onMouseMove(Engine &engine, sf::Event &event) {
    mouseX = event.mouseMove.x;
    mouseY = event.mouseMove.y;
}

void Overlay::onClick(Engine &engine, sf::Event &event) {
    lastClickedX = event.mouseButton.x;
    lastClickedY = event.mouseButton.y;

    engine.entity()
        .with<Dummy>(lastClickedX, lastClickedY)
        .with<Age>(std::chrono::seconds(3));
}

std::string Overlay::getString(Engine &engine) {
    auto elapsed = duration_cast<seconds>(engine.getAge()).count();

    auto frameDuration =
        duration_cast<milliseconds>(engine.getFrameDuration()).count();

    auto result =
        "Elapsed: " + std::to_string(elapsed) + "s\n" +
        "Frame duration: " + std::to_string(frameDuration) + "ms\n" +
        "Frames per second: " + std::to_string((int)(1 / engine.deltaTime())) +
        "\n" + "Mouse position: " + std::to_string(mouseX) + " " +
        std::to_string(mouseY) + "\n" +
        "Last click: " + std::to_string(lastClickedX) + " " +
        std::to_string(lastClickedY) + "\n" +
        "Entity count: " + std::to_string(engine.getEntityCount()) + "\n";

    return result;
}