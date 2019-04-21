#include <goomy.h>
#include <iostream>

class Settings;
class FooSystem;
class BarSystem;

class Transform;
class Collider;
class Renderer;

using Engine = goomy::Engine<goomy::Mount<Settings, FooSystem, BarSystem>,
                             goomy::Components<Transform, Collider, Renderer>>;

enum class Difficulty { easy, hard };

class Settings {
  public:
    Difficulty difficulty = Difficulty::easy;
};

class FooSystem {
  public:
    void init();
    void update(Engine &engine);
};

class BarSystem {
  public:
    void postupdate(Engine &engine);
};

void FooSystem::init() {
    std::cout << "Init foo" << std::endl;
}

void FooSystem::update(Engine &engine) {
    if (engine.get<Settings>().difficulty == Difficulty::easy) {
        std::cout << "Do something easy" << std::endl;
    } else {
        std::cout << "Do something hard" << std::endl;
    }

    auto &entity = engine.create_entity();

    std::cout << entity.has<Transform>() << std::endl;
    std::cout << engine.get_entity(0).get<Transform>() << std::endl;
}

void BarSystem::postupdate(Engine &engine) {
    std::cout << "Shutting down" << std::endl;
    engine.shutdown();
}

int main() {
    Engine().loop();
    return 0;
}