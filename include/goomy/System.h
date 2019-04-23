#pragma once

#include "goomy/Signal.h"
#include <experimental/type_traits>
#include <type_traits>

namespace goomy {

template <typename SystemType>
class System : public SystemType {
  public:
    System() = default;

    // Disallow copy
    System(const System &system) = delete;
    void operator=(const System &system) = delete;
};

template <typename T, typename... Ts>
struct SystemContainer : SystemContainer<T>, SystemContainer<Ts...> {
    using SystemContainer<T>::get;
    using SystemContainer<Ts...>::get;
};

template <typename T>
struct SystemContainer<T> {
    template <typename U, typename = std::enable_if_t<std::is_same<T, U>{}>>
    auto &get() {
        return instance;
    }

  private:
    System<T> instance;
};

template <typename EngineType, typename... SystemTypes>
class SystemManager {
  public:
    explicit SystemManager(EngineType &engine) : engine(engine) {
    }

    template <typename SystemType>
    auto &get() {
        return container.template get<SystemType>();
    }

    template <typename SignalType, typename... Args>
    void dispatch(Args &&... args) {
        (invoke<SignalType>(engine.template get<SystemTypes>(),
                            std::forward<Args>(args)...),
         ...);
        (invoke<SignalType>(engine.template get<SystemTypes>(), engine,
                            std::forward<Args>(args)...),
         ...);
    }

    void init() {
        dispatch<signals::onBeforeInit>();
        dispatch<signals::onInit>();
        dispatch<signals::onAfterInit>();
    }

    void update() {
        dispatch<signals::onBeforeUpdate>();
        dispatch<signals::onUpdate>();
        dispatch<signals::onAfterUpdate>();
    }

  private:
    EngineType &engine = engine;
    SystemContainer<SystemTypes...> container;

    template <typename SignalType, typename... Args>
    void invoke(Args &&... args) {
        Signal<SignalType>::invoke(std::forward<Args>(args)...);
    }
};

}