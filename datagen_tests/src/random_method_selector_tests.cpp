//
// Created by ukhegg on 07.04.2017.
//

#include <catch.hpp>

#include "datagen/internal/random_method_selector.hpp"
using namespace datagen::internal;

namespace
{
	struct dummy { int val; };
}

namespace datagen
{
	template<class TInjector>
	dummy get_random(TInjector& inj, dummy*)
	{
		return {5};
	}
}


TEST_CASE("random_method_selector tests") {
    SECTION("check braces ctor for braces constructible class"){

		struct braces_constructible {
			int a{ 0 };
		};

        using braces_ctor_t = random_method_selector<braces_constructible>::braces_ctor_invoker ;

		using expected_ctor_t = braces_initializer_invoker;

        REQUIRE( (std::is_same<expected_ctor_t, braces_ctor_t>::value) );
    }

	SECTION("check braces ctor for braces non-constructible class") {
		struct braces_non_constructible {

		private:
			int a{ 0 };
			braces_non_constructible();
		};

		using braces_ctor_t = random_method_selector<braces_non_constructible>::braces_ctor_invoker;

		using expected_ctor_t = none_t;

		REQUIRE((std::is_same<expected_ctor_t, braces_ctor_t>::value));
	}

	SECTION("ctor_invoker is direct_ctor_invoker for ctor constructible class")
    {
		struct ctor_constructible {
			ctor_constructible(int) {}
		};

		using direct_ctor_t = random_method_selector<ctor_constructible>::direct_ctor_invoker;
		using expected_ctor_t = direct_ctor_invoker;
		REQUIRE((std::is_same<direct_ctor_t, expected_ctor_t>::value));
    }

	SECTION("ctor_invoker is none_t for ctor non-constructible class")
	{
		struct ctor_non_constructible {
		private:
			ctor_non_constructible() {}
		};

		using direct_ctor_t = random_method_selector<ctor_non_constructible>::direct_ctor_invoker;
		using expected_ctor_t = none_t;
		REQUIRE((std::is_same<direct_ctor_t, expected_ctor_t>::value));
	}

	SECTION("direct_invoker is direct_get_random_invoker for class with defined random function")
    {
		using invoker_t = random_method_selector<dummy>::direct_random_invoker;
		REQUIRE(has_explicit_random<dummy>::value);
		REQUIRE((std::is_same<invoker_t, direct_get_random_invoker>::value));
    }

	SECTION("direct_invoker is none_t for class with no random function defined")
	{
		struct temp{};
		REQUIRE_FALSE(has_explicit_random<temp>::value);

		using invoker_t = random_method_selector<temp>::direct_random_invoker;
		REQUIRE((std::is_same<invoker_t, none_t>::value));
	}

	SECTION("test select_first_not_none struct")
    {
		REQUIRE((std::is_same<int, select_first_not_none<int>::type>::value));
		REQUIRE((std::is_same<int, select_first_not_none<none_t, int>::type>::value));
		REQUIRE((std::is_same<int, select_first_not_none<none_t, int, none_t>::type>::value));
		REQUIRE((std::is_same<int, select_first_not_none<none_t, int, float>::type>::value));
		REQUIRE((std::is_same<none_t, select_first_not_none<none_t>::type>::value));
    }
}
