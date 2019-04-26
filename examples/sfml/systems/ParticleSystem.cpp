#include "main.h"
#include <algorithm>

ParticleSystem::ParticleSystem()
    : gravity{0, 9.8}, scale(400), bounciness(0.5), rng(std::random_device()()),
      randomness(0.5) {
}

Particle::Particle(sf::Vector2f position, sf::Vector2f velocity)
    : position(position), velocity(velocity) {
}

void ParticleSystem::onUpdate(Engine &engine, Particle &particle) {
    auto viewport = static_cast<sf::Vector2f>(engine.get<Window>().getSize());

    if ((particle.position.x <= 0 && particle.velocity.x < 0) ||
        (particle.position.x >= viewport.x - 1 && particle.velocity.x > 0)) {
        particle.velocity.x *= -bounciness;
        particle.velocity.y *= bounciness;

        applyRandomness(particle);
        clampToViewport(particle, viewport);
    }

    if ((particle.position.y <= 0 && particle.velocity.y < 0) ||
        (particle.position.y >= viewport.y - 1 && particle.velocity.y > 0)) {
        particle.velocity.x *= bounciness;
        particle.velocity.y *= -bounciness;

        applyRandomness(particle);
        clampToViewport(particle, viewport);
    }

    particle.velocity += gravity * engine.deltaTime();
    particle.position += particle.velocity * engine.deltaTime() * scale;
}

void ParticleSystem::onLeftMouseDrag(Engine &engine, int x, int y) {
    auto entity = engine.entity()
                      .with<Particle>(sf::Vector2f(x, y), sf::Vector2f(0, 0))
                      .with<Age>(std::chrono::seconds(2));

    applyRandomness(entity.get<Particle>().data());
}

void ParticleSystem::applyRandomness(Particle &particle) {
    particle.velocity += sf::Vector2f((nextFloat(rng) - 0.5) * randomness,
                                      (nextFloat(rng) - 0.5) * randomness);
}

void ParticleSystem::clampToViewport(Particle &particle,
                                     sf::Vector2f viewport) {
    particle.position.x =
        std::clamp(particle.position.x, 0.0f, (float)viewport.x);
    particle.position.y =
        std::clamp(particle.position.y, 0.0f, (float)viewport.y);
}