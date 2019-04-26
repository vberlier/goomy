#include "main.h"

void InputEventsDispatcher::onMouseMove(Engine &engine, sf::Event &event) {
    mouseX = event.mouseMove.x;
    mouseY = event.mouseMove.y;

    if (leftButtonPressed) {
        engine.dispatch<onLeftMouseDrag>(mouseX, mouseY);
    }

    if (rightButtonPressed) {
        engine.dispatch<onRightMouseDrag>(mouseX, mouseY);
    }
}

void InputEventsDispatcher::onMouseDown(Engine &engine, sf::Event &event) {
    mouseX = event.mouseButton.x;
    mouseY = event.mouseButton.y;

    if (event.mouseButton.button == sf::Mouse::Left) {
        engine.dispatch<onLeftMouseDown>(mouseX, mouseY);
        leftButtonPressed = true;
    } else if (event.mouseButton.button == sf::Mouse::Right) {
        engine.dispatch<onRightMouseDown>(mouseX, mouseY);
        rightButtonPressed = true;
    }
}

void InputEventsDispatcher::onMouseUp(Engine &engine, sf::Event &event) {
    mouseX = event.mouseButton.x;
    mouseY = event.mouseButton.y;

    if (event.mouseButton.button == sf::Mouse::Left) {
        engine.dispatch<onLeftMouseUp>(mouseX, mouseY);
        leftButtonPressed = false;
    } else if (event.mouseButton.button == sf::Mouse::Right) {
        engine.dispatch<onRightMouseUp>(mouseX, mouseY);
        rightButtonPressed = false;
    }
}
