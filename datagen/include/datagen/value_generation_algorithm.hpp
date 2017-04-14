//
// Created by ukhegg on 11.04.2017.
//

#ifndef DATAGEN_LIB_VALUE_GENERATION_ALGORITHM_HPP
#define DATAGEN_LIB_VALUE_GENERATION_ALGORITHM_HPP

namespace datagen
{
	template <class TValue>
	struct value_generation_algorithm {};
}

#define DATAGEN_RANDOM_IMPL(ValueType, Implementation)\
	namespace datagen{\
		template<> struct value_generation_algorithm<ValueType>{\
			ValueType get_random(random_source_base& r_source) Implementation\
		};\
	}

#endif //DATAGEN_LIB_VALUE_GENERATION_ALGORITHM_HPP
