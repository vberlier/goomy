#pragma once

#include "goomy/SignalDispatcher.h"
#include "goomy/util.h"

namespace goomy {

template <typename... SystemTypes>
struct Mount {
    using containerType = util::container<SystemTypes...>;

    template <typename EngineType>
    using signalDispatcherType = SignalDispatcher<EngineType, SystemTypes...>;
};

template <typename MountedSystems>
class Engine {
  public:
    Engine() : signalDispatcher(*this), running(false) {
    }

    // Prevent copying
    Engine(const Engine &engine) = delete;
    void operator=(const Engine &engine) = delete;

    template <typename SystemType>
    SystemType &get() {
        return systems.template get<SystemType>();
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
        Engine<MountedSystems>>
        signalDispatcher;
    bool running;
};

}