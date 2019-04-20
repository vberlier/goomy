#pragma once

#include <experimental/type_traits>
#include <type_traits>

namespace goomy {

template <typename EngineType>
class SignalDispatcher {
  public:
    explicit SignalDispatcher(EngineType &engine) : engine(engine) {
    }

  private:
    EngineType &engine;

#define GENERATE_SIGNAL(NAME)                                                  \
  private:                                                                     \
    template <typename T>                                                      \
    using NAME##Detector =                                                     \
        decltype(std::declval<T>().NAME(std::declval<EngineType &>()));        \
                                                                               \
    template <typename T>                                                      \
    using NAME##DetectorOmit = decltype(std::declval<T>().NAME());             \
                                                                               \
    template <typename T>                                                      \
    using NAME##Method = std::experimental::is_detected<NAME##Detector, T>;    \
                                                                               \
    template <typename T>                                                      \
    using NAME##MethodOmit =                                                   \
        std::experimental::is_detected<NAME##DetectorOmit, T>;                 \
                                                                               \
    template <typename SystemType>                                             \
    typename std::enable_if_t<!NAME##Method<SystemType>{} &&                   \
                              !NAME##MethodOmit<SystemType>{}>                 \
        NAME##Signal() {                                                       \
    }                                                                          \
                                                                               \
    template <typename SystemType>                                             \
    typename std::enable_if_t<NAME##Method<SystemType>{}> NAME##Signal() {     \
        engine.template get<SystemType>().NAME(engine);                        \
    }                                                                          \
                                                                               \
    template <typename SystemType>                                             \
    typename std::enable_if_t<NAME##MethodOmit<SystemType>{}> NAME##Signal() { \
        engine.template get<SystemType>().NAME();                              \
    }

#define GENERATE_DISPATCHER(NAME)                                              \
    GENERATE_SIGNAL(pre##NAME)                                                 \
    GENERATE_SIGNAL(NAME)                                                      \
    GENERATE_SIGNAL(post##NAME)                                                \
                                                                               \
  public:                                                                      \
    template <typename... SystemTypes>                                         \
    void NAME() {                                                              \
        (pre##NAME##Signal<SystemTypes>(), ...);                               \
        (NAME##Signal<SystemTypes>(), ...);                                    \
        (post##NAME##Signal<SystemTypes>(), ...);                              \
    }

    GENERATE_DISPATCHER(init)
    GENERATE_DISPATCHER(update)

#undef GENERATE_DISPATCHER
#undef GENERATE_SIGNAL
};

}