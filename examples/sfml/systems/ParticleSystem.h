#pragma once

#include "Engine.h"

struct Particle {
    int x;
    int y;

    Particle(int x, int y);
};

class ParticleSystem {
  public:
    void onLeftMouseDown(Engine &engine, int x, int y);
};
