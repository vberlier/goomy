#pragma once

#include "Engine.h"
#include <string>

class Overlay {
  public:
    void onUpdate(Engine &engine);

  private:
    std::string getString(Engine &engine);
};
