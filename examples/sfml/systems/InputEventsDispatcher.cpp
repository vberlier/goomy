#include "main.h"

InputEventsDispatcher::InputEventsDispatcher()
    : mouseX(0), mouseY(0), leftButtonPressed(false),
      rightButtonPressed(false) {
}

void InputEventsDispatcher::onUpdate(Engine &engine) {
    if (leftButtonPressed) {
        engine.dispatch<onLeftMouseDrag>(mouseX, mouseY);
    }

    if (rightButtonPressed) {
        engine.dispatch<onRightMouseDrag>(mouseX, mouseY);
    }
}

void InputEventsDispatcher::onMouseMove(sf::Event &event) {
    mouseX = event.mouseMove.x;
    mouseY = event.mouseMove.y;
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