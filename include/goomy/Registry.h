#pragma once

#include <algorithm>
#include <set>
#include <vector>

namespace goomy {

template <typename BaseType, typename IndexType = std::size_t>
class RegistryItem : public BaseType {
  public:
    using baseType = BaseType;
    using indexType = IndexType;

    template <typename... Args>
    RegistryItem(indexType index, Args... args)
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
    using indexType = typename T::indexType;

    T &get(indexType index) {
        std::size_t size = items.size();
        return index < size ? items[index] : created[index - size];
    }

    T &create() {
        return created.emplace_back(items.size() + created.size());
    }

    void destroy(T &item) {
        destroyed.insert(item.getIndex());
    }

    void flush() {
        std::move(created.begin(), created.end(), std::back_inserter(items));
        created.clear();

        for (auto it = destroyed.rbegin(); it != destroyed.rend(); it++) {
            auto index = *it;
            auto &back = items.back();

            back.setIndex(index);

            items[index] = std::move(back);
            items.pop_back();
        }

        destroyed.clear();
    }

    std::size_t size() {
        return items.size();
    }

  private:
    std::vector<T> items;
    std::vector<T> created;
    std::set<indexType> destroyed;
};

}