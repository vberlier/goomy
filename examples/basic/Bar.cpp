#include "Bar.h"
#include "example.h"
#include <iostream>

void Bar::init(Engine &engine) {
    std::cout << "Configuration for bar: "
              << engine.get<Configuration>().getBar() << std::endl;
}

void Bar::update(Engine &engine) {
    std::cout << "Shutting down" << std::endl;
    engine.shutdown();
}
