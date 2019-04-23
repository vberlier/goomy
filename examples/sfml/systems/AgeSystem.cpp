#include "main.h"

void AgeSystem::onUpdate(Engine &engine, Age &age) {
    age.age++;

    if (age.maxAge >= 0 && age.age >= age.maxAge) {
        auto &entityManager = engine.getEntityManager();
        entityManager.destroyEntity(entityManager.getEntity(age));
    }
}
