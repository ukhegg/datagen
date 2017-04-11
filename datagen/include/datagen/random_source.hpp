//
// Created by ukhegg on 10.04.2017.
//

#ifndef DATAGEN_LIB_DEFAULT_RANDOM_SOURCE_HPP
#define DATAGEN_LIB_DEFAULT_RANDOM_SOURCE_HPP

#include "datagen/random_source_fwd.hpp"
#include "datagen/internal/random_method_selector.hpp"
#include "datagen/limits/random_limits.hpp"
#include <random>

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
			template <class TRandomSource>
			static TValue create(value_generation_algorithm<TValue>& alg, TRandomSource& r_src)
			{
				TValue res;
				r_src.generate_random_sequence((int8_t *)&res, sizeof(TValue));
				return res;
			}
		};
	}

	template <class TValue>
	TValue random_source_base::create()
	{
		value_generation_algorithm<TValue> alg;
		return internal::random_creator<TValue, std::is_fundamental<TValue>::value>::create(alg, *this);
	}

	template <class TValue, class ... TLimits>
	TValue random_source_base::create(TLimits&&... limits)
	{
		value_generation_algorithm<TValue> alg;
		limits::apply_algorithm_limits(*this, alg, std::forward<TLimits>(limits)...);
		auto val = internal::random_creator<TValue, std::is_fundamental<TValue>::value>::create(alg, *this);
		limits::apply_value_limits(*this, val, std::forward<TLimits>(limits)...);
		return std::move(val);
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
