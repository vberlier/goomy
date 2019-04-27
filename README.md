<!-- Go to https://github.com/vberlier/goomy to read this file with formatting -->

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

## Running the examples

The project has only been tested on Linux, but doesn't rely on any platform-specific library so you should be good as long as you have a compliant C++17 compiler on hand. One of the example demonstrates how the framework could be used in combination with [SFML](https://www.sfml-dev.org/), so you'll need to make sure that the latest stable version is available on your system.

1. Clone this repository

    ``` sh
    $ git clone https://github.com/vberlier/goomy.git
    $ cd goomy
    ```

2. Build the project

    ``` sh
    $ mkdir cmake-build-release
    $ cd cmake-build-release
    $ cmake -DCMAKE_BUILD_TYPE=Release ..
    $ make
    ```

3. Run the examples

    ``` sh
    $ ./examples/basic/example_basic
    $ ./examples/sfml/example_sfml
    ```

### The SFML example

<p align="center">
  <img width="49%" src="https://raw.githubusercontent.com/vberlier/goomy/master/examples/sfml/screenshots/1.png">
  <img width="49%" src="https://raw.githubusercontent.com/vberlier/goomy/master/examples/sfml/screenshots/2.png">
</p>

This example demonstrates how the framework could be used in combination with [SFML](https://www.sfml-dev.org/). The executable creates a window in which you can spawn particles in a few different ways:

- Clicking and dragging the left mouse button anywhere in the window creates a stream of particles (a particle spawns every frame)
- Dragging the right mouse button and releasing it lets you shoot a single particle in a particular direction
- Pressing the left mouse button while aiming with the right mouse button reactivates the particle stream

> The code for this example is in the [`examples/sfml`](https://github.com/vberlier/goomy/tree/master/examples/sfml) directory.

## Usage

Getting started with `goomy` is fairly straightforward. You pretty much only need to instantiate the `goomy::Engine` template and declare your systems and components.

```cpp
using Engine = goomy::Engine<goomy::Mount<System1, System2, ...>,
                             goomy::Register<Component1, Component2, ...>>;
```

Instantiating the template effectively generates an entity type, creates registries for your different components and wires all the systems and signals together. Everything then happens through your specialized `Engine` type alias.

> You will probably need to forward-declare your system classes because you'll often want to reference your custom `Engine` in their declarations.

Once you declared you custom engine type, the only thing you need to do is to instantiate it and call the `loop()` member function.

```cpp
int main() {
    Engine().loop();
    return 0;
}
```

### Engine API

The engine instance lets you interact with the systems, entities and components you declared when creating your custom `Engine` type. You can use it to get a reference to any of your custom systems, dispatch signals, iterate through entities and components and more. Note that the engine is explicitly marked as non-copiable, and cannot be moved.

#### `engine.get<SystemType>()`

Instantiating a custom `Engine` type creates instances of all the mounted system types. You can get a reference to any of your system instances by calling the templated `get<SystemType>()` member function.

```cpp
auto &system1 = engine.get<System1>();
```

Note that the returned reference is a very thin wrapper that simply marks the system as non-copiable to prevent any accident.

---

License - [MIT](https://github.com/vberlier/goomy/blob/master/LICENSE)
