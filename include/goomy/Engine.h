#pragma once

#include "goomy/EngineBase.h"
#include "goomy/Entity.h"
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
    using entityType = Entity<ComponentTypes...>;

    template <typename EngineType>
    using entityRegistryType =
        EntityRegistry<EngineType, entityType>;
};

template <typename MountedSystems, typename DeclaredComponents>
class Engine : public EngineBase {
  public:
    using engineType = Engine<MountedSystems, DeclaredComponents>;

    using systemContainerType = typename MountedSystems::containerType;
    using signalDispatcherType =
        typename MountedSystems::template signalDispatcherType<engineType>;

    using entityType = typename DeclaredComponents::entityType;
    using entityRegistryType =
        typename DeclaredComponents::template entityRegistryType<engineType>;

    Engine() : signalDispatcher(*this), entityRegistry(*this) {
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
        return entityRegistry[index];
    }

    entityType &createEntity() {
        return entityRegistry.create();
    }

    void loop() {
        start();

        signalDispatcher.init();

        while (isRunning()) {
            frameTick();
            signalDispatcher.update();
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