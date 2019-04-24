#pragma once

#include "Engine.h"
#include <SFML/Graphics.hpp>
#include <string>

class Overlay {
  public:
    Overlay();

    const sf::Color color{100, 250, 50};

    void onUpdate(Engine &engine);
    void onMouseMove(Engine &engine, sf::Event &event);
    void onClick(Engine &engine, sf::Event &event);

  private:
    int mouseX;
    int mouseY;
    int lastClickedX;
    int lastClickedY;

    std::string getString(Engine &engine);
};
