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
        return componentIndex;
    }

    template <typename U, typename = std::enable_if_t<std::is_same<T, U>{}>>
    void set(int i) {
        componentIndex = i;
    }

  private:
    int componentIndex = -1;
};

template <typename... Ts>
class Entity : public ComponentIndices<Ts...> {
  public:
    int index;

    Entity(int i) : index(i) {
    }

    // Disallow copy
    Entity(const Entity &entity) = delete;
    void operator=(const Entity &entity) = delete;

    // Allow move
    Entity(Entity &&entity) noexcept = default;
    Entity &operator=(Entity &&entity) noexcept = default;

    template <typename T>
    bool has() {
        return ComponentIndices<Ts...>::template get<T>() >= 0;
    }
};

template <typename EngineType, typename EntityType>
class EntityRegistry {
  public:
    explicit EntityRegistry(EngineType &engine) : engine(engine) {
    }

    std::size_t size() {
        return entities.size();
    }

    EntityType &operator[](int index) {
        return entities[index];
    }

    EntityType &create() {
        entities.emplace_back(entities.size());
        return entities.back();
    }

  private:
    EngineType &engine;
    std::vector<EntityType> entities;
};

}