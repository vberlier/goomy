#pragma once

#include <goomy.h>

class Overlay;
class ResourceManager;
class Window;

struct Dummy {
    int x;
    int y;

    Dummy(int x, int y) : x(x), y(y) {}
};

using Engine =
    goomy::Engine<goomy::Mount<Overlay, ResourceManager, Window>,
                  goomy::Components<Dummy>>;

GOOMY_SIGNAL(onMouseMove);
GOOMY_SIGNAL(onClick);