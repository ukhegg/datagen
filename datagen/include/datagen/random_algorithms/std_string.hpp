//
// Created by ukhegg on 11.04.2017.
//

#ifndef DATAGEN_LIB_STD_STRING_HPP
#define DATAGEN_LIB_STD_STRING_HPP

#include <datagen/limits/builtin_limits.hpp>
#include "datagen/random_source.hpp"

namespace datagen
{
	template <class CharType, class Traits, class Allocator>
	struct value_generation_algorithm<std::basic_string<CharType, Traits, Allocator>>
	{
		using string_t = std::basic_string<CharType, Traits, Allocator>;

		string_t get_random(random_source_base& r_source)
		{
			if (alphabet.empty()) throw std::runtime_error("empty alphabet");
			string_t res;
			auto size = r_source.create<size_t>(limits::between(min_size, max_size));
			res.resize(size);
			for (auto& c : res)
			{
				c = alphabet.at(r_source.create<size_t>(limits::between(0, alphabet.size() - 1)));
			}
			return std::move(res);
		};

		size_t min_size{0};
		size_t max_size{30};
		std::basic_string<CharType> alphabet{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"};
	};
}
#endif //DATAGEN_LIB_STD_STRING_HPP
