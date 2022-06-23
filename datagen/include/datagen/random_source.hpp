//
// Created by ukhegg on 10.04.2017.
//

#ifndef DATAGEN_LIB_DEFAULT_RANDOM_SOURCE_HPP
#define DATAGEN_LIB_DEFAULT_RANDOM_SOURCE_HPP

#include "datagen/random_source_fwd.hpp"
#include "datagen/internal/random_method_selector.hpp"
#include "datagen/limits/random_limits.hpp"
#include <random>
#include "limits/scoped_limit.hpp"

namespace datagen
{
	namespace internal
	{
		template <class TValue, bool IsFundamental>
		struct random_creator
		{
			static TValue create(value_generation_algorithm<TValue>& alg, random_source_base& r_src)
			{
				using random_invoker = typename random_method_selector<TValue>::invoker;
				return random_invoker::create(alg, r_src);
			}
		};

		template <class TValue>
		struct random_creator<TValue, true>
		{
			static TValue create(value_generation_algorithm<TValue>& alg, random_source_base& r_src)
			{
				TValue res;
				r_src.generate_random_sequence((int8_t *)&res, sizeof(TValue));
				return res;
			}
		};

		template <>
		struct random_creator<bool, true>
		{
			static bool create(value_generation_algorithm<bool>& alg, random_source_base& r_src)
			{
				return r_src.create<uint32_t>() > r_src.create<uint32_t>();
			}
		};

		template <>
		struct random_creator<float, true>
		{
			static float create(value_generation_algorithm<float>& alg, random_source_base& r_src)
			{
				auto units = static_cast<float>(r_src.create<int32_t>());
				auto decimal_points = r_src.create<int32_t>() / std::numeric_limits<int32_t>::max();
				return units + decimal_points;
			}
		};

		template <>
		struct random_creator<double, true>
		{
			static double create(value_generation_algorithm<double>& alg, random_source_base& r_src)
			{
				auto units = static_cast<double>(r_src.create<int64_t>());
				auto decimal_points = r_src.create<int64_t>() / std::numeric_limits<int64_t>::max();
				return units + decimal_points;
			}
		};
	}

	template <class TValue, class ... TLimits>
	TValue random_source_base::create(TLimits const&... limits)
	{
		value_generation_algorithm<TValue> alg;

		limits::type_scoped_limits<TValue>::adjust_algorithm(*this, alg);
		limits::apply_algorithm_limits(*this, alg, limits...);

		auto val = internal::random_creator<TValue, std::is_fundamental<TValue>::value>::create(alg, *this);

		limits::type_scoped_limits<TValue>::adjust_value(*this, val);
		limits::apply_value_limits(*this, val, limits...);

		return std::move(val);
	}

	template <class TValue, class ... TLimits>
	void random_source_base::randomize(TValue& value, TLimits const&... limits)
	{
		value = std::move(this->create<TValue>(limits...));
	}

	template <class TValue>
	TValue random_source_base::select_random(std::initializer_list<TValue> const& l)
	{
		auto it = l.begin();
		std::advance(it, this->create<size_t>() % l.size());
		return *it;
	}

	class random_source_impl : public random_source_base
	{
	public:
		virtual ~random_source_impl() = default;

		random_source_impl() {}

		void generate_random_sequence(int8_t* bytes, size_t count) override
		{
			auto blocks_count = count / sizeof(std::random_device::result_type);
			for(auto block = 0 ;block < blocks_count ;++block)
			{
				reinterpret_cast<std::random_device::result_type *>(bytes)[block] = this->r_device_();
			}

			for(auto index = blocks_count * sizeof(std::random_device::result_type) ;index < count ;++index)
			{
				bytes[index] = this->r_device_();
			}
		}

	private:
		std::random_device r_device_;
	};
}
#endif //DATAGEN_LIB_DEFAULT_RANDOM_SOURCE_HPP
