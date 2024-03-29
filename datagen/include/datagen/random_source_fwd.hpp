//
// Created by ukhegg on 10.04.2017.
//

#ifndef DATAGEN_LIB_RANDOM_SOURCE_FWD_HPP
#define DATAGEN_LIB_RANDOM_SOURCE_FWD_HPP
#include "datagen/value_generation_algorithm.hpp"

namespace datagen
{
	class random_source_base
	{
	public:
		virtual ~random_source_base() = default;

		template <class TValue, class ... TLimits>
		TValue create(TLimits const& ... limits);

		template <class TValue, class ... TLimits>
		void randomize(TValue& value, TLimits const& ... limits);

		template <class TValue>
		TValue select_random(std::initializer_list<TValue> const& l);

		virtual void generate_random_sequence(int8_t* bytes, size_t count) = 0;
	private:
	};
}
#endif //DATAGEN_LIB_RANDOM_SOURCE_FWD_HPP
