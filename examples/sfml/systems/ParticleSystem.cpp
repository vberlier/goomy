#include "main.hpp"
#include <algorithm>

ParticleSystem::ParticleSystem()
    : spawnPosition(0, 0), spawnVelocity(0, 0), gravity{0, 9.8}, scale(400),
      bounciness(0.5), rng(std::random_device()()), randomness(0.3) {
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
    sf::Vector2f position(x, y);
    sf::Vector2f velocity(0, 1);

    if (spawnVelocity != sf::Vector2f(0, 0)) {
        position = spawnPosition;
        velocity = spawnVelocity;
    }

    auto entity = engine.entity()
                      .with<Particle>(position, velocity)
                      .with<Age>(std::chrono::seconds(2));

    applyRandomness(entity.get<Particle>().data());
}

void ParticleSystem::onRightMouseDown(int x, int y) {
    spawnPosition = sf::Vector2f(x, y);
}

void ParticleSystem::onRightMouseDrag(int x, int y) {
    spawnVelocity = (spawnPosition - sf::Vector2f(x, y)) * 10.0f / scale;
}

void ParticleSystem::onRightMouseUp(Engine &engine) {
    auto entity = engine.entity()
                      .with<Particle>(spawnPosition, spawnVelocity)
                      .with<Age>(std::chrono::seconds(2));

    applyRandomness(entity.get<Particle>().data());

    spawnVelocity = sf::Vector2f(0, 0);
}

void ParticleSystem::applyRandomness(Particle &particle) {
    auto factor =
        std::max(std::abs(particle.velocity.x), std::abs(particle.velocity.y)) /
        2.0f;

    particle.velocity +=
        sf::Vector2f((nextFloat(rng) - 0.5) * randomness * factor,
                     (nextFloat(rng) - 0.5) * randomness * factor);
}

void ParticleSystem::clampToViewport(Particle &particle,
                                     sf::Vector2f viewport) {
    particle.position.x =
        std::clamp(particle.position.x, 0.0f, (float)viewport.x);
    particle.position.y =
        std::clamp(particle.position.y, 0.0f, (float)viewport.y);
}

const sf::Vector2f &ParticleSystem::getSpawnPosition() const {
    return spawnPosition;
}

const sf::Vector2f &ParticleSystem::getSpawnVelocity() const {
    return spawnVelocity;
}
