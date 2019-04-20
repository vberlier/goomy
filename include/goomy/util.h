#pragma once

#include <type_traits>

namespace goomy::util {

template <typename T, typename... Ts>
struct container : container<T>, container<Ts...> {
    using container<T>::get;
    using container<Ts...>::get;
};

template <typename T>
struct container<T> {
    template <typename U, typename = std::enable_if_t<std::is_same<T, U>{}>>
    U &get() {
        return instance;
    }

  private:
    T instance;
};

}