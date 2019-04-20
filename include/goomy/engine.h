#pragma once

#include "goomy/signal.h"
#include "goomy/util.h"

namespace goomy {

template <typename... SystemTypes>
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

        signalDispatcher.template init<SystemTypes...>();

        while (running) {
            signalDispatcher.template update<SystemTypes...>();
        }
    }

    void shutdown() {
        running = false;
    }

  private:
    util::container<SystemTypes...> systems;
    SignalDispatcher<Engine<SystemTypes...>> signalDispatcher;
    bool running;
};

}