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

    auto &entityManager = engine.getEntityManager();

    for (int i = 0; i < entityManager.getEntityCount(); ++i) {
        auto &entity = entityManager.getEntity(i);

        if (entity.has<Dummy>()) {
            auto &dummy = entityManager.getComponent<Dummy>(entity);

            sf::CircleShape circle(4);
            circle.setFillColor(green);
            circle.setPosition(dummy.x - circle.getRadius(),
                               dummy.y - circle.getRadius());

            window.draw(circle);
        }
    }
}

void Overlay::onMouseMove(Engine &engine, sf::Event &event) {
    mouseX = event.mouseMove.x;
    mouseY = event.mouseMove.y;
}

void Overlay::onClick(Engine &engine, sf::Event &event) {
    lastClickedX = event.mouseButton.x;
    lastClickedY = event.mouseButton.y;

    auto &entityManager = engine.getEntityManager();

    auto &entity = entityManager.createEntity();

    if (entityManager.getEntityCount() > 12) {
        for (int i = 1; i < 8; ++i) {
            auto &e = entityManager.getEntity(i);

            if (e.has<Dummy>()) {
                entityManager.destroyComponent<Dummy>(e);
            }
        }
    }

    if (entityManager.getEntityCount() > 24) {
        for (int i = 3; i < entityManager.getEntityCount() - 2; ++i) {
            entityManager.destroyEntity(entityManager.getEntity(i));
        }
    } else {
        entityManager.createComponent<Dummy>(entity, lastClickedX,
                                             lastClickedY);
    }
}

std::string Overlay::getString(Engine &engine) {
    auto &entityManager = engine.getEntityManager();

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
                  std::to_string(lastClickedY) + "\n" + "Entity count: " +
                  std::to_string(entityManager.getEntityCount()) + "\n";

    for (int i = 0; i < entityManager.getEntityCount(); ++i) {
        auto &entity = entityManager.getEntity(i);
        result += "\n Entity #" + std::to_string(entity.getIndex()) +
                  " with dummy " + std::to_string(entity.get<Dummy>());
    }

    return result;
}