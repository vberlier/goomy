#pragma once

#include "goomy/Registry.hpp"

namespace goomy::internal {

template <typename EntityType, typename ComponentType>
class InternalComponent : public ComponentType {
  public:
    using entityIndexType = typename EntityType::indexType;
    using componentType = ComponentType;

    template <typename... Args>
    explicit InternalComponent(entityIndexType index, Args &&... args)
        : componentType(std::forward<Args>(args)...), entityIndex(index) {
    }

    auto getEntityIndex() const {
        return entityIndex;
    }

    void setEntityIndex(entityIndexType index) {
        entityIndex = index;
    }

  private:
    entityIndexType entityIndex;
};

template <typename EntityType, typename T, typename... Ts>
struct ComponentRegistryContainer
    : ComponentRegistryContainer<EntityType, T>,
      ComponentRegistryContainer<EntityType, Ts...> {
    using ComponentRegistryContainer<EntityType, T>::get;
    using ComponentRegistryContainer<EntityType, Ts...>::get;
};

template <typename EntityType, typename T>
struct ComponentRegistryContainer<EntityType, T> {
    template <typename U, typename = std::enable_if_t<std::is_same<T, U>{}>>
    auto &get() {
        return registry;
    }

  private:
    Registry<RegistryItem<InternalComponent<EntityType, T>>> registry;
};

}