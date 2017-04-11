//
// Created by ukhegg on 10.04.2017.
//

#ifndef DATAGEN_LIB_STL_LIMITS_HPP
#define DATAGEN_LIB_STL_LIMITS_HPP

#include <string>
#include <set>
#include "datagen/random.hpp"
#include "datagen/stl_random.hpp"
#include "datagen/limits/random_limits.hpp"

namespace datagen
{
	namespace limits
	{
		namespace details
		{
			template <class _Elem>
			struct alphabet_limit
			{
				explicit alphabet_limit(std::basic_string<_Elem> const& chars)
					: alphabet(chars)
				{
				}

				std::basic_string<_Elem> alphabet;
			};
		}

		template <class _Elem, class _Traits, class _Alloc>
		void adjust_algorithm(random_source_base& r_source,
		                      details::alphabet_limit<_Elem> const& limit,
		                      value_generation_algorithm<std::basic_string<_Elem, _Traits, _Alloc>>& alg_params)
		{
			alg_params.alphabet = limit.alphabet;
		}

		template <class TChar>
		details::alphabet_limit<TChar> valid_chars(const TChar* chars)
		{
			return details::alphabet_limit<TChar>(chars);
		}
	}
}
#endif //DATAGEN_LIB_STL_LIMITS_HPP
