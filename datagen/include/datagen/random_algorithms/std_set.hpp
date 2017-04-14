//
// Created by ukhegg on 11.04.2017.
//

#ifndef DATAGEN_LIB_STD_SET_HPP
#define DATAGEN_LIB_STD_SET_HPP
#include <set>
#include <datagen/random_source_fwd.hpp>

namespace datagen
{
	template <class Key, class Traits, class Allocator>
	struct value_generation_algorithm<std::set<Key, Traits, Allocator>>
	{
		using set_t = std::set<Key, Traits, Allocator>;

		set_t get_random(random_source_base& r_source)
		{
			set_t res;
			auto s = r_source.create<size_t>(limits::between(this->min_size, this->max_size));
			size_t tries_count = 0;
			while(res.size() != s)
			{
				res.insert(std::move(r_source.create<Key>()));
				if(++tries_count > (100 + 10 * s))
				{
					throw std::runtime_error("hang-ip while generating random std::set");
				}
			}
			return std::move(res);
		};

		size_t min_size{0};
		size_t max_size{30};
	};
}
#endif //DATAGEN_LIB_STD_SET_HPP
