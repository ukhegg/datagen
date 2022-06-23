//
// Created by ukhegg on 07.04.2017.
//

#include <catch2/catch.hpp>
#include "datagen/internal/braces_construction.hpp"

using namespace datagen::internal;

namespace
{
	struct random_src_t
	{
		template <class C>
		C create() { return C(); }

		int int_value{0};
		float float_value{0};
	};

	template <>
	int random_src_t::create<int>() { return int_value; }

	template <>
	float random_src_t::create<float>() { return float_value; }
}

TEST_CASE("braces constructible tests")
{
	random_src_t injector;

	SECTION("can construct simple object")
	{
		injector.int_value = 1;
		injector.float_value = 0.5;

		using simple_t = std::pair<int, float>;
		datagen::value_generation_algorithm<simple_t> alg;

		auto t = braces_initializer_invoker<simple_t>::create(alg, injector);

		REQUIRE(t.first == 1);
		REQUIRE(t.second == 0.5);
	}

	SECTION("can construct object with no fields")
	{
		struct void_object {};

		void_object v = {};
		datagen::value_generation_algorithm<void_object> alg;

		auto t = braces_initializer_invoker<void_object>::create(alg, injector);
	}
}
