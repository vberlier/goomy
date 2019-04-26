#pragma once

#include "Engine.h"
#include <SFML/Graphics.hpp>
#include <random>

struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;

    Particle(sf::Vector2f position, sf::Vector2f velocity);
};

class ParticleSystem {
  public:
    ParticleSystem();

    void onUpdate(Engine &engine, Particle &particle);
    void onLeftMouseDrag(Engine &engine, int x, int y);

  private:
    sf::Vector2f gravity;
    float scale;
    float bounciness;

    std::mt19937 rng;
    std::uniform_real_distribution<float> nextFloat;
    float randomness;

    void applyRandomness(Particle &particle);
    void clampToViewport(Particle &particle, sf::Vector2f viewport);
};
