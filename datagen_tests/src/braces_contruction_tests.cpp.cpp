//
// Created by ukhegg on 07.04.2017.
//

#include <catch.hpp>
#include "datagen/internal/braces_construction.hpp"

using namespace datagen::internal;

namespace
{
	struct injector_t
	{
		template<class C>
		C create() { return C(); }

		int int_value{ 0 };
		float float_value{ 0 };
	};

	template<>
	int injector_t::create<int>() { return int_value; }

	template<>
	float injector_t::create<float>() { return float_value; }
}


TEST_CASE("braces constructible tests")
{
	injector_t injector;



	SECTION("can construct simple object")
	{
		injector.int_value = 1;
		injector.float_value = 0.5;

		using simple_t = std::pair<int, float>;
		auto t = braces_initializer_invoker::create<simple_t>(injector);

		REQUIRE(t.first == 1);
		REQUIRE(t.second == 0.5);
	}

	SECTION("can construct object with no fields")
	{
		struct void_object {};

		void_object v = {};

		auto t = braces_initializer_invoker::create<void_object>(injector);
	}
}