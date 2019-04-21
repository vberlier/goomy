#pragma once

#include "goomy/EntityRegistry.h"
#include "goomy/SignalDispatcher.h"

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
        EntityRegistry<EngineType, entityType, ComponentTypes...>;
};

template <typename MountedSystems, typename DeclaredComponents>
class Engine {
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

    // Prevent copying
    Engine(const Engine &engine) = delete;
    void operator=(const Engine &engine) = delete;

    template <typename SystemType>
    SystemType &get() {
        return systems.template get<SystemType>();
    }

    entityType &get_entity(int index) {
        return entityRegistry[index];
    }

    entityType &create_entity() {
        return entityRegistry.create();
    }

    void loop() {
        running = true;

        signalDispatcher.init();

        while (running) {
            signalDispatcher.update();
        }
    }

    void shutdown() {
        running = false;
    }

  private:
    systemContainerType systems;
    signalDispatcherType signalDispatcher;

    entityRegistryType entityRegistry;

    bool running = false;
};

}