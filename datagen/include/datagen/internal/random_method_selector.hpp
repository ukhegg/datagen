//
// Created by ukhegg on 07.04.2017.
//

#ifndef DATAGEN_RANDOM_METHOD_SELECTOR_HPP
#define DATAGEN_RANDOM_METHOD_SELECTOR_HPP

#include "braces_construction.hpp"
#include "ctor_construction.hpp"
#include "explicit_random_invoker.hpp"

namespace datagen {
    namespace internal {
        struct none_t {
        };

        template<class TFirst, class ... TOthers>
        struct select_first_not_none {
            using type = TFirst;
        };

        template<class ... TOthers>
        struct select_first_not_none<none_t, TOthers ...> {
            using type = typename select_first_not_none<TOthers...>::type;
        };

        template<>
        struct select_first_not_none<none_t> {
            using type = none_t;
        };

        template<class TValue>
        struct random_method_selector {
        private:
            using braces_ctor = braces_ctor_traits<TValue>;
            using ctor = ctor_traits<TValue>;

            static const auto has_random_algorithm = has_random_generation_algorithm<TValue>::value;
        public:
            using braces_invoker = typename std::conditional<braces_ctor::is_constructible,
                    braces_initializer_invoker<TValue>,
                    none_t>::type;
            using ctor_invoker = typename std::conditional<ctor::is_constructible,
                    direct_ctor_invoker<TValue>,
                    none_t>::type;
            using direct_random_invoker = typename std::conditional<has_random_algorithm,
                    direct_get_random_invoker<TValue>,
                    none_t>::type;

            using invoker = typename select_first_not_none<direct_random_invoker,
                    braces_invoker,
                    ctor_invoker>::type;
        };
    }
}
#endif //DATAGEN_RANDOM_METHOD_SELECTOR_HPP
