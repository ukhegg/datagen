//
// Created by ukhegg on 07.04.2017.
//

#ifndef DATAGEN_CTOR_CONSTRUCTION_HPP
#define DATAGEN_CTOR_CONSTRUCTION_HPP

#include "datagen/internal/config.hpp"
#include "datagen/internal/any_type.hpp"
#include <cstddef>
#include <utility>

namespace datagen {
    namespace internal {
        template<class T, std::size_t>
        using any_type_t = any_type<T>;

        template<class T, class TInjector, std::size_t>
        using any_type_with_injector_t = any_type_with_injector<T, TInjector>;

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
            static const auto params_count = t::params_count;
            static const auto is_constructible = t::is_constructible;
        };

        template<class T>
        struct check_ctor_contructibility<T, false, 0> {
            static const auto params_count = 0;
            static const auto is_constructible = false;
        };

        template<class T, size_t CurrentParamsCount>
        struct check_ctor_contructibility<T, true, CurrentParamsCount> {
            static const auto params_count = CurrentParamsCount;
            static const auto is_constructible = true;
        };

        template<class T>
        struct ctor_traits {
        private:
            using check = check_ctor_contructibility<T,
                    is_ctor_constructible<T, DATAGEN_CFG_BRACES_CTOR_LIMIT_SIZE>::value,
                    DATAGEN_CFG_BRACES_CTOR_LIMIT_SIZE>;
        public:
            static const auto params_count = check::params_count;
            static const auto is_constructible = check::is_constructible;
        };

        template<class C, class TInjector, size_t ParamsLeft>
        struct direct_ctor_invoker_impl {
            template<class ... Args>
            static C create(TInjector &injector, Args ... args) {
                using sub_invoker_t = direct_ctor_invoker_impl<C, TInjector, ParamsLeft - 1>;
                any_type_with_injector<C, TInjector> one_more_param(injector);
                return sub_invoker_t::create(injector, one_more_param, args...);
            }
        };

        template<class C, class TInjector>
        struct direct_ctor_invoker_impl<C, TInjector, 0> {
            template<class ... Args>
            static C create(TInjector &injector, Args ... args) {
                return std::move(C(args...));
            }
        };

        struct direct_ctor_invoker {
            template<class C, class TInjector>
            static C create(TInjector &injector) {
                using traits = ctor_traits<C>;
                static_assert(traits::is_constructible,
                              "C is not constructible using ctor");

                return direct_ctor_invoker_impl<C, TInjector, traits::params_count>::create(injector);
            }
        };
    }
}
#endif //DATAGEN_CTOR_CONSTRUCTION_HPP
