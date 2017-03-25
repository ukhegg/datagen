//
// Created by ukhegg on 25.03.2017.
//

#ifndef DATAGEN_LIB_ANY_TYPE_HPP
#define DATAGEN_LIB_ANY_TYPE_HPP

#include <type_traits>
#include "datagen/injector.hpp"

namespace datagen {
    namespace aux {

        template<class TParent>
        struct any_type {
            template<class T, class =
            std::enable_if_t<!std::is_same<TParent, T>{}>
            > constexpr operator T();
        };

    }
}
#endif //DATAGEN_LIB_ANY_TYPE_HPP
