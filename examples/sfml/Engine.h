#pragma once

#include <goomy.h>

class Overlay;
class ResourceManager;
class Window;

class Dummy;

using Engine =
    goomy::Engine<goomy::Mount<Overlay, ResourceManager, Window>,
                  goomy::Components<Dummy>>;