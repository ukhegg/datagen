//
// Created by ukheg on 07.04.2017.
//
#include <catch2/catch.hpp>
#include <datagen/random_source.hpp>
#include "datagen/internal/explicit_random_invoker.hpp"

namespace
{
	struct dummy
	{
		int value;
	};

	struct has_no_explicit_random {};
}

namespace datagen
{
	template <>
	struct value_generation_algorithm<dummy>
	{
		dummy get_random(random_source_base&)
		{
			return dummy{666};
		}
	};
}

TEST_CASE("explicit random invoker tests")
{
	SECTION("can check for explicit_random existance")
	{
		REQUIRE(datagen::internal::has_random_generation_algorithm<dummy>::value);
	}

	SECTION("can check for explicit_random non-existance")
	{
		REQUIRE_FALSE(datagen::internal::has_random_generation_algorithm<has_no_explicit_random>::value);
	}

	SECTION("can use direct random invoker to construct value")
	{
		datagen::random_source_impl random_source;
		datagen::value_generation_algorithm<dummy> alg;
		auto r = datagen::internal::direct_get_random_invoker<dummy>::create(alg, random_source);
		REQUIRE(r.value == 666);
	}
}
