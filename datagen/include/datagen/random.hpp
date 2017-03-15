//
// Created by ukhegg on 15.03.2017.
//

#ifndef DATAGEN_LIB_RANDOM_HPP
#define DATAGEN_LIB_RANDOM_HPP

#include <type_traits>
#include <utility>
#include "datagen/datagen.hpp"
#include <stdexcept>

namespace datagen {

    namespace impl {
        template<class t_value>
        struct random {
            t_value operator()();
        };

        template<class t_value>
        class value_limitation {
        public:
            virtual ~value_limitation() = default;

            virtual void apply(t_value &val) const = 0;
        };
    }

    template<class t_value>
    inline t_value random() {
        return impl::random<t_value>()();
    }

    template<class t_value>
    inline t_value random(impl::value_limitation<t_value> const &limit) {
        t_value val = std::move(random<t_value>());
        limit.apply(val);
        return val;
    }
}

#endif //DATAGEN_LIB_RANDOM_HPP
