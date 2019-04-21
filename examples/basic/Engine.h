#pragma once

#include <goomy.h>

class Configuration;
class FooSystem;
class BarSystem;

using Engine = goomy::Engine<goomy::Mount<Configuration, FooSystem, BarSystem>>;