#include "Configuration.h"
#include "example.h"

void Configuration::preinit() {
    foo = "hello";
    bar = "world";
    number = 42;
}

const std::string &Configuration::getFoo() const {
    return foo;
}

const std::string &Configuration::getBar() const {
    return bar;
}

int Configuration::getNumber() const {
    return number;
}
