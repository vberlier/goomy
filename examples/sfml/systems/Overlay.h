#pragma once

#include "Engine.h"
#include <SFML/Graphics.hpp>
#include <string>

class Overlay {
  public:
    Overlay();

    void onUpdate(Engine &engine);
    void onClick(Engine &engine, sf::Event &event);

  private:
    int lastClickedX;
    int lastClickedY;

    std::string getString(Engine &engine);
};
