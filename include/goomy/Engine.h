#pragma once

#include "goomy/EngineBase.h"
#include "goomy/Entity.h"
#include "goomy/System.h"

namespace goomy {

template <typename... SystemTypes>
struct Mount {
    template <typename EngineType>
    using systemManagerType = SystemManager<EngineType, SystemTypes...>;
};

template <typename... ComponentTypes>
struct Components {
    using entityManagerType = EntityManager<ComponentTypes...>;
};

template <typename MountedSystems, typename DeclaredComponents>
class Engine : public EngineBase {
  public:
    using engineType = Engine<MountedSystems, DeclaredComponents>;

    using systemManagerType =
        typename MountedSystems::template systemManagerType<engineType>;

    using entityManagerType = typename DeclaredComponents::entityManagerType;

    Engine() : systemManager(*this) {
    }

    // Disallow copy
    Engine(const Engine &engine) = delete;
    void operator=(const Engine &engine) = delete;

    template <typename SystemType>
    auto &get() {
        return systemManager.template get<SystemType>();
    }

    template <typename SignalType, typename... Args>
    void dispatch(Args &&... args) {
        systemManager.template dispatch<SignalType>(
            std::forward<Args>(args)...);
    }

    auto &getEntity(typename entityManagerType::registryIndexType index) {
        return entityManager.getEntity(index);
    }

    auto &createEntity() {
        return entityManager.createEntity();
    }

    void destroyEntity(typename entityManagerType::entityType &entity) {
        entityManager.destroyEntity(entity);
    }

    void loop() {
        start();

        systemManager.init();

        while (isRunning()) {
            frameTick();
            systemManager.update();
            entityManager.update();
        }
    }

    std::size_t entityCount() {
        return entityManager.entityCount();
    }

  private:
    systemManagerType systemManager;
    entityManagerType entityManager;
};

}