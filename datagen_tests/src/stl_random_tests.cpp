//
// Created by ukhegg on 10.04.2017.
//

#include <catch.hpp>
#include <datagen/stl_random.hpp>
#include <datagen/random.hpp>
#include "datagen/limits/stl_limits.hpp"

TEST_CASE("stl classes random generation tests")
{
	struct dummy
	{
		int a;
		int b;
		float c;

		bool operator<(dummy const& rhs) const
		{
			if(this->a < rhs.a) return true;
			if(rhs.a < this->a) return false;
			if(this->b < rhs.b) return true;
			if(rhs.b < this->b) return false;
			return this->c < rhs.c;
		}
	};

	SECTION("string tests")
	{
		SECTION("string has explicit_random defined")
		{
			REQUIRE((datagen::internal::has_random_generation_algorithm<std::string>::value));
		}

		SECTION("generated string are different")
		{
			size_t string_to_generate = 1024;
			std::map<std::string, size_t> counts;
			for(size_t i = 0 ;i < string_to_generate ;++i)
			{
				auto s = datagen::random<std::string>();
				counts[s]++;
			}

			REQUIRE(counts.size() > string_to_generate * 0.9);
		}
	}

	SECTION("vector tests")
	{
		SECTION("vector has explicit random defined")
		{
			REQUIRE((datagen::internal::has_random_generation_algorithm<std::vector<int>>::value));
		}

		SECTION("can generate vector of simple types")
		{
			std::vector<int> generated;
			for(int i = 0 ;i < 10 ;++i)
			{
				generated = datagen::random<decltype(generated)>();
				if(!generated.empty()) break;
			}
			REQUIRE(generated.size() != 0);
		}

		SECTION("can generate vector of custom types types")
		{
			std::vector<dummy> generated;
			for(int i = 0 ;i < 10 ;++i)
			{
				generated = datagen::random<decltype(generated)>();
				if(!generated.empty()) break;
			}
			REQUIRE(generated.size() != 0);
		}
	}

	SECTION("set tests")
	{
		SECTION("set has explicit random defined")
		{
			REQUIRE((datagen::internal::has_random_generation_algorithm<std::set<int>>::value));
		}

		SECTION("can generate set of simple types")
		{
			std::set<int> generated;
			for(int i = 0 ;i < 10 ;++i)
			{
				generated = datagen::random<decltype(generated)>();
				if(!generated.empty()) break;
			}
			REQUIRE(generated.size() != 0);
		}

		SECTION("can generate set of custom types types")
		{
			std::set<dummy> generated;
			for(int i = 0 ;i < 10 ;++i)
			{
				generated = datagen::random<decltype(generated)>();
				if(!generated.empty()) break;
			}
			REQUIRE(generated.size() != 0);
		}
	}

	SECTION("map tests")
	{
		SECTION("map has explicit random defined")
		{
			REQUIRE((datagen::internal::has_random_generation_algorithm<std::map<int, float>>::value));
		}

		SECTION("can generate map of simple types")
		{
			std::map<int, int> generated;
			for(int i = 0 ;i < 10 ;++i)
			{
				generated = datagen::random<decltype(generated)>();
				if(!generated.empty()) break;
			}
			REQUIRE(generated.size() != 0);
		}

		SECTION("can generate map of custom types types")
		{
			std::map<dummy, dummy> generated;
			for(int i = 0 ;i < 10 ;++i)
			{
				generated = datagen::random<std::map<dummy, dummy>>();
				if(!generated.empty()) break;
			}
			REQUIRE(generated.size() != 0);
		}
	}

	SECTION("array tests")
	{
		SECTION("array has explicit random defined")
		{
			REQUIRE((datagen::internal::has_random_generation_algorithm<std::array<int, 666>>::value));
		}

		SECTION("can generate array of simple types")
		{
			auto v = datagen::random<std::array<int, 666>>();
			std::map<int, size_t> distribution;
			for(auto i : v) distribution[i]++;
			REQUIRE(distribution.size() > 666 * 0.9);
		}

		SECTION("can generate array of custom types")
		{
			auto v = datagen::random<std::array<dummy, 666>>();
			std::map<dummy, size_t> distribution;
			for(auto i : v) distribution[i]++;
			REQUIRE(distribution.size() > 666 * 0.9);
		}
	}
}
