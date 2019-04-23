#pragma once

#include <goomy.h>

class AgeSystem;
class Overlay;
class ResourceManager;
class Window;

struct Dummy {
    int x;
    int y;

    Dummy(int x, int y) : x(x), y(y) {}
};

class Age;

using Engine =
    goomy::Engine<goomy::Mount<AgeSystem,Overlay, ResourceManager, Window>,
                  goomy::Components<Dummy, Age>>;

GOOMY_SIGNAL(onMouseMove);
GOOMY_SIGNAL(onClick);