//
// Created by ukhegg on 12.04.2017.
//

#ifndef DATAGEN_LIB_STL_LIMITS_DETAILS_HPP
#define DATAGEN_LIB_STL_LIMITS_DETAILS_HPP

namespace datagen
{
	namespace limits
	{
        template <class _Elem>
        struct alphabet_contains_limit_t
        {
            explicit alphabet_contains_limit_t(std::basic_string<_Elem> const& chars)
                    : alphabet(chars) { }

            std::basic_string<_Elem> alphabet;
        };

        template <class _Elem>
        struct alphabet_does_not_contain_limit_t
        {
            explicit alphabet_does_not_contain_limit_t(std::basic_string<_Elem> const& chars)
                    : chars(chars) { }

            std::basic_string<_Elem> chars;
        };

        struct container_size_limit_t
        {
            container_size_limit_t(size_t min, size_t max)
                    : min_elements(min), max_elements(max)
            {
                if(min > max) throw std::invalid_argument("invalid container size limit");
            }

            size_t min_elements;
            size_t max_elements;
        };

		template <class _Elem, class _Traits, class _Alloc>
		void adjust_algorithm(random_source_base& r_source,
		                      alphabet_contains_limit_t<_Elem> const& limit,
		                      value_generation_algorithm<std::basic_string<_Elem, _Traits, _Alloc>>& alg_params)
		{
			alg_params.alphabet = limit.alphabet;
		}

		template <class _Elem, class _Traits, class _Alloc>
		void adjust_algorithm(random_source_base& r_source,
		                      alphabet_does_not_contain_limit_t<_Elem> const& limit,
		                      value_generation_algorithm<std::basic_string<_Elem, _Traits, _Alloc>>& alg_params)
		{
			std::basic_string<_Elem> new_alphabet;
			new_alphabet.reserve(alg_params.alphabet.size());
			for(auto c : alg_params.alphabet)
			{
				if(limit.chars.find(c) == limit.chars.npos) new_alphabet.push_back(c);
			}
			alg_params.alphabet = new_alphabet;
		}

		template <class _Elem, class _Traits, class _Alloc>
		void adjust_algorithm(random_source_base& r_source,
		                      container_size_limit_t const& limit,
		                      value_generation_algorithm<std::basic_string<_Elem, _Traits, _Alloc>>& alg_params)
		{
			alg_params.min_size = limit.min_elements;
			alg_params.max_size = limit.max_elements;
		}

		template <class Type, class Allocator>
		void adjust_algorithm(random_source_base& r_source,
		                      container_size_limit_t const& limit,
		                      value_generation_algorithm<std::vector<Type, Allocator>>& alg_params)
		{
			alg_params.min_size = limit.min_elements;
			alg_params.max_size = limit.max_elements;
		}

		template <class Key, class Traits, class Allocator>
		void adjust_algorithm(random_source_base& r_source,
		                      container_size_limit_t const& limit,
		                      value_generation_algorithm<std::set<Key, Traits, Allocator>>& alg_params)
		{
			alg_params.min_size = limit.min_elements;
			alg_params.max_size = limit.max_elements;
		}

		template <class Key, class TValue, class Traits, class Allocator>
		void adjust_algorithm(random_source_base& r_source,
		                      container_size_limit_t const& limit,
		                      value_generation_algorithm<std::map<Key, TValue, Traits, Allocator>>& alg_params)
		{
			alg_params.min_size = limit.min_elements;
			alg_params.max_size = limit.max_elements;
		}
	}
}
#endif //DATAGEN_LIB_STL_LIMITS_DETAILS_HPP
