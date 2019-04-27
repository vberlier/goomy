#include <goomy.h>

class Lifetime;
class TestSystem;

using Engine =
    goomy::Engine<goomy::Mount<TestSystem>, goomy::Register<Lifetime>>;

class Lifetime {
  public:
    int lifetime;

    Lifetime(int lifetime) : lifetime(lifetime) {
    }
};

class TestSystem {
  public:
    void onInit(Engine &engine) {
        for (int i = 0; i < 1000; ++i) {
            engine.entity().with<Lifetime>(i);
        }
    }

    void onUpdate(Engine &engine) {
        if (engine.getEntityCount() == 0) {
            engine.shutdown();
        }
    }

    void onUpdate(goomy::Component<Engine, Lifetime> component) {
        if (--component.data().lifetime <= 0) {
            component.entity().destroy();
        }
    }
};

int main() {
    Engine().loop();
    return 0;
}