//
// Created by ukhegg on 07.04.2017.
//

#ifndef DATAGEN_BRACES_CONSTRUCTION_HPP
#define DATAGEN_BRACES_CONSTRUCTION_HPP

#include "datagen/internal/any_type.hpp"
#include "datagen/internal/config.hpp"
#include "datagen/value_generation_algorithm.hpp"
#include <cstddef>
#include <utility>

namespace datagen {
    namespace internal {
        template<class T, std::size_t>
        using any_type_t = any_type<T>;

        template<class T>
        struct is_true {
            enum {
                value = std::is_same<T, std::true_type>::value
            };
        };


        template<class...>
        struct is_braces_constructible_impl;

        template<class T, size_t ... N>
        struct is_braces_constructible_impl<T, std::index_sequence<N...>> {
        private:
            template<class C, typename... CArgs>
            static decltype(void(C{std::declval<CArgs>()...}), std::true_type()) test(int);

            template<class C, typename... CArgs>
            static std::false_type test(...);

        public:
            enum {
                value = is_true<decltype(test<T, any_type_t<T, N>...>(0))>::value
            };
        };

        template<class T, size_t N>
        struct is_braces_constructible {
            enum {
                value = is_braces_constructible_impl<T, std::make_index_sequence<N>>::value
            };
        };

        template<class T, bool IsConstructible, size_t CurrentParamsCount>
        struct check_braces_contructibility;

        template<class T, size_t CurrentParamsCount>
        struct check_braces_contructibility<T, false, CurrentParamsCount> {
            using t = check_braces_contructibility<T,
                                                   is_braces_constructible<T, CurrentParamsCount - 1>::value,
                                                   CurrentParamsCount - 1>;
            static const auto params_count = t::params_count;
            static const auto is_constructible = t::is_constructible;
        };

        template<class T>
        struct check_braces_contructibility<T, false, 0> {
            static const auto params_count = 0;
            static const auto is_constructible = false;
        };

        template<class T, size_t CurrentParamsCount>
        struct check_braces_contructibility<T, true, CurrentParamsCount> {
            static const auto params_count = CurrentParamsCount;
            static const auto is_constructible = true;
        };

        template<class T>
        struct braces_ctor_traits {
        private:
            using check = check_braces_contructibility<T,
                    is_braces_constructible<T, DATAGEN_CFG_BRACES_CTOR_LIMIT_SIZE>::value,
                    DATAGEN_CFG_BRACES_CTOR_LIMIT_SIZE>;
        public:
            static const auto params_count = check::params_count;
            static const auto is_constructible = check::is_constructible;
        };

        template<class C, class TRandomSource, size_t ParamsLeft>
        struct braces_initializer_invoker_impl {
            template<class ... Args>
            static C create(TRandomSource &random_source, Args ... args) {
                using sub_invoker_t = braces_initializer_invoker_impl<C, TRandomSource, ParamsLeft - 1>;
                any_type_with_rsrc<C, TRandomSource> one_more_param(random_source);
                return sub_invoker_t::create(random_source, one_more_param, args...);
            }
        };

        template<class C, class TRandomSource>
        struct braces_initializer_invoker_impl<C, TRandomSource, 0> {
            template<class ... Args>
            static C create(TRandomSource &random_source, Args ... args) {
                return C{args...};
            }
        };

        template<class TValue>
        struct braces_initializer_invoker {
            template<class TRandomSource>
            static TValue create(value_generation_algorithm<TValue>& alg, TRandomSource &random_source) {
                using traits = braces_ctor_traits<TValue>;
                static_assert(traits::is_constructible,
                              "TValue is not constructible using braces");
                static const auto params_count = traits::params_count;
                return braces_initializer_invoker_impl<TValue, TRandomSource, params_count>::create(random_source);
            }
        };
    }
}
#endif //DATAGEN_BRACES_CONSTRUCTION_HPP
