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
        // Create a new entity with a TestComponent
        engine.entity().create<TestComponent>(7);
    }
};

int main() {
    Engine().loop();
    return 0;
}