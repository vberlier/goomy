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

    template <typename ComponentType>
    auto &getComponent(entityType &entity) {
        auto &components = componentRegistries.template get<ComponentType>();
        return components.get(entity.template get<ComponentType>());
    }

    auto &createEntity() {
        return registry.create();
    }

    template <typename ComponentType, typename... Args>
    auto &createComponent(entityType &entity, Args &&... args) {
        auto &components = componentRegistries.template get<ComponentType>();

        if (entity.template has<ComponentType>()) {
            components.destroy(
                components.get(entity.template get<ComponentType>()));
        }

        auto &component =
            components.create(entity.getIndex(), std::forward<Args>(args)...);

        entity.template set<ComponentType>(component.getIndex());

        return component;
    }

    void destroyEntity(entityType &entity) {
        (destroyComponent<ComponentTypes>(entity), ...);
        registry.destroy(entity);
    }

    template <typename ComponentType>
    void destroyComponent(entityType &entity) {
        if (!entity.template has<ComponentType>()) {
            return;
        }
        auto &components = componentRegistries.template get<ComponentType>();

        components.destroy(
            components.get(entity.template get<ComponentType>()));
    }

    void flush() {
        (flushComponents<ComponentTypes>(), ...);

        registry.flush();

        for (auto it = registry.destroyedBegin(); it != registry.destroyedEnd();
             it++) {
            auto index = *it;

            if (index >= registry.size()) {
                break;
            }

            auto &entity = getEntity(index);

            (updateEntityIndex<ComponentTypes>(entity), ...);
        }

        registry.reset();
    }

    std::size_t getEntityCount() const {
        return registry.size();
    }

  private:
    Registry<entityType> registry;
    ComponentRegistryContainer<entityType, ComponentTypes...>
        componentRegistries;

    template <typename ComponentType>
    void flushComponents() {
        auto &components = componentRegistries.template get<ComponentType>();

        for (auto it = components.destroyedBegin();
             it != components.destroyedEnd(); it++) {
            auto index = *it;

            getEntity(components.get(index).getEntityIndex())
                .template set<ComponentType>(-1);
        }

        components.flush();

        for (auto it = components.destroyedBegin();
             it != components.destroyedEnd(); it++) {
            auto index = *it;

            if (index >= components.size()) {
                break;
            }

            getEntity(components.get(index).getEntityIndex())
                .template set<ComponentType>(index);
        }

        components.reset();
    }

    template <typename ComponentType>
    void updateEntityIndex(entityType &entity) {
        if (entity.template has<ComponentType>()) {
            getComponent<ComponentType>(entity).setEntityIndex(
                entity.getIndex());
        }
    }
};

}