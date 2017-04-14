//
// Created by ukhegg on 13.04.2017.
//

#ifndef CAPTURE_WRITER_LIB_BOOST_OPTIONAL_HPP
#define CAPTURE_WRITER_LIB_BOOST_OPTIONAL_HPP
#include "datagen/random_source_fwd.hpp"
#include <boost/optional.hpp>

namespace datagen
{
	template <class TValue>
	struct value_generation_algorithm<boost::optional<TValue>>
	{
		boost::optional<TValue> get_random(random_source_base& r_source)
		{
			return r_source.create<bool>()
				       ? boost::make_optional(r_source.create<TValue>())
				       : boost::optional<TValue>();
		}
	};
}
#endif //CAPTURE_WRITER_LIB_BOOST_OPTIONAL_HPP
