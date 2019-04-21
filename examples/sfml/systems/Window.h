#pragma once

#include "Engine.h"
#include <SFML/Graphics.hpp>

class Window : public sf::RenderWindow {
  public:
    Window();
    void onBeforeUpdate();
    void onUpdate(Engine &engine);
    void onAfterUpdate(Engine &engine);
};