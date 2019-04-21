#pragma once

#include "Engine.h"
#include <string>

class Overlay {
  public:
    void update(Engine &engine);

  private:
    std::string getString(Engine &engine);
};
