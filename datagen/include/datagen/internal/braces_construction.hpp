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

        template<class T, class TInjector, std::size_t>
        using any_type_with_injector_t = any_type_with_injector<T, TInjector>;

        template<class...>
        struct is_braces_constructible;

        template<class T, std::size_t... Ns>
        struct is_braces_constructible<T, std::index_sequence<Ns...>>
                : std::is_constructible<T, any_type_t<T, Ns>...> {
        };

        template<class T, std::size_t... Ns>
        constexpr auto get_braces_initializer_size(std::index_sequence<Ns...>) noexcept {
            auto value = 0;
            int _[]{0, (is_braces_constructible<T, std::make_index_sequence<Ns>>{}
                        ? value = Ns : value)...};
            return value;
        }

        template<class T>
        constexpr auto get_braces_initializer_size() noexcept {
            return get_braces_initializer_size<T>(std::make_index_sequence<DATAGEN_CFG_BRACES_CTOR_LIMIT_SIZE>{});
        }

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
                return braces_initializer_invoker_impl<C, TInjector, get_braces_initializer_size<C>()>::create(injector);
            }
        };
    }
}
#endif //DATAGEN_BRACES_CONSTRUCTION_HPP
