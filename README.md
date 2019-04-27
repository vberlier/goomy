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
#include <goomy.hpp>

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

## API reference

### Engine

The engine instance lets you interact with the systems, entities and components you declared when creating your custom `Engine` type. You can use it to get a reference to any of your custom systems, dispatch signals, iterate through entities and components and more. Note that the engine is explicitly marked as non-copiable, and cannot be moved.

#### `engine.get<SystemType>()`

Instantiating a custom `Engine` type creates instances of all the mounted system types. You can get a reference to any of your system instances by calling the templated `get<SystemType>()` member function.

```cpp
auto &system1 = engine.get<System1>();
```

Note that the returned reference is a very thin wrapper that simply marks the system as non-copiable to prevent any accident.

#### `engine.dispatch<SignalType>(Args &&... args)`

This function lets you dispatch signals throughout your systems. The template parameter of the `dispatch<SignalType>()` member function must be a signal class/struct, usually generated with the `GOOMY_SIGNAL(NAME)` macro.

```cpp
GOOMY_SIGNAL(onCustomEvent);

engine.dispatch<onCustomEvent>(arg1, arg2, ...);
```

This code will essentially go through every system instance at compile-time and generate a call to the `onCustomEvent()` member function if it is defined.

#### `engine.entity()`

You can create entities with the `entity()` member function. This function returns a lightweight wrapper around a reference to the newly created internal entity.

```cpp
auto entity = engine.entity();
```

For more details check out the entity API reference.

#### `engine.entity(std::size_t id)`

This function lets you retrieve an entity by its id.

```cpp
auto entity = engine.entity(42);
```

#### `engine.entities()`

You can iterate over every entity by using the `entities()` member function.

```cpp
for (auto entity : engine.entities()) {
    // ...
}
```

#### `engine.getEntityCount()`

Returns the current number of entities.

```cpp
auto count = engine.getEntityCount();
```

#### `engine.components<ComponentType>()`

This lets your iterate over all the components of a specific type.

```cpp
for (auto component1 : engine.components<Component1>()) {
    // ...
}
```

The iterator doesn't yield references to the internal components directly, but lightweight wrappers that provide a few useful member functions to let you access the id of the component or its associated entity for instance.

For more details check out the component API reference.

#### `engine.loop()`

This function is usually called in your program's `main()`. It runs the main execution loop and will block until the engine gets shutdown.

```cpp
engine.loop();
```

#### `engine.shutdown()`

You can stop the main execution loop by calling the `shutdown()` member function. The engine will cleanly finish executing the current frame and exit.

```cpp
engine.shutdown();
```

#### `engine.deltaTime()`

This returns the number of seconds that it took for executing the frame.

```cpp
position += velocity * engine.deltaTime();
```

### Entity

The framework lets you interact with the internal entities through lightweight reference wrappers that provide a nice and clean API.

Entities are internally all stored contiguously in memory, in a standard `std::vector`. The engine guarantees that the vector holding the entities internally will not be mutated during the frame by buffering entity creation and deletion in separate containers, which in turn means that entity reference wrappers are valid for the entire frame.

However, the engine flushes the pending modifications at the end of each frame, which potentially mutates the internal vector and invalidates references and reference wrappers. Unless you know what you're doing, the lifetime of any entity reference wrapper should not exceed the frame it was created in.

#### `entity.id()`

Returns the id of the entity. The id can be used to retrieve the entity from the engine instance with the `engine.entity(id)` member function.

```cpp
auto id = entity.id();
```

Note that this id can change over time when other entities are destroyed.

#### `entity.engine()`

Returns a reference to the engine instance.

```cpp
auto &engine = entity.engine();
```

#### `entity.has<ComponentType>()`

Whether the entity has a component of the given type.

```cpp
bool hasComponent1 = entity.has<Component1>();
```

#### `entity.get<ComponentType>()`

This function returns a lightweight wrapper around a reference to the component of the given type associated with the entity.

```cpp
auto component1 = entity.get<Component1>();
```

This function should only be called if the component actually exists so make sure to check for the component with the `has<ComponentType>()` member function where appropriate.

#### `entity.create<ComponentType>(Args &&... args)`

Create and attach a new component of the given type to the entity. The arguments are passed to the constructor of the component. The function returns a component wrapper.

```cpp
auto component1 = entity.create<Component1>(arg1, arg2, ...);
```

#### `entity.with<ComponentType>(Args &&... args)`

This function is very similar to the `entity.create<ComponentType>()` member function, but returns the entity itself instead of a reference wrapper to the newly created component.

```cpp
entity
    .with<Component1>(arg1, arg2, ...)
    .with<Component2>(arg1, arg2, ...)
    .with<Component3>(arg1, arg2, ...);
```

The fluent API makes it easy to create multiple components at the same time.

#### `entity.destroy()`

Destroys the entity. This will also destroy all the components associated with the entity.

```cpp
entity.destroy();
```

### Component

The framework lets you interact with the internal component instances through lightweight reference wrappers that provide a nice and clean API.

Instances of each type of component are internally stored contiguously in memory in separate standard `std::vector`, very similarly to how entities are stored internally as well. The engine guarantees that the vectors holding the components internally will not be mutated during the frame by buffering component creation and deletion in separate containers, which in turn means that component reference wrappers are valid for the entire frame.

However, the engine flushes the pending modifications at the end of each frame, which potentially mutates the internal vectors and invalidates references and reference wrappers. Just like with entity reference wrappers, unless you know what you're doing, the lifetime of any component reference wrapper should not exceed the frame it was created in.

#### `component.id()`

Returns the id of the component. Note that this id is only unique for components of the same type, and can change over time when other components of the same type are destroyed.

```cpp
auto id = component.id();
```

#### `component.engine()`

Returns a reference to the engine instance.

```cpp
auto &engine = component.engine();
```

#### `component.entity()`

Returns a reference wrapper to the associated entity.

```cpp
auto entity = component.entity();
```

#### `component.data()`

This function returns a reference to the declared component instance.

```cpp
auto &data = component.data();
```

#### `component.destroy()`

Destroys the component.

```cpp
component.destroy();
```

---

License - [MIT](https://github.com/vberlier/goomy/blob/master/LICENSE)
