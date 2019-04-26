#pragma once

#include "Engine.h"
#include <SFML/Graphics.hpp>

class InputEventsDispatcher {
  public:
    void onMouseMove(Engine &engine, sf::Event &event);
    void onMouseDown(Engine &engine, sf::Event &event);
    void onMouseUp(Engine &engine, sf::Event &event);

  private:
    int mouseX;
    int mouseY;
    bool leftButtonPressed;
    bool rightButtonPressed;
};
