#include "main.h"

Particle::Particle(int x, int y) : x(x), y(y) {
}

void ParticleSystem::onLeftMouseDown(Engine &engine, int x, int y) {
    engine.entity().with<Particle>(x, y).with<Age>(std::chrono::seconds(3));
}
