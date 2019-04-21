#pragma once

#include <type_traits>
#include <vector>

namespace goomy {

template <typename T, typename... Ts>
struct component_indices : component_indices<T>, component_indices<Ts...> {
    using component_indices<T>::get;
    using component_indices<Ts...>::get;
    using component_indices<T>::set;
    using component_indices<Ts...>::set;
};

template <typename T>
struct component_indices<T> {
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
class Entity : public component_indices<Ts...> {
  public:
    template <typename T>
    bool has() {
        return component_indices<Ts...>::template get<T>() >= 0;
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