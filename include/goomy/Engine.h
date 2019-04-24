#pragma once

#include "goomy/EngineBase.h"
#include "goomy/Entity.h"
#include "goomy/Signal.h"
#include "goomy/System.h"
#include "goomy/Wrapper.h"

namespace goomy {

template <typename... SystemTypes>
struct Mount {
    template <typename EngineType>
    using systemManagerType = internal::SystemManager<EngineType, SystemTypes...>;
};

template <typename... ComponentTypes>
struct Register {
    using entityManagerType = internal::EntityManager<ComponentTypes...>;

    template <typename systemManagerType>
    using signalDispatcherType =
        internal::SignalDispatcher<systemManagerType, entityManagerType,
                         ComponentTypes...>;
};

template <typename MountedSystems, typename DeclaredComponents>
class Engine : public internal::EngineBase {
  public:
    using engineType = Engine<MountedSystems, DeclaredComponents>;

    using systemManagerType =
        typename MountedSystems::template systemManagerType<engineType>;

    using entityManagerType = typename DeclaredComponents::entityManagerType;

    using signalDispatcherType =
        typename DeclaredComponents::template signalDispatcherType<
            systemManagerType>;

    Engine()
        : systemManager(*this), signalDispatcher(systemManager, entityManager) {
    }

    // Disallow copy
    Engine(const engineType &engine) = delete;
    void operator=(const engineType &engine) = delete;

    template <typename SystemType>
    auto &get() {
        return systemManager.template get<SystemType>();
    }

    template <typename SignalType, typename... Args>
    void dispatch(Args &&... args) {
        signalDispatcher.template dispatch<SignalType>(
            std::forward<Args>(args)...);
    }

    auto entity() {
        return Entity(*this, entityManager.createEntity());
    }

    auto entity(typename entityManagerType::registryIndexType index) {
        return Entity(*this, entityManager.getEntity(index));
    }

    auto &getEntityManager() {
        return entityManager;
    }

    void loop() {
        start();

        signalDispatcher.init();

        while (isRunning()) {
            frameTick();
            signalDispatcher.update();
            entityManager.flush();
        }
    }

  private:
    systemManagerType systemManager;
    entityManagerType entityManager;
    signalDispatcherType signalDispatcher;
};

}