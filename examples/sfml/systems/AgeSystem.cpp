#include "main.h"

Age::Age() : maxAge(std::nullopt) {
}

Age::Age(goomy::clock::duration maxAge) : maxAge(std::optional(maxAge)) {
}

void AgeSystem::onUpdate(Engine &engine, goomy::Component<Engine, Age> age) {
    auto &data = age.data();
    data.age += engine.getFrameDuration();

    if (data.maxAge && data.age > data.maxAge.value()) {
        age.entity().destroy();
    }
}