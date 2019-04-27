#pragma once

#include "Engine.hpp"
#include <SFML/Graphics.hpp>

class InputEventsDispatcher {
  public:
    InputEventsDispatcher();

    void onUpdate(Engine &engine);
    void onMouseMove(sf::Event &event);
    void onMouseDown(Engine &engine, sf::Event &event);
    void onMouseUp(Engine &engine, sf::Event &event);

  private:
    int mouseX;
    int mouseY;
    bool leftButtonPressed;
    bool rightButtonPressed;
};
