//
// Created by ukhegg on 10.04.2017.
//

#include <catch2/catch.hpp>
#include <datagen/random.hpp>

using namespace datagen;
using namespace limits;

TEST_CASE("built-in random limits tests")
{
	random_source_impl r_source;

	GIVEN("between_t<int8_t> limit")
	{
		auto limit = between(10, 20);
		WHEN("applying to value in this range")
		{
			int8_t val1 = 10;
			int8_t val2 = 15;
			int8_t val3 = 20;
			adjust_value(r_source, limit, val1);
			adjust_value(r_source, limit, val2);
			adjust_value(r_source, limit, val3);
			THEN("values remains unchanged")
			{
				REQUIRE(val1 == 10);
				REQUIRE(val2 == 15);
				REQUIRE(val3 == 20);
			}
		}

		WHEN("applying to value bigger than upper bound")
		{
			int8_t val = 21;
			adjust_value(r_source, limit, val);
			THEN("values changes to fit range")
			{
				REQUIRE(val >= limit.lower_bound);
				REQUIRE(val <= limit.upper_bound);
			}
		}

		WHEN("applying to value less than lower bound")
		{
			int8_t val = 9;
			adjust_value(r_source, limit, val);
			THEN("values changes to fit range")
			{
				REQUIRE(val >= limit.lower_bound);
				REQUIRE(val <= limit.upper_bound);
			}
		}
	}

	GIVEN("between_t<float> limit")
	{
		auto limit = between<float>(10, 20);

		WHEN("applying to value in this range")
		{
			float val1 = 10;
			float val2 = 15;
			float val3 = 20;
			adjust_value(r_source, limit, val1);
			adjust_value(r_source, limit, val2);
			adjust_value(r_source, limit, val3);
			THEN("values remains unchanged")
			{
				REQUIRE(val1 == 10);
				REQUIRE(val2 == 15);
				REQUIRE(val3 == 20);
			}
		}

		WHEN("applying to value bigger than upper bound")
		{
			float val = 21;
			adjust_value(r_source, limit, val);
			THEN("values changes to fit range")
			{
				REQUIRE(val >= limit.lower_bound);
				REQUIRE(val <= limit.upper_bound);
			}
		}

		WHEN("applying to value less than lower bound")
		{
			float val = 9;
			adjust_value(r_source, limit, val);
			THEN("values changes to fit range")
			{
				REQUIRE(val >= limit.lower_bound);
				REQUIRE(val <= limit.upper_bound);
			}
		}

		WHEN("calling random with limit")
		{
			auto val = random<float>(limit);
			THEN("obtained value is in range")
			{
				REQUIRE(val >= limit.lower_bound);
				REQUIRE(val <= limit.upper_bound);
			}
		}
	}

	GIVEN("odd limit")
	{
		auto limit = odd<int8_t>();

		WHEN("applying to odd value")
		{
			int8_t val = 1;
			adjust_value(r_source, limit, val);
			THEN("the value does not change")
			{
				REQUIRE(val == 1);
			}
		}

		WHEN("applying to even value")
		{
			int8_t val = 2;
			adjust_value(r_source, limit, val);
			THEN("the value is changed to be odd")
			{
				REQUIRE(val % 2 == 1);
			}
		}

		WHEN("applying to bound odd value")
		{
			int8_t val = std::numeric_limits<int8_t>::max();
			adjust_value(r_source, limit, val);
			THEN("the value does not change")
			{
				REQUIRE(val % 2 == 1);
			}
		}
	}

	GIVEN("even limit")
	{
		auto limit = even<int8_t>();

		WHEN("applying to even value")
		{
			int8_t val = 10;
			adjust_value(r_source, limit, val);
			THEN("the value does not change")
			{
				REQUIRE(val == 10);
			}
		}

		WHEN("applying to odd value")
		{
			int8_t val = 3;
			adjust_value(r_source, limit, val);
			THEN("the value is changed to be even")
			{
				REQUIRE(val % 2 == 0);
			}
		}

		WHEN("applying to bound even value")
		{
			int8_t val = std::numeric_limits<int8_t>::min();
			adjust_value(r_source, limit, val);
			THEN("the value does not change")
			{
				REQUIRE(val % 2 == 0);
			}
		}
	}
}
