//
// Created by ukhegg on 13.04.2017.
//

#ifndef CAPTURE_WRITER_LIB_BOOST_PTIME_HPP
#define CAPTURE_WRITER_LIB_BOOST_PTIME_HPP
#include "datagen/random_source_fwd.hpp"
#include <boost/optional.hpp>
#include "datagen/value_generation_algorithm.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include "datagen/limits/builtin_limits.hpp"

namespace datagen
{
	template <>
	struct value_generation_algorithm<boost::posix_time::ptime>
	{
		boost::posix_time::ptime get_random(random_source_base& r_source)
		{
			auto get_random_between = [&r_source](boost::posix_time::ptime from,
						boost::posix_time::ptime to)
					{
						auto dt = to - from;
						auto random_mcs = r_source.create<uint64_t>(limits::between(0, dt.total_microseconds()));
						return from + boost::posix_time::microsec(random_mcs);
					};

			if(this->from != boost::none &&
				this->to != boost::none)
				return get_random_between(*this->from, *this->to);

			auto type = r_source.select_random({
				boost::date_time::not_a_date_time,
				boost::date_time::neg_infin,
				boost::date_time::pos_infin,
				boost::date_time::min_date_time,
				boost::date_time::max_date_time,
				boost::date_time::not_special
			});

			if(type != boost::date_time::not_special) return boost::posix_time::ptime(type);
			return get_random_between(boost::posix_time::ptime(boost::date_time::min_date_time),
			                          boost::posix_time::ptime(boost::date_time::max_date_time));
		}

		boost::optional<boost::posix_time::ptime> from;
		boost::optional<boost::posix_time::ptime> to;
	};

	template <>
	struct value_generation_algorithm<boost::posix_time::time_duration>
	{
		boost::posix_time::time_duration get_random(random_source_base& r_source)
		{
			auto get_random_between = [&r_source](boost::posix_time::time_duration from,
												  boost::posix_time::time_duration to)
					{
						auto dt = to - from;
						auto random_mcs = r_source.create<uint64_t>(limits::between(0, dt.total_milliseconds()));
						return from + boost::posix_time::milliseconds(random_mcs);
					};

			if(this->min_value != boost::none &&
				this->max_value != boost::none)
				return get_random_between(*this->min_value, *this->max_value);

			auto type = r_source.select_random({
				boost::date_time::neg_infin,
				boost::date_time::pos_infin,
				boost::date_time::not_special
			});

			if(type != boost::date_time::not_special) return boost::posix_time::time_duration(type);

			static const auto max_dt = boost::posix_time::ptime(boost::date_time::max_date_time)
									 - boost::posix_time::ptime(boost::date_time::min_date_time);
			return get_random_between(-max_dt, max_dt);
		}

		boost::optional<boost::posix_time::time_duration> min_value;
		boost::optional<boost::posix_time::time_duration> max_value;
	};

	namespace limits
	{
		namespace details
		{
			struct ptime_between_t
			{
				boost::posix_time::ptime from;
				boost::posix_time::ptime to;
			};

			struct time_duration_between_t
			{
				boost::posix_time::time_duration from;
				boost::posix_time::time_duration to;
			};
		}

		inline void adjust_algorithm(random_source_base& r_source,
		                             details::ptime_between_t const& limit,
		                             value_generation_algorithm<boost::posix_time::ptime>& alg)
		{
			alg.from = limit.from;
			alg.to = limit.to;
		}

		inline details::ptime_between_t between(boost::posix_time::ptime const& start,
		                                        boost::posix_time::ptime const& end)
		{
			return details::ptime_between_t{start, end};
		}

		inline void adjust_algorithm(random_source_base& r_source,
		                             details::time_duration_between_t const& limit,
		                             value_generation_algorithm<boost::posix_time::time_duration>& alg)
		{
			alg.min_value = limit.from;
			alg.max_value = limit.to;
		}

		inline details::time_duration_between_t between(boost::posix_time::time_duration const& min,
		                                                boost::posix_time::time_duration const& max)
		{
			return details::time_duration_between_t{min, max};
		}
	}
}
#endif //CAPTURE_WRITER_LIB_BOOST_PTIME_HPP
