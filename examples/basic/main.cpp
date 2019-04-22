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
    void onInit();
    void onUpdate(Engine &engine);
};

class BarSystem {
  public:
    void onAfterUpdate(Engine &engine);
};

void FooSystem::onInit() {
    std::cout << "Init foo" << std::endl;
}

void FooSystem::onUpdate(Engine &engine) {
    auto &settings = engine.get<Settings>();

    if (settings.difficulty == Difficulty::easy) {
        std::cout << "Do something easy" << std::endl;
    } else {
        std::cout << "Do something hard" << std::endl;
    }

    auto &entity = engine.createEntity();

    std::cout << entity.has<Transform>() << std::endl;
    std::cout << engine.getEntity(entity.id).get<Transform>() << std::endl;
    std::cout << engine.createEntity().id << std::endl;
}

void BarSystem::onAfterUpdate(Engine &engine) {
    std::cout << "Shutting down" << std::endl;
    engine.shutdown();
}

int main() {
    Engine().loop();
    return 0;
}