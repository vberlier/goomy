#pragma once

#include <algorithm>
#include <set>
#include <vector>

namespace goomy::internal {

template <typename BaseType, typename IndexType = std::size_t>
class RegistryItem : public BaseType {
  public:
    using baseType = BaseType;
    using indexType = IndexType;

    template <typename... Args>
    explicit RegistryItem(indexType index, Args &&... args)
        : registryIndex(index), baseType(std::forward<Args>(args)...) {
    }

    // Disallow copy
    RegistryItem(const RegistryItem &item) = delete;
    void operator=(const RegistryItem &item) = delete;

    // Allow move
    RegistryItem(RegistryItem &&item) noexcept = default;
    RegistryItem &operator=(RegistryItem &&item) noexcept = default;

    indexType getIndex() const {
        return registryIndex;
    }

    void setIndex(indexType index) {
        registryIndex = index;
    }

  private:
    indexType registryIndex;
};

template <typename T>
class Registry {
  public:
    using registryType = Registry<T>;
    using indexType = typename T::indexType;

    Registry() = default;

    // Disallow copy
    Registry(const registryType &registry) = delete;
    void operator=(const registryType &registry) = delete;

    T &get(indexType index) {
        auto size = items.size();
        return index < size ? items[index] : created[index - size];
    }

    template <typename... Args>
    T &create(Args &&... args) {
        return created.emplace_back(items.size() + created.size(),
                                    std::forward<Args>(args)...);
    }

    void destroy(T &item) {
        destroyed.insert(item.getIndex());
    }

    void flush() {
        std::move(created.begin(), created.end(), std::back_inserter(items));

        for (auto it = destroyed.rbegin(); it != destroyed.rend(); it++) {
            auto index = *it;
            auto &back = items.back();

            back.setIndex(index);

            items[index] = std::move(back);
            items.pop_back();
        }
    }

    void reset() {
        created.clear();
        destroyed.clear();
    }

    auto begin() const {
        return items.begin();
    }

    auto end() const {
        return items.end();
    }

    auto createdBegin() const {
        return created.begin();
    }

    auto createdEnd() const {
        return created.end();
    }

    auto destroyedBegin() const {
        return destroyed.begin();
    }

    auto destroyedEnd() const {
        return destroyed.end();
    }

    std::size_t size() const {
        return items.size();
    }

  private:
    std::vector<T> items;
    std::vector<T> created;
    std::set<indexType> destroyed;
};

}