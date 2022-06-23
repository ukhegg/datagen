//
// Created by ukhegg on 10.04.2017.
//

#ifndef DATAGEN_LIB_BUILTIN_LIMITS_HPP
#define DATAGEN_LIB_BUILTIN_LIMITS_HPP

#include "datagen/limits/random_limits.hpp"

namespace datagen::limits
{
    namespace details
    {
        template<typename T, typename T2 = T> inline constexpr bool has_less_operator_v
                = std::is_same_v<decltype(std::declval<T &>() < std::declval<T2 &>()), bool>;

        template<typename T, typename T2 = T> inline constexpr bool has_plus_operator_v
                = std::is_same_v<decltype(std::declval<T &>() + std::declval<T2 &>()), bool>;
    }

    template<class TValue>
    struct between_t
    {
        static_assert(details::has_less_operator_v<TValue>, "TValue is not less-comparable");

        between_t(TValue lower_bound, TValue upper_bound)
                : lower_bound(lower_bound),
                  upper_bound(upper_bound)
        {
        }

        TValue lower_bound;
        TValue upper_bound;
    };

    struct odd_t
    {
    };

    struct even_t
    {
    };

    template<class TValueType, class TLimitType>
    auto adjust_value(random_source_base &, between_t<TLimitType> const &limit, TValueType &value)
    -> std::enable_if_t<(std::is_integral_v<TValueType> || std::is_floating_point_v<TValueType>)
                        && (std::is_integral_v<TLimitType> || std::is_floating_point_v<TValueType>), void>
    {
        using _ = decltype(std::declval<TValueType>() + std::declval<TLimitType>());

        if (static_cast<double>(limit.lower_bound) > static_cast<double>(std::numeric_limits<TValueType>::max())) {
            throw std::runtime_error("lower bound is bigger than target type's max");
        }
        auto m = std::numeric_limits<TValueType>::min();
        if (static_cast<double>(limit.upper_bound) < static_cast<double>(m)) {
            throw std::runtime_error("upper bound is less than target type's min");
        }
        if (static_cast<double>(value) >= static_cast<double>(limit.lower_bound) &&
            static_cast<double>(value) <= static_cast<double>(limit.upper_bound))
            return;

        auto max_range = static_cast<double>(std::numeric_limits<TValueType>::max())
                         - static_cast<double>(std::numeric_limits<TValueType>::min());

        auto k = (static_cast<double>(value) - static_cast<double>(std::numeric_limits<TValueType>::min())) /
                 max_range;
        auto range = static_cast<double>(limit.upper_bound - limit.lower_bound);
        value = static_cast<TValueType>(limit.lower_bound + k * range);
    }

    template<class TLimitType>
    void adjust_value(random_source_base &, between_t<TLimitType> const &limit, double &value)
    {
        auto max_range = static_cast<double>(std::numeric_limits<int64_t>::max())
                         - static_cast<double>(std::numeric_limits<int64_t>::min());

        auto k =
                (static_cast<double>(value) - static_cast<double>(std::numeric_limits<int64_t>::min())) / max_range;
        auto range = static_cast<double>(limit.upper_bound - limit.lower_bound);
        value = limit.lower_bound + k * range;
    }

    template<class TValue>
    void adjust_value(random_source_base &, odd_t const &, TValue &value)
    {
        if (value % 2 == 1) return;
        if (value == std::numeric_limits<TValue>::max()) value -= 1;
        else value += 1;
    }

    template<class TValue>
    void adjust_value(random_source_base &, even_t const &, TValue &value)
    {
        if (value % 2 == 0) return;
        if (value == std::numeric_limits<TValue>::max()) value -= 1;
        else value += 1;
    }

    template<class TValue1, class TValue2>
    auto between(TValue1 lower_bound, TValue2 upper_bound)
    -> typename std::enable_if<std::is_fundamental<TValue1>::value && std::is_fundamental<TValue2>::value,
            between_t<decltype(lower_bound + upper_bound)>>::type
    {
        using r_type = decltype(lower_bound + upper_bound);
        return between_t<r_type>(static_cast<r_type>(lower_bound),
                                 static_cast<r_type>(upper_bound));
    }

    template<class TValue>
    auto greater_than(TValue val)
    -> typename std::enable_if<std::is_fundamental<TValue>::value, between_t<TValue>>::type
    {
        return between_t<TValue>(val + std::numeric_limits<TValue>::epsilon(),
                                 std::numeric_limits<TValue>::max());
    }

    template<class TValue>
    auto greater_or_equal_than(TValue val)
    -> typename std::enable_if<std::is_fundamental<TValue>::value, between_t<TValue>>::type
    {
        return between_t<TValue>(val, std::numeric_limits<TValue>::max());
    }

    template<class TValue>
    auto less_than(TValue val)
    -> typename std::enable_if<std::is_fundamental<TValue>::value, between_t<TValue>>::type
    {
        return between_t<TValue>(std::numeric_limits<TValue>::min(),
                                 val - std::numeric_limits<TValue>::epsilon());
    }

    template<class TValue>
    auto less_or_equal_than(TValue val)
    -> typename std::enable_if<std::is_fundamental<TValue>::value, between_t<TValue>>::type
    {
        return between_t<TValue>(std::numeric_limits<TValue>::min(), val);
    }

    template<class TInteger>
    odd_t odd()
    {
        static_assert(std::is_integral<TInteger>::value, "can be used only with integral types");
        return odd_t();
    }

    template<class TInteger>
    even_t even()
    {
        static_assert(std::is_integral<TInteger>::value, "can be used only with integral types");
        return even_t();
    }
}
#endif //DATAGEN_LIB_BUILTIN_LIMITS_HPP
