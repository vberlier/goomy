#pragma once

namespace goomy {

template <typename EngineType>
class Entity;

template <typename EngineType, typename ComponentType>
class Component {
    using internalComponentType = internal::RegistryItem<internal::InternalComponent<
        typename EngineType::entityManagerType::entityType, ComponentType>>;

  public:
    Component(EngineType &engine, internalComponentType &component)
        : engineReference(engine), component(component) {
    }

    typename EngineType::entityManagerType::registryIndexType id() {
        return component.getIndex();
    }

    auto &engine() {
        return engineReference;
    }

    auto entity() {
        return Entity<EngineType>(engineReference,
                      engineReference.getEntityManager().getEntity(
                          component.getEntityIndex()));
    }

    ComponentType &data() {
        return component;
    }

    void destroy() {
        engineReference.getEntityManager()
            .template destroyComponent<ComponentType>(
                engineReference.getEntityManager().getEntity(
                    component.getEntityIndex()));
    }

  private:
    EngineType &engineReference;
    internalComponentType &component;
};

template <typename EngineType>
class Entity {
  public:
    Entity(EngineType &engine,
           typename EngineType::entityManagerType::entityType &entity)
        : engineReference(engine), entity(entity) {
    }

    typename EngineType::entityManagerType::registryIndexType id() {
        return entity.getIndex();
    }

    auto &engine() {
        return engineReference;
    }

    template <typename ComponentType>
    bool has() {
        return entity.template has<ComponentType>();
    }

    template <typename ComponentType>
    auto get() {
        return Component(engineReference,
                         engineReference.getEntityManager()
                             .template getComponent<ComponentType>(entity));
    }

    template <typename ComponentType, typename... Args>
    auto create(Args &&... args) {
        return Component(engineReference,
                         engineReference.getEntityManager()
                             .template createComponent<ComponentType>(
                                 entity, std::forward<Args>(args)...));
    }

    template <typename ComponentType, typename... Args>
    auto with(Args &&... args) {
        create<ComponentType>(std::forward<Args>(args)...);
        return *this;
    }

    void destroy() {
        engineReference.getEntityManager().destroyEntity(entity);
    }

  private:
    EngineType &engineReference;
    typename EngineType::entityManagerType::entityType &entity;
};

}