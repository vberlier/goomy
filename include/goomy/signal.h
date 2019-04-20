#pragma once

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
    using NAME##ReturnType =                                                   \
        decltype(std::declval<T>().NAME(std::declval<EngineType &>()));        \
                                                                               \
    template <typename T, typename = void>                                     \
    struct NAME##Method : std::false_type {};                                  \
                                                                               \
    template <typename T>                                                      \
    struct NAME##Method<T, std::void_t<NAME##ReturnType<T>>>                   \
        : std::is_same<NAME##ReturnType<T>, void> {};                          \
    template <typename SystemType>                                             \
    typename std::enable_if_t<!NAME##Method<SystemType>{}> NAME##Signal() {    \
    }                                                                          \
                                                                               \
    template <typename SystemType>                                             \
    typename std::enable_if_t<NAME##Method<SystemType>{}> NAME##Signal() {     \
        engine.template get<SystemType>().NAME(engine);                        \
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