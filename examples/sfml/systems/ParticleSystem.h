#pragma once

#include "Engine.h"

struct Particle {
    int x;
    int y;

    Particle(int x, int y) : x(x), y(y) {
    }
};

class ParticleSystem {
  public:
    void onUpdate(Engine &engine, Particle &particle);
    void onClick(Engine &engine, sf::Event &event);
};
