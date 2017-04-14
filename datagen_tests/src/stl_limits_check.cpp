//
// Created by ukhegg on 12.04.2017.
//
#include <catch.hpp>
#include <datagen/stl_random.hpp>
#include <datagen/limits/stl_limits.hpp>

using namespace datagen;
using namespace limits;

TEST_CASE("stl limits check")
{
	SECTION("std::string limits tests")
	{
		SECTION("alphabet limit is algorithm limit")
		{
			using t = details::alphabet_contains_limit_t<char>;
			REQUIRE((datagen::limits::internal::is_algorithm_limit<std::string, t>::value));
		}
		SECTION("can apply alphabet limit")
		{
			auto str = random<std::string>(alphabet::consists_of("abc"));

			for(auto c : str)
			{
				REQUIRE((c == 'a' || c == 'b' || c == 'c'));
			}
		}
		SECTION("can apply alphabet does not contain limit")
		{
			auto std_alphabet = value_generation_algorithm<std::string>().alphabet;
			auto last_letter = std_alphabet.back();
			std_alphabet.pop_back();
			auto str = random<std::string>(alphabet::does_not_contain(std_alphabet));

			for(auto c : str)
			{
				REQUIRE(c == last_letter);
			}
		}
		SECTION("elements count is algorithm limit")
		{
			using t = details::container_size_limit_t;
			REQUIRE((datagen::limits::internal::is_algorithm_limit<std::string, t>::value));
		}
		SECTION("can apply elements count limit")
		{
			auto limit = details::container_size_limit_t(42, 42);
			auto str = random<std::string>(limit);
			REQUIRE(str.size() == 42);
		}
	}

	SECTION("vector limits tests")
	{
		SECTION("elements count is algorithm limit")
		{
			using t = details::container_size_limit_t;
			REQUIRE((datagen::limits::internal::is_algorithm_limit<std::vector<int>, t>::value));
		}
		SECTION("can apply container size between limit")
		{
			auto str = random<std::vector<int>>(container_size::between(42, 42));
			REQUIRE(str.size() == 42);
		}
		SECTION("can apply container size not bigger than limit")
		{
			auto str = random<std::vector<int>>(container_size::not_bigger_than(1));
			REQUIRE((str.size() == 0 || str.size() == 1));
		}
	}

	SECTION("std::set limits tests")
	{
		SECTION("elements count is algorithm limit")
		{
			using t = details::container_size_limit_t;
			REQUIRE((datagen::limits::internal::is_algorithm_limit<std::set<int>, t>::value));
		}
		SECTION("can apply container size between limit")
		{
			auto str = random<std::set<int>>(container_size::between(42, 42));
			REQUIRE(str.size() == 42);
		}
	}

	SECTION("std::map limits tests")
	{
		SECTION("elements count is algorithm limit")
		{
			using t = details::container_size_limit_t;
			REQUIRE((datagen::limits::internal::is_algorithm_limit<std::map<int, float>, t>::value));
		}
		SECTION("can apply container size between limit")
		{
			auto str = random<std::map<int, float>>(container_size::between(42, 42));
			REQUIRE(str.size() == 42);
		}
	}
}
