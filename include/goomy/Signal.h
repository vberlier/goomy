#pragma once

#include <experimental/type_traits>
#include <type_traits>

#define GOOMY_SIGNAL(NAME)                                                     \
    struct NAME {                                                              \
        template <typename T>                                                  \
        using callback = decltype(&T::NAME);                                   \
                                                                               \
        template <typename T, typename... Args>                                \
        static void invoke(T &instance, Args &&... args) {                     \
            instance.NAME(std::forward<Args>(args)...);                        \
        }                                                                      \
    }

namespace goomy {

namespace signals {

GOOMY_SIGNAL(onBeforeInit);
GOOMY_SIGNAL(onInit);
GOOMY_SIGNAL(onAfterInit);

GOOMY_SIGNAL(onBeforeUpdate);
GOOMY_SIGNAL(onUpdate);
GOOMY_SIGNAL(onAfterUpdate);

} // namespace signals

template <typename SignalType>
struct Signal {
    template <typename T>
    using callback = typename SignalType::template callback<T>;

    template <typename T, typename... Args>
    using detector = decltype((std::declval<T>().*std::declval<callback<T>>())(
        std::declval<Args>()...));

    template <typename... Args>
    using detected = std::experimental::is_detected<detector, Args...>;

    template <typename T, typename... Args>
    static typename std::enable_if_t<!detected<T, Args...>{}>
    invoke(T &instance, Args &&... args) {
    }

    template <typename T, typename... Args>
    static typename std::enable_if_t<detected<T, Args...>{}>
    invoke(T &instance, Args &&... args) {
        SignalType::template invoke<T>(instance, std::forward<Args>(args)...);
    }
};

template <typename EngineType, typename... SystemTypes>
class SignalDispatcher {
public:
    explicit SignalDispatcher(EngineType &engine) : engine(engine) {
    }

    template <typename SignalType>
    void dispatch() {
        (Signal<SignalType>::invoke(engine.template get<SystemTypes>()),...);
        (Signal<SignalType>::invoke(engine.template get<SystemTypes>(), engine),...);
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
    EngineType &engine;
};

}