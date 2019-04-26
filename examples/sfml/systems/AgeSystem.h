#pragma once

#include "Engine.h"

struct Age {
    goomy::clock::duration age = goomy::clock::duration(0);
    std::optional<goomy::clock::duration> maxAge;

    Age();
    explicit Age(goomy::clock::duration maxAge);
};

class AgeSystem {
  public:
    void onUpdate(Engine &engine, goomy::Component<Engine, Age> age);
};
