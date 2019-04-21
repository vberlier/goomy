#pragma once

#include "goomy/EntityRegistry.h"
#include "goomy/SignalDispatcher.h"

namespace goomy {

template <typename... SystemTypes>
struct Mount {
    using containerType = system_instances<SystemTypes...>;

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
    using Entity = typename DeclaredComponents::entityType;

    Engine() : signalDispatcher(*this), entityRegistry(*this) {
    }

    // Prevent copying
    Engine(const Engine &engine) = delete;
    void operator=(const Engine &engine) = delete;

    template <typename SystemType>
    SystemType &get() {
        return systems.template get<SystemType>();
    }

    Entity &get_entity(int index) {
        return entityRegistry[index];
    }

    Entity &create_entity() {
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
    typename MountedSystems::containerType systems;

    typename MountedSystems::template signalDispatcherType<
        Engine<MountedSystems, DeclaredComponents>>
        signalDispatcher;

    typename DeclaredComponents::template entityRegistryType<
        Engine<MountedSystems, DeclaredComponents>>
        entityRegistry;

    bool running = false;
};

}