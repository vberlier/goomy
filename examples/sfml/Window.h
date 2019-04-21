#pragma once

#include "Engine.h"
#include <SFML/Graphics.hpp>

class Window : public sf::RenderWindow {
  public:
    Window();
    void preupdate();
    void update(Engine &engine);
    void postupdate();
};