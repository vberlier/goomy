# goomy

> A tiny, experimental [ECS framework](https://en.wikipedia.org/wiki/Entity_component_system).

**⚠️ Disclaimer ⚠️**

This project is a proof-of-concept, and is mostly an excuse for me to get familiar with modern C++ and a few experimental features. Don't use this code.

## Introduction

The framework is heavily template-based, and effectively almost disappears at compile-time. Components of the same type are stored contiguously in memory, and entities only hold indices linking to their associated components. Entities are stored contiguously in memory as well, but all these lower-level details are packaged behind a lightweight wrapper api.

The framework also comes with a signaling/event system that makes use of the [detection idiom](https://en.cppreference.com/w/cpp/experimental/is_detected) to facilitate dependency injection.

## What it looks like

The following example is extremely simple and really only scratches the surface of what's possible, but it should give you a basic idea of how the framework works.

```cpp
#include <goomy.h>

class Lifetime;
class TestSystem;

using Engine =
    goomy::Engine<goomy::Mount<TestSystem>, goomy::Register<Lifetime>>;

class Lifetime {
  public:
    int lifetime;
    Lifetime(int lifetime) : lifetime(lifetime) {}
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
```

> You can find this example in the [`examples/basic`](https://github.com/vberlier/goomy/tree/master/examples/basic) directory.

---

License - [MIT](https://github.com/vberlier/goomy/blob/master/LICENSE)
