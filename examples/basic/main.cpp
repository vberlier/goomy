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
    void onBeforeUpdate(Engine &engine);

  private:
    int frame = 0;
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

    auto &foo = engine.createEntity();
    std::cout << "Created foo: " << foo.index << std::endl;

    foo.set<Transform>(engine.entityCount() + 42);
    std::cout << "Foo transform: "
              << engine.getEntity(foo.index).get<Transform>() << std::endl;

    std::cout << "All entities:" << std::endl;

    for (int i = 0; i < engine.entityCount(); ++i) {
        std::cout << engine.getEntity(i).index << " has transform "
                  << engine.getEntity(i).get<Transform>() << std::endl;
    }

    std::cout << "Foo index: " << engine.getEntity(engine.entityCount()).index
              << std::endl;
}

void BarSystem::onBeforeUpdate(Engine &engine) {
    std::cout << "-> Frame " << frame << std::endl;
    std::cout << "Entity count " << engine.entityCount() << std::endl;

    if (frame >= 5) {
        std::cout << "Shutting down" << std::endl;
        engine.shutdown();
    }

    frame++;
}

int main() {
    Engine().loop();
    return 0;
}