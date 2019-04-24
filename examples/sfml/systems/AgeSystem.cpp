#include "main.h"

void AgeSystem::onUpdate(Engine &engine, goomy::Component<Engine, Age> age) {
    auto &data = age.data();
    data.age++;

    if (data.maxAge >= 0 && data.age >= data.maxAge) {
        age.entity().destroy();
    }
}
