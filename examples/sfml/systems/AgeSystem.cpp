#include "main.h"

void AgeSystem::onUpdate(Engine &engine, goomy::Component<Engine, Age> age) {
    auto &data = age.data();
    data.age += engine.getFrameDuration();

    if (data.maxAge && data.age > data.maxAge.value()) {
        age.entity().destroy();
    }
}
