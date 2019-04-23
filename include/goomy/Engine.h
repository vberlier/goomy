#pragma once

#include "goomy/EngineBase.h"
#include "goomy/Entity.h"
#include "goomy/Registry.h"
#include "goomy/Signal.h"
#include "goomy/System.h"

namespace goomy {

template <typename... SystemTypes>
struct Mount {
    template <typename EngineType>
    using systemManagerType = SystemManager<EngineType, SystemTypes...>;
};

template <typename... ComponentTypes>
struct Components {
    using entityType = RegistryItem<Entity<ComponentTypes...>>;
    using entityRegistryType = Registry<entityType>;
};

template <typename MountedSystems, typename DeclaredComponents>
class Engine : public EngineBase {
  public:
    using engineType = Engine<MountedSystems, DeclaredComponents>;

    using systemManagerType =
        typename MountedSystems::template systemManagerType<engineType>;

    using entityType = typename DeclaredComponents::entityType;
    using entityRegistryType = typename DeclaredComponents::entityRegistryType;

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

    entityType &getEntity(int index) {
        return entityRegistry.get(index);
    }

    entityType &createEntity() {
        return entityRegistry.create();
    }

    void destroyEntity(entityType &entity) {
        entityRegistry.destroy(entity);
    }

    void loop() {
        start();

        systemManager.init();

        while (isRunning()) {
            frameTick();
            systemManager.update();
            entityRegistry.flush();
        }
    }

    std::size_t entityCount() {
        return entityRegistry.size();
    }

  private:
    systemManagerType systemManager;
    entityRegistryType entityRegistry;
};

}