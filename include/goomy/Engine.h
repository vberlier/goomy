#pragma once

#include "goomy/EngineBase.h"
#include "goomy/Entity.h"
#include "goomy/Registry.h"
#include "goomy/Signal.h"
#include "goomy/System.h"

namespace goomy {

template <typename... SystemTypes>
struct Mount {
    using containerType = SystemContainer<SystemTypes...>;

    template <typename EngineType>
    using signalDispatcherType = SignalDispatcher<EngineType, SystemTypes...>;
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

    using systemContainerType = typename MountedSystems::containerType;
    using signalDispatcherType =
        typename MountedSystems::template signalDispatcherType<engineType>;

    using entityType = typename DeclaredComponents::entityType;
    using entityRegistryType = typename DeclaredComponents::entityRegistryType;

    Engine() : signalDispatcher(*this) {
    }

    // Disallow copy
    Engine(const Engine &engine) = delete;
    void operator=(const Engine &engine) = delete;

    template <typename SystemType>
    System<SystemType> &get() {
        return systems.template get<SystemType>();
    }

    template <typename SignalType, typename... Args>
    void dispatch(Args &&... args) {
        signalDispatcher.template dispatch<SignalType>(
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

        signalDispatcher.init();

        while (isRunning()) {
            frameTick();
            signalDispatcher.update();
            entityRegistry.flush();
        }
    }

    std::size_t entityCount() {
        return entityRegistry.size();
    }

  private:
    systemContainerType systems;
    signalDispatcherType signalDispatcher;
    entityRegistryType entityRegistry;
};

}