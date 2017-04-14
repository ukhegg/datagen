//
// Created by ukhegg on 11.04.2017.
//

#ifndef DATAGEN_LIB_STD_MAP_HPP
#define DATAGEN_LIB_STD_MAP_HPP

#include <map>
#include <datagen/random_source_fwd.hpp>

namespace datagen
{
	template <class Key, class TValue, class Traits, class Allocator>
	struct value_generation_algorithm<std::map<Key, TValue, Traits, Allocator>>
	{
		using map_t = std::map<Key, TValue, Traits, Allocator>;

		map_t get_random(random_source_base& r_source)
		{
			map_t res;
			auto s = r_source.create<size_t>(limits::between(this->min_size, this->max_size));
			size_t tries_count = 0;
			while(res.size() != s)
			{
				auto key = std::move(r_source.create<Key>());
				if(res.find(key) == res.end())
				{
					res[key] = std::move(r_source.create<TValue>());
				}
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
#endif //DATAGEN_LIB_STD_MAP_HPP
