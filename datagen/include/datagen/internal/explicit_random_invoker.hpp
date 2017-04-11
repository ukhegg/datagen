//
// Created by ukheg on 07.04.2017.
//

#ifndef DATAGEN_EXPLICIT_RANDOM_INVOKER_HPP
#define DATAGEN_EXPLICIT_RANDOM_INVOKER_HPP

#include <type_traits>
#include "datagen/random_source_fwd.hpp"
#include "datagen/value_generation_algorithm.hpp"
namespace datagen {
    namespace internal {
        using namespace datagen;

		template<class T>
		struct has_random_generation_algorithm
		{
		private:
			static std::false_type test(...) {};

			template<class C>
			static value_generation_algorithm<C>& get_alg() { return std::declval<C>(); }

			template<class C>
			static typename std::enable_if<
				std::is_same<T, decltype(get_alg<C>().get_random(std::declval<random_source_base&>()))>::value,
				std::true_type>::type test(C);
		public:
			enum { value = std::is_same<std::true_type, decltype(test(std::declval<T>()))>::value };
		};

        template<class TValue>
        struct direct_get_random_invoker {
			using value_generation_algorithm_t = value_generation_algorithm<TValue>;

            static TValue create(value_generation_algorithm_t& alg, random_source_base &random_source) {
				return std::move(alg.get_random(random_source));
            }
        };
    }
}
#endif //DATAGEN_EXPLICIT_RANDOM_INVOKER_HPP
