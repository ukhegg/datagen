//
// Created by ukhegg on 07.04.2017.
//

#ifndef DATAGEN_CTOR_CONSTRUCTION_HPP
#define DATAGEN_CTOR_CONSTRUCTION_HPP

#include "datagen/internal/config.hpp"
#include "datagen/internal/any_type.hpp"
#include "datagen/value_generation_algorithm.hpp"
#include <cstddef>
#include <utility>

namespace datagen {
    namespace internal {
        template<class T, std::size_t>
        using any_type_t = any_type<T>;

        template<class T, class TInjector, std::size_t>
        using any_type_with_injector_t = any_type_with_rsrc<T, TInjector>;

        template<class...>
        struct is_ctor_constructible_impl;

        template<class T, size_t ... N>
        struct is_ctor_constructible_impl<T, std::index_sequence<N...>> {
            enum {
                value = std::is_constructible<T, any_type_t<T, N>...>::value
            };
        };

        template<class T, size_t N>
        struct is_ctor_constructible {
            enum {
                value = is_ctor_constructible_impl<T, std::make_index_sequence<N>>::value
            };
        };

        template<class T, bool IsConstructible, size_t CurrentParamsCount>
        struct check_ctor_contructibility;

        template<class T, size_t CurrentParamsCount>
        struct check_ctor_contructibility<T, false, CurrentParamsCount> {
            using t = check_ctor_contructibility<T, is_ctor_constructible<T, CurrentParamsCount - 1>::value,
                    CurrentParamsCount - 1>;
            enum{
                params_count = t::params_count
            };
            enum{
                is_constructible = t::is_constructible
            };
        };

        template<class T>
        struct check_ctor_contructibility<T, false, 0> {
            enum{
                params_count = 0
            };
            enum
            {
                is_constructible = false
            };
        };

        template<class T, size_t CurrentParamsCount>
        struct check_ctor_contructibility<T, true, CurrentParamsCount> {
            enum{
                params_count = CurrentParamsCount
            };
            enum{
                is_constructible = true
            };
        };

        template<class T>
        struct ctor_traits {
        private:
            using check = check_ctor_contructibility<T,
                    is_ctor_constructible<T, DATAGEN_CFG_BRACES_CTOR_LIMIT_SIZE>::value,
                    DATAGEN_CFG_BRACES_CTOR_LIMIT_SIZE>;
        public:
            enum {
                params_count = check::params_count
            };
            enum{
                is_constructible = check::is_constructible
            };
        };

        template<class C, class TRandomSource, size_t ParamsLeft>
        struct direct_ctor_invoker_impl {
            template<class ... Args>
            static C create(TRandomSource &random_source, Args ... args) {
                using sub_invoker_t = direct_ctor_invoker_impl<C, TRandomSource, ParamsLeft - 1>;
                any_type_with_rsrc<C, TRandomSource> one_more_param(random_source);
                return sub_invoker_t::create(random_source, one_more_param, args...);
            }
        };

        template<class C, class TRandomSource>
        struct direct_ctor_invoker_impl<C, TRandomSource, 0> {
            template<class ... Args>
            static C create(TRandomSource &random_source, Args ... args) {
                return std::move(C(args...));
            }
        };

        template<class TValue>
        struct direct_ctor_invoker {
            template<class TRandomSource>
            static TValue create(value_generation_algorithm<TValue>& alg, TRandomSource &random_source) {
                using traits = ctor_traits<TValue>;
                static_assert(traits::is_constructible,
                              "TValue is not constructible using ctor");

                return direct_ctor_invoker_impl<TValue, TRandomSource, traits::params_count>::create(random_source);
            }
        };
    }
}
#endif //DATAGEN_CTOR_CONSTRUCTION_HPP
