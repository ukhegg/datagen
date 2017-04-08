//
// Created by ukhegg on 07.04.2017.
//

#ifndef DATAGEN_BRACES_CONSTRUCTION_HPP
#define DATAGEN_BRACES_CONSTRUCTION_HPP

#include "datagen/internal/any_type.hpp"
#include "datagen/internal/config.hpp"
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
            using t = check_braces_contructibility<T, is_braces_constructible<T, CurrentParamsCount - 1>::value,
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

        template<class C, class TInjector, size_t ParamsLeft>
        struct braces_initializer_invoker_impl {
            template<class ... Args>
            static C create(TInjector &injector, Args ... args) {
                using sub_invoker_t = braces_initializer_invoker_impl<C, TInjector, ParamsLeft - 1>;
                any_type_with_injector<C, TInjector> one_more_param(injector);
                return sub_invoker_t::create(injector, one_more_param, args...);
            }
        };

        template<class C, class TInjector>
        struct braces_initializer_invoker_impl<C, TInjector, 0> {
            template<class ... Args>
            static C create(TInjector &injector, Args ... args) {
                return std::move(C(args...));
            }
        };

        struct braces_initializer_invoker {
            template<class C, class TInjector>
            static C create(TInjector &injector) {
                using traits = braces_ctor_traits<C>;
                static_assert(traits::is_constructible,
                              "C is not constructible using braces");
                static const auto params_count = traits::params_count;
                return braces_initializer_invoker_impl<C, TInjector, params_count>::create(injector);
            }
        };
    }
}
#endif //DATAGEN_BRACES_CONSTRUCTION_HPP
