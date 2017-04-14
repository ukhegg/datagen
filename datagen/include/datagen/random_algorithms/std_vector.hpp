//
// Created by ukhegg on 11.04.2017.
//
#pragma once
#include <datagen/limits/builtin_limits.hpp>
#include "datagen/random_source.hpp"
#include <vector>

namespace datagen
{
	template <class Type, class Allocator>
	struct value_generation_algorithm<std::vector<Type, Allocator>>
	{
		using vector_t = std::vector<Type, Allocator>;

		vector_t get_random(random_source_base& r_source)
		{
			vector_t res;
			auto s = r_source.create<size_t>(limits::between(this->min_size, this->max_size));
			res.reserve(s);
			for(auto i = 0 ;i < s ;++i)
			{
				res.push_back(std::move(r_source.create<Type>()));
			}
			return std::move(res);
		}

		size_t min_size{0};
		size_t max_size{30};
	};

	template <class Type, class Allocator>
	using vector_alg_t = value_generation_algorithm<std::vector<Type, Allocator>>;
}
