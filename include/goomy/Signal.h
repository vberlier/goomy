#pragma once

#include <experimental/type_traits>
#include <type_traits>

#define GOOMY_SIGNAL(NAME)                                                     \
    struct NAME {                                                              \
        template <typename T, typename... Args>                                \
        using detector =                                                       \
            decltype(std::declval<T>().NAME(std::declval<Args>()...));         \
                                                                               \
        template <typename T, typename... Args>                                \
        static void invoke(T &instance, Args &&... args) {                     \
            instance.NAME(std::forward<Args>(args)...);                        \
        }                                                                      \
    }

namespace goomy::internal {

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
    template <typename T, typename... Args>
    using detector = typename SignalType::template detector<T, Args...>;

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

template <typename EngineType, typename SystemManagerType,
          typename... ComponentTypes>
class SignalDispatcher {
  public:
    SignalDispatcher(EngineType &engine, SystemManagerType &systemManager)
        : engine(engine), systemManager(systemManager) {
    }

    template <typename SignalType, typename... Args>
    void dispatch(Args &&... args) {
        systemManager.template dispatch<SignalType>(
            std::forward<Args>(args)...);

        (dispatchWithComponent<SignalType, ComponentTypes>(
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
    EngineType &engine;
    SystemManagerType &systemManager;

    template <typename SignalType, typename ComponentType, typename... Args>
    void dispatchWithComponent(Args &&... args) {
        for (auto component : engine.template components<ComponentType>()) {
            systemManager.template dispatch<SignalType>(
                component, std::forward<Args>(args)...);

            systemManager.template dispatch<SignalType>(
                component.data(), std::forward<Args>(args)...);
        }
    }
};

}