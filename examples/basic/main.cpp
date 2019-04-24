#include <goomy.h>
#include <iostream>

class Settings;
class FooSystem;
class BarSystem;

struct Testing {
    int frame;

    explicit Testing(int frame) : frame(frame) {
    }
};

class Transform {};
class Collider {};
class Renderer {};

using Engine =
    goomy::Engine<goomy::Mount<Settings, FooSystem, BarSystem>,
                  goomy::Register<Testing, Transform, Collider, Renderer>>;

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
    int frame = 0;

    void onBeforeUpdate(Engine &engine);
};

void FooSystem::onInit() {
    std::cout << "Init foo" << std::endl;
}

void FooSystem::onUpdate(Engine &engine) {
    auto &entityManager = engine.getEntityManager();

    auto &settings = engine.get<Settings>();

    if (settings.difficulty == Difficulty::easy) {
        std::cout << "Do something easy" << std::endl;
        settings.difficulty = Difficulty::hard;
    } else {
        std::cout << "Do something hard" << std::endl;
        settings.difficulty = Difficulty::easy;
    }

    auto &foo = entityManager.createEntity();
    std::cout << "Created foo: " << foo.getIndex() << std::endl;

    entityManager.createComponent<Testing>(foo, engine.get<BarSystem>().frame);
    std::cout << "Foo transform: "
              << entityManager.getEntity(foo.getIndex()).get<Testing>()
              << std::endl;

    std::cout << "All entities:" << std::endl;

    for (int i = 0; i < entityManager.getEntityCount(); ++i) {
        std::cout << entityManager.getEntity(i).getIndex() << " has component "
                  << entityManager.getEntity(i).get<Testing>() << " ("
                  << entityManager
                         .getComponent<Testing>(entityManager.getEntity(i))
                         .frame
                  << ")" << std::endl;
    }

    std::cout
        << "Foo index: "
        << entityManager.getEntity(entityManager.getEntityCount()).getIndex()
        << std::endl;

    std::cout << "Use wrapper API" << std::endl;

    auto entity = engine.entity();
    std::cout << "New entity: " << entity.id() << std::endl;
    std::cout << "Lookup entity: " << engine.entity(entity.id()).id() << std::endl;
    std::cout << "Has test component? " << entity.has<Testing>() << std::endl;

    auto testing = entity.create<Testing>(95);

    std::cout << "What about now? " << entity.has<Testing>() << std::endl;
    std::cout << "Testing id: " << testing.id() << std::endl;
    std::cout << "Testing data: " << testing.data().frame << std::endl;
    std::cout << "Entity through component: " << testing.entity().id() << std::endl;
    std::cout << "Data through entity: " << entity.get<Testing>().data().frame << std::endl;

    testing.destroy();

    std::cout << "Entity no longer has component: " << entity.has<Testing>() << std::endl;

    entity.destroy();

    std::cout << "Destroyed " << std::endl;
}

void BarSystem::onBeforeUpdate(Engine &engine) {
    auto &entityManager = engine.getEntityManager();

    std::cout << "-> Frame " << frame << std::endl;
    std::cout << "Entity count " << entityManager.getEntityCount() << std::endl;

    if (frame == 4) {
        entityManager.destroyEntity(entityManager.getEntity(2));
    }

    if (frame >= 7) {
        std::cout << "Shutting down" << std::endl;
        engine.shutdown();
    }

    frame++;
}

int main() {
    Engine().loop();
    return 0;
}