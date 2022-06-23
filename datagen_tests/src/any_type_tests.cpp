//
// Created by ukhegg on 07.04.2017.
//

#include <catch2/catch.hpp>
#include "datagen/internal/any_type.hpp"

using namespace datagen::internal;

namespace
{
	struct dummy {};

	struct random_src_t
	{
		template <class T>
		T create();

		int i;
	};

	template <>
	int random_src_t::create<int>() { return i; }
}

TEST_CASE("any_type tests")
{
	SECTION("can be converted to arithmetic types")
	{
		using any_type_t = any_type<dummy>;
		REQUIRE((std::is_convertible<any_type_t, int>::value));
		REQUIRE((std::is_convertible<any_type_t, uint32_t>::value));
		REQUIRE((std::is_convertible<any_type_t, float>::value));
		REQUIRE((std::is_convertible<any_type_t, double>::value));
		REQUIRE((std::is_convertible<any_type_t, char>::value));
	}

	SECTION("arithmetic types can be constructed from any_type")
	{
		using any_type_t = any_type<dummy>;
		REQUIRE((std::is_constructible<int, any_type_t>::value));
		REQUIRE((std::is_constructible<uint32_t, any_type_t>::value));
		REQUIRE((std::is_constructible<float, any_type_t>::value));
		REQUIRE((std::is_constructible<double, any_type_t>::value));
		REQUIRE((std::is_constructible<char, any_type_t>::value));
	}

	SECTION("any_type conversion to first template param is forbidden")
	{
		using any_type_t = any_type<int>;
		REQUIRE_FALSE((std::is_convertible<any_type_t, int>::value));
	}

	SECTION("any_type takes value from random source if converting to some type")
	{
		random_src_t inj;
		inj.i = 666;
		using any_type_t = any_type_with_rsrc<dummy, random_src_t>;
		any_type_t att(inj);

		REQUIRE(static_cast<int>(att) == 666);
	}
}
