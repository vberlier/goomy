#include "Foo.h"
#include "example.h"
#include <iostream>

void Foo::init(Engine &engine) {
    std::cout << "Configuration for foo: "
              << engine.get<Configuration>().getFoo() << std::endl;
}

void Foo::update(Engine &engine) {
    std::cout << "The number is " << engine.get<Configuration>().getNumber()
              << std::endl;
}
