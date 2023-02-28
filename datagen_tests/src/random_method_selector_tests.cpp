//
// Created by ukhegg on 07.04.2017.
//

#include <catch2/catch_all.hpp>

#include "datagen/internal/random_method_selector.hpp"

namespace
{
	struct dummy2
	{
		int val;
	};
}

namespace datagen
{
	template <>
	struct value_generation_algorithm<dummy2>
	{
		dummy2 get_random(random_source_base&)
		{
			return dummy2{666};
		}
	};
}

TEST_CASE("random_method_selector tests")
{
	SECTION("check braces ctor for braces constructible class")
	{
		struct braces_constructible
		{
			int a{0};
		};

		using braces_ctor_t = datagen::internal::random_method_selector<braces_constructible>::braces_invoker;

		using expected_ctor_t = datagen::internal::braces_initializer_invoker<braces_constructible>;

		REQUIRE( (std::is_same<expected_ctor_t, braces_ctor_t>::value) );
	}

	SECTION("check braces ctor for braces non-constructible class")
	{
		struct braces_non_constructible
		{
		private:
			int a{0};
			braces_non_constructible() {}
		};

		using braces_ctor_t = datagen::internal::random_method_selector<braces_non_constructible>::braces_invoker;

		using expected_ctor_t = datagen::internal::none_t;

		REQUIRE((std::is_same<expected_ctor_t, braces_ctor_t>::value));
	}

	SECTION("ctor_invoker is direct_ctor_invoker for ctor constructible class")
	{
		struct ctor_constructible
		{
			ctor_constructible(int) {}
		};

		using direct_ctor_t = datagen::internal::random_method_selector<ctor_constructible>::ctor_invoker;
		using expected_ctor_t = datagen::internal::direct_ctor_invoker<ctor_constructible>;
		REQUIRE((std::is_same<direct_ctor_t, expected_ctor_t>::value));
	}

	SECTION("ctor_invoker is none_t for ctor non-constructible class")
	{
		struct ctor_non_constructible
		{
		private:
			ctor_non_constructible() {}
		};

		using direct_ctor_t = datagen::internal::random_method_selector<ctor_non_constructible>::ctor_invoker;
		using expected_ctor_t = datagen::internal::none_t;
		REQUIRE((std::is_same<direct_ctor_t, expected_ctor_t>::value));
	}

	SECTION("direct_invoker is direct_get_random_invoker for class with defined random function")
	{
		using invoker_t = datagen::internal::random_method_selector<dummy2>::direct_random_invoker;
		REQUIRE(datagen::internal::has_random_generation_algorithm<dummy2>::value);
		REQUIRE((std::is_same<invoker_t, datagen::internal::direct_get_random_invoker<dummy2>>::value));
	}

	SECTION("direct_invoker is none_t for class with no random function defined")
	{
		struct temp {};
		REQUIRE_FALSE(datagen::internal::has_random_generation_algorithm<temp>::value);

		using invoker_t = datagen::internal::random_method_selector<temp>::direct_random_invoker;
		REQUIRE((std::is_same<invoker_t, datagen::internal::none_t>::value));
	}

	SECTION("test select_first_not_none struct")
	{
		REQUIRE((std::is_same<int, datagen::internal::select_first_not_none<int>::type>::value));
		REQUIRE((std::is_same<int, datagen::internal::select_first_not_none<datagen::internal::none_t, int>::type>::value));
		REQUIRE((std::is_same<int, datagen::internal::select_first_not_none<datagen::internal::none_t, int, datagen::internal::none_t>::type>::value));
		REQUIRE((std::is_same<int, datagen::internal::select_first_not_none<datagen::internal::none_t, int, float>::type>::value));
		REQUIRE((std::is_same<datagen::internal::none_t, datagen::internal::select_first_not_none<datagen::internal::none_t>::type>::value));
	}
}
