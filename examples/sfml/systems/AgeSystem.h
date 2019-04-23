#pragma once

#include "Engine.h"

struct Age {
    int age = 0;
    int maxAge;

    Age() : Age(-1) {
    }

    explicit Age(int maxAge) : maxAge(maxAge) {
    }
};

class AgeSystem {
  public:
    void onUpdate(Engine &engine, Age &age);
};
