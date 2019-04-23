#pragma once

#include "goomy/Registry.h"
#include <algorithm>
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
    template <typename T>
    bool has() {
        return ComponentIndices<Ts...>::template get<T>() >= 0;
    }
};

template <typename... ComponentTypes>
class EntityManager {
  public:
    using entityManagerType = EntityManager<ComponentTypes...>;
    using entityType = RegistryItem<Entity<ComponentTypes...>>;
    using registryIndexType = typename entityType::indexType;

    EntityManager() = default;

    // Disallow copy
    EntityManager(const entityManagerType &engine) = delete;
    void operator=(const entityManagerType &engine) = delete;

    auto &getEntity(registryIndexType index) {
        return registry.get(index);
    }

    auto &createEntity() {
        return registry.create();
    }

    void destroyEntity(entityType &entity) {
        registry.destroy(entity);
    }

    void flush() {
        registry.flush();
    }

    std::size_t getEntityCount() const {
        return registry.size();
    }

  private:
    Registry<entityType> registry;
};

}