#pragma once

#include "Engine.h"
#include <SFML/Graphics.hpp>
#include <string>

class Display {
  public:
    void onUpdate(Engine &engine);
    void onUpdate(Engine &engine, Particle &particle);

  private:
    const sf::Color color{100, 250, 50};

    std::string stats(Engine &engine);
};
