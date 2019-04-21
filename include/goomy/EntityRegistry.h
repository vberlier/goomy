#pragma once

#include <type_traits>
#include <vector>

namespace goomy {

template <typename T, typename... Ts>
struct ComponentIndices : ComponentIndices<T>, ComponentIndices<Ts...> {
    using ComponentIndices<T>::get;
    using ComponentIndices<Ts...>::get;
    using ComponentIndices<T>::set;
    using ComponentIndices<Ts...>::set;
};

template <typename T>
struct ComponentIndices<T> {
    template <typename U, typename = std::enable_if_t<std::is_same<T, U>{}>>
    int get() {
        return index;
    }

    template <typename U, typename = std::enable_if_t<std::is_same<T, U>{}>>
    void set(int i) {
        index = i;
    }

  private:
    int index = -1;
};

template <typename... Ts>
class Entity : public ComponentIndices<Ts...> {
  public:
    template <typename T>
    bool has() {
        return ComponentIndices<Ts...>::template get<T>() >= 0;
    }
};

template <typename EngineType, typename EntityType, typename... ComponentTypes>
class EntityRegistry {
  public:
    explicit EntityRegistry(EngineType &engine) : engine(engine) {
    }

    EntityType &operator[](int index) {
        return entities[index];
    }

    EntityType &create() {
        entities.emplace_back();
        return entities.back();
    }

  private:
    EngineType &engine;
    std::vector<EntityType> entities;
};

}