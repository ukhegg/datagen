//
// Created by ukhegg on 10.04.2017.
//

#ifndef DATAGEN_LIB_RANDOM_LIMITS_HPP
#define DATAGEN_LIB_RANDOM_LIMITS_HPP

#include "datagen/value_generation_algorithm.hpp"
#include <type_traits>

namespace datagen
{
    class random_source_base;

    namespace limits
    {
        namespace dummies
        {
            void adjust_algorithm();

            void adjust_value();
        }

        struct limits_applicator
        {

            template<class TValue, class TLimit>
            static auto do_adjust_value(random_source_base &rs,
                                        TLimit const &l,
                                        TValue &v)
            -> typename std::enable_if_t<std::is_same_v<decltype(adjust_value(rs, l, v)), void>,
                    std::true_type>
            {
                using ::datagen::limits::dummies::adjust_value;
                adjust_value(rs, l, v);
                return {};
            }


            template<class TValue, class TLimit>
            static auto do_adjust_algorithm(random_source_base &rs,
                                            TLimit const &l,
                                            value_generation_algorithm<TValue> &vga)
            -> typename std::enable_if<std::is_same<decltype(adjust_algorithm(rs, l, vga)), void>::value,
                    std::true_type>::type
            {
                using ::datagen::limits::dummies::adjust_algorithm;
                adjust_algorithm(rs, l, vga);
                return {};
            }

            static std::false_type do_adjust_value(random_source_base &rs, ...)
            {
                return {};
            }


            static std::false_type do_adjust_algorithm(random_source_base &rs, ...)
            {
                return {};
            }
        };

        namespace internal
        {
            template<class TValue, class TLimit>
            static constexpr bool is_algorithm_limit_v = std::is_same_v<std::true_type,
                    decltype(limits_applicator::do_adjust_algorithm(
                            std::declval<datagen::random_source_base &>(),
                            std::declval<TLimit const &>(),
                            std::declval<value_generation_algorithm<TValue> &>()))>;


            template<class TValue, class TLimit>
            static constexpr bool is_value_limit_v = std::is_same_v<std::true_type,
                    decltype(limits_applicator::do_adjust_value(
                            std::declval<datagen::random_source_base &>(),
                            std::declval<TLimit const &>(),
                            std::declval<TValue &>()))>;


            template<class TValue, class ... TLimits>
            struct limits_enumerator_t
            {
            };

            template<class TValue>
            struct limits_enumerator_t<TValue>
            {
                static void apply_algorithm_limits(random_source_base &r_source,
                                                   value_generation_algorithm<TValue> &alg)
                {
                }

                static void apply_value_limits(random_source_base &r_source,
                                               TValue &value)
                {
                }
            };

            template<class TValue, class TLimit, class ... TOtherLimits>
            struct limits_enumerator_t<TValue, TLimit, TOtherLimits...>
            {
                static_assert(is_algorithm_limit_v<TValue, TLimit> || is_value_limit_v<TValue, TLimit>,
                              "TLimit is neither algorithm nor value limit for TValue");

                static void apply_algorithm_limits(random_source_base &r_source,
                                                   value_generation_algorithm<TValue> &alg,
                                                   TLimit const &limit,
                                                   TOtherLimits const &... other_limits)
                {
                    limits_applicator::do_adjust_algorithm(r_source, limit, alg);
                    limits_enumerator_t<TValue, TOtherLimits...>::apply_algorithm_limits(r_source,
                                                                                         alg,
                                                                                         other_limits...);
                }

                static void apply_value_limits(random_source_base &r_source,
                                               TValue &value,
                                               TLimit const &limit,
                                               TOtherLimits const &... other_limits)
                {
                    limits_applicator::do_adjust_value(r_source, limit, value);
                    limits_enumerator_t<TValue, TOtherLimits...>::apply_value_limits(r_source,
                                                                                     value,
                                                                                     other_limits...);
                }
            };
        }

        template<class TValue, class ... TLimits>
        void apply_algorithm_limits(random_source_base &r_source,
                                    value_generation_algorithm<TValue> &alg,
                                    TLimits const &... limits)
        {
            internal::limits_enumerator_t<TValue, TLimits...>::apply_algorithm_limits(r_source,
                                                                                      alg,
                                                                                      limits...);
        }

        template<class TValue, class ... TLimits>
        void apply_value_limits(random_source_base &r_source,
                                TValue &value,
                                TLimits const &... limits)
        {
            internal::limits_enumerator_t<TValue, TLimits...>::apply_value_limits(r_source,
                                                                                  value,
                                                                                  limits...);
        }
    }
}
#endif //DATAGEN_LIB_RANDOM_LIMITS_HPP
