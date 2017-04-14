//
// Created by ukhegg on 11.04.2017.
//

#ifndef DATAGEN_LIB_STD_ARRAY_HPP
#define DATAGEN_LIB_STD_ARRAY_HPP

#include <array>
#include <map>
#include "datagen/random_source_fwd.hpp"

namespace datagen
{
	template <class Ty, size_t N>
	struct value_generation_algorithm<std::array<Ty, N>>
	{
		using array_t = std::array<Ty, N>;

		array_t get_random(random_source_base& r_source)
		{
			array_t res;
			for(auto& e : res)
			{
				e = r_source.create<Ty>();
			}
			return std::move(res);
		}
	};
}
#endif //DATAGEN_LIB_STD_ARRAY_HPP
