//
// Created by ukheg on 07.04.2017.
//

#ifndef DATAGEN_RANDOM_HPP
#define DATAGEN_RANDOM_HPP

#include "datagen/random_source.hpp"
#include "datagen/limits/builtin_limits.hpp"

namespace datagen
{
	/*
	 * value of type TValue can be constructed if one of the following createrias are satisifed:
	 * 1. specialization of class value_generate_algorithm with method TValue get_random(random_source_base) defined
	 * 2. value is constructible via {}-notation where all parameters are constructible
	 * 3. value is constructible vai ctor, where all parameters are constructible
	 *
	 * the first met criteria is used to construct random value
	 */

	//to use your own random source,just specialize this class for int
	template <class TDummy>
	struct random_source_instance
	{
		static random_source_impl instance;
	};

	template <class TDummy>
	random_source_impl random_source_instance<TDummy>::instance;

	template <class TValue>
	TValue random()
	{
		return random_source_instance<int>::instance.create<TValue>();
	}

	template <class TValue, class ... TLimits>
	TValue random(TLimits&&... limits)
	{
		return random_source_instance<int>::instance.create<TValue>(std::forward<TLimits>(limits)...);
	}

	template <class TValue, class ... TLimits>
	void randomize(TValue& value, TLimits&& ... limits)
	{
		random_source_instance<int>::instance.randomize(value, limits...);
	}

	template <class TValue>
	TValue select_random(std::initializer_list<TValue> const& l)
	{
		return random_source_instance<int>::instance.select_random(l);
	}
}
#endif //DATAGEN_RANDOM_HPP
