//
// Created by ukhegg on 25.03.2017.
//

#ifndef DATAGEN_LIB_IS_CONSTRUCTIBLE_HPP
#define DATAGEN_LIB_IS_CONSTRUCTIBLE_HPP

#include <stdint.h>
#include <utility>
#include "any_type.hpp"

namespace datagen {
    namespace aux {
        constexpr auto BOOST_DI_CFG_CTOR_LIMIT_SIZE = 10;

        template<class T, std::size_t>
        using any_type_t = any_type<T>;

        template<class...>
        struct is_constructible;

        template<class T, std::size_t... Ns>
        struct is_constructible<T, std::index_sequence<Ns...>>
                : std::is_constructible<T, any_type_t<T, Ns>...> {
        };

        template<class T, std::size_t... Ns>
        constexpr auto get_ctor_size(std::index_sequence<Ns...>) noexcept {
            auto value = 0;
            int _[]{0, (is_constructible<T, std::make_index_sequence<Ns>>{}
                        ? value = Ns : value)...};
            return value;
        }
    }
}

#endif //DATAGEN_LIB_IS_CONSTRUCTIBLE_HPP
