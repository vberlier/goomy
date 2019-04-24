#include <goomy.h>
#include <iostream>

class TestSystem;

struct TestComponent {
    int number;

    TestComponent(int number) : number(number) {
    }
};

using Engine =
    goomy::Engine<goomy::Mount<TestSystem>, goomy::Register<TestComponent>>;

class TestSystem {
  public:
    void onBeforeUpdate() {
        std::cout << "---" << std::endl;
    }

    void onUpdate(Engine &engine) {
        std::cout << "New entity" << std::endl;
        auto entity = engine.entity();

        if (entity.id() % 2 == 0) {
            std::cout << "Attach test component with 7" << std::endl;
            entity.create<TestComponent>(7);
        }
    }

    void onUpdate(Engine &engine, TestComponent &test) {
        test.number--;

        if (test.number == 0) {
            std::cout << "Shutting down" << std::endl;
            engine.shutdown();
        }
    }
};

int main() {
    Engine().loop();
    return 0;
}