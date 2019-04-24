#pragma once

#include <goomy.h>

class AgeSystem;
struct Age;

class Overlay;

class ParticleSystem;
struct Particle;

class ResourceManager;

class Window;
GOOMY_SIGNAL(onMouseMove);
GOOMY_SIGNAL(onClick);

using Engine = goomy::Engine<
    goomy::Mount<AgeSystem, Overlay, ParticleSystem, ResourceManager, Window>,
    goomy::Register<Age, Particle>>;
