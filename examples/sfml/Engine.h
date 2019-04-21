#pragma once

#include <goomy.h>

class Window;
class ExampleSystem;

using Engine = goomy::Engine<goomy::Mount<Window, ExampleSystem>>;