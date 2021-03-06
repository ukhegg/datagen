//
// Created by ukhegg on 10.04.2017.
//

#ifndef DATAGEN_LIB_BUILTIN_LIMITS_HPP
#define DATAGEN_LIB_BUILTIN_LIMITS_HPP

#include "datagen/limits/random_limits.hpp"

namespace datagen
{
	namespace limits
	{
		namespace details
		{
			template <class TBuiltin>
			struct between_t
			{
				static_assert(std::is_fundamental<TBuiltin>::value, "TBuildin is not fundamental");

				between_t(TBuiltin lower_bound, TBuiltin upper_bound)
					: lower_bound(lower_bound),
					  upper_bound(upper_bound) { }

				TBuiltin lower_bound;
				TBuiltin upper_bound;
			};

			struct odd_t { };

			struct even_t { };
		}

		template <class TValueType, class TLimitType>
		void adjust_value(random_source_base&, details::between_t<TLimitType> const& limit, TValueType& value)
		{
			using common_type = decltype(std::declval<TValueType>() + std::declval<TLimitType>());

			if (static_cast<double>(limit.lower_bound) > static_cast<double>(std::numeric_limits<TValueType>::max())) {
				throw std::runtime_error("lower bound is bigger than target type's max");
			}
			auto m = std::numeric_limits<TValueType>::min();
			if (static_cast<double>(limit.upper_bound) < static_cast<double>(m))
			{
				throw std::runtime_error("upper bound is less than target type's min");
			}
			if (static_cast<double>(value) >= static_cast<double>(limit.lower_bound) &&
				static_cast<double>(value) <= static_cast<double>(limit.upper_bound)) return;

			auto max_range = static_cast<double>(std::numeric_limits<TValueType>::max())
					- static_cast<double>(std::numeric_limits<TValueType>::min());

			auto k = (static_cast<double>(value) - static_cast<double>(std::numeric_limits<TValueType>::min())) / max_range;
			auto range = static_cast<double>(limit.upper_bound - limit.lower_bound);
			value = static_cast<TValueType>(limit.lower_bound + k * range);
		}

		template <class TLimitType>
		void adjust_value(random_source_base&, details::between_t<TLimitType> const& limit, double& value)
		{
			auto max_range = static_cast<double>(std::numeric_limits<int64_t>::max())
					- static_cast<double>(std::numeric_limits<int64_t>::min());

			auto k = (static_cast<double>(value) - static_cast<double>(std::numeric_limits<int64_t>::min())) / max_range;
			auto range = static_cast<double>(limit.upper_bound - limit.lower_bound);
			value = limit.lower_bound + k * range;
		}

		template <class TBuiltin>
		void adjust_value(random_source_base&, details::odd_t const&, TBuiltin& value)
		{
			if(value % 2 == 1) return;
			if(value == std::numeric_limits<TBuiltin>::max()) value -= 1;
			else value += 1;
		}

		template <class TBuiltin>
		void adjust_value(random_source_base&, details::even_t const&, TBuiltin& value)
		{
			if(value % 2 == 0) return;
			if(value == std::numeric_limits<TBuiltin>::max()) value -= 1;
			else value += 1;
		}

		template <class TBuiltin1, class TBuildin2>
		auto between(TBuiltin1 lower_bound, TBuildin2 upper_bound)
		-> typename std::enable_if<std::is_fundamental<TBuiltin1>::value && std::is_fundamental<TBuildin2>::value,
		                           details::between_t<decltype(lower_bound + upper_bound)>>::type
		{
			using r_type = decltype(lower_bound + upper_bound);
			return details::between_t<r_type>(static_cast<r_type>(lower_bound),
			                                  static_cast<r_type>(upper_bound));
		}

		template <class TBuiltin>
		auto greater_than(TBuiltin val)
		-> typename std::enable_if<std::is_fundamental<TBuiltin>::value, details::between_t<TBuiltin>>::type
		{
			return details::between_t<TBuiltin>(val + std::numeric_limits<TBuiltin>::epsilon(), std::numeric_limits<TBuiltin>::max());
		}

		template <class TBuiltin>
		auto greater_or_equal_than(TBuiltin val)
		-> typename std::enable_if<std::is_fundamental<TBuiltin>::value, details::between_t<TBuiltin>>::type
		{
			return details::between_t<TBuiltin>(val, std::numeric_limits<TBuiltin>::max());
		}

		template <class TBuiltin>
		auto less_than(TBuiltin val)
		-> typename std::enable_if<std::is_fundamental<TBuiltin>::value, details::between_t<TBuiltin>>::type
		{
			return details::between_t<TBuiltin>(std::numeric_limits<TBuiltin>::min(), val - std::numeric_limits<TBuiltin>::epsilon());
		}

		template <class TBuiltin>
		auto less_or_equal_than(TBuiltin val)
		-> typename std::enable_if<std::is_fundamental<TBuiltin>::value, details::between_t<TBuiltin>>::type
		{
			return details::between_t<TBuiltin>(std::numeric_limits<TBuiltin>::min(), val);
		}

		template <class TInteger>
		details::odd_t odd()
		{
			static_assert(std::is_integral<TInteger>::value, "can be used only with integral types");
			return details::odd_t();
		}

		template <class TInteger>
		details::even_t even()
		{
			static_assert(std::is_integral<TInteger>::value, "can be used only with integral types");
			return details::even_t();
		}
	}
}
#endif //DATAGEN_LIB_BUILTIN_LIMITS_HPP
