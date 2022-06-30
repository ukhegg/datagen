//
// Created by ukhegg on 10.04.2017.
//

#ifndef DATAGEN_LIB_STL_LIMITS_HPP
#define DATAGEN_LIB_STL_LIMITS_HPP

#include <string>
#include "datagen/random.hpp"
#include "datagen/limits/detail/stl_limits_details.hpp"

namespace datagen
{
	namespace limits
	{
		struct container_size
		{
            static container_size_limit_t equals(size_t value)
            {
                return container_size_limit_t{value, value};
            }

			static container_size_limit_t between(size_t min, size_t max)
			{
				return container_size_limit_t(min, max);
			}

			static container_size_limit_t not_less_than(size_t min)
			{
				return container_size_limit_t(min, std::numeric_limits<size_t>::max());
			}

			static container_size_limit_t not_bigger_than(size_t max)
			{
				return container_size_limit_t(0, max);
			}
		};

		struct alphabet
		{
			template <class TChar>
			static alphabet_does_not_contain_limit_t<TChar> does_not_contain(const TChar* chars)
			{
				return alphabet_does_not_contain_limit_t<TChar>(chars);
			}

			template <class TChar>
			static alphabet_does_not_contain_limit_t<TChar> does_not_contain(std::basic_string<TChar> const& chars)
			{
				return alphabet_does_not_contain_limit_t<TChar>(chars);
			}

			template <class TChar>
			static alphabet_contains_limit_t<TChar> consists_of(const TChar* chars)
			{
				return alphabet_contains_limit_t<TChar>(chars);
			}

			template <class TChar>
			static alphabet_contains_limit_t<TChar> consists_of(std::basic_string<TChar> const& chars)
			{
				return alphabet_contains_limit_t<TChar>(chars);
			}
		};
	}
}
#endif //DATAGEN_LIB_STL_LIMITS_HPP
