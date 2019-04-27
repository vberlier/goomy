#pragma once

namespace goomy {

namespace internal {

template <typename EngineType, typename T>
class Iterator : public std::iterator<std::input_iterator_tag, T> {
    using indexType = typename EngineType::entityManagerType::registryIndexType;
    using iteratorType = Iterator<EngineType, T>;

  public:
    Iterator(EngineType &engine, indexType index)
        : engine(engine), index(index) {
    }

    iteratorType &operator++() {
        index++;
        return *this;
    }

    iteratorType operator++(int) {
        return iteratorType(engine, index + 1);
    }

    bool operator==(iteratorType other) const {
        return index == other.index;
    }

    bool operator!=(iteratorType other) const {
        return !(*this == other);
    }

    auto operator*() const {
        return T::find(engine, index);
    }

  private:
    EngineType &engine;
    indexType index;
};

template <typename EngineType, typename T>
class Range {
    using iterator = Iterator<EngineType, T>;
    using indexType = typename EngineType::indexType;

  public:
    Range(EngineType &engine, indexType end) : engine(engine), endIndex(end) {
    }

    iterator begin() {
        return iterator(engine, 0);
    }

    iterator end() {
        return iterator(engine, endIndex);
    }

  private:
    EngineType &engine;
    indexType endIndex;
};

} // namespace internal

template <typename EngineType>
class Entity;

template <typename EngineType, typename ComponentType>
class Component {
    using internalComponentType =
        internal::RegistryItem<internal::InternalComponent<
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

    static auto
    find(EngineType &engine,
         typename EngineType::entityManagerType::registryIndexType index) {
        return Component(engine, engine.getEntityManager()
            .getComponentRegistries().template get<ComponentType>()
            .get(index));
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

    static auto
    find(EngineType &engine,
         typename EngineType::entityManagerType::registryIndexType index) {
        return engine.entity(index);
    }

  private:
    EngineType &engineReference;
    typename EngineType::entityManagerType::entityType &entity;
};

}