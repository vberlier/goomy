#pragma once

#include <goomy.h>

class AgeSystem;
class Display;
class InputEventsDispatcher;
class ParticleSystem;
class ResourceManager;
class Window;

class Age;
class Particle;

GOOMY_SIGNAL(onMouseMove);
GOOMY_SIGNAL(onMouseDown);
GOOMY_SIGNAL(onMouseUp);

GOOMY_SIGNAL(onLeftMouseDown);
GOOMY_SIGNAL(onLeftMouseDrag);
GOOMY_SIGNAL(onLeftMouseUp);
GOOMY_SIGNAL(onRightMouseDown);
GOOMY_SIGNAL(onRightMouseDrag);
GOOMY_SIGNAL(onRightMouseUp);

using Engine =
    goomy::Engine<goomy::Mount<AgeSystem, Display, InputEventsDispatcher,
                               ParticleSystem, ResourceManager, Window>,
                  goomy::Register<Age, Particle>>;
