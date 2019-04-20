#pragma once

#include "Engine.h"
#include <string>

class Configuration {
  public:
    void preinit(Engine &engine);

    const std::string &getFoo() const;
    const std::string &getBar() const;
    int getNumber() const;

  private:
    std::string foo;
    std::string bar;
    int number;
};
