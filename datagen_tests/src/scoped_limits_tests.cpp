//
// Created by ukhegg on 14.04.2017.
//

#include <catch.hpp>
#include <datagen/limits/scoped_limit.hpp>
#include <datagen/limits/builtin_limits.hpp>
#include "datagen/random.hpp"
#include <datagen/random_source.hpp>
using namespace datagen;
using namespace limits;

namespace
{
	struct dummy
	{
		dummy(){}
		dummy(bool value_adjusted, bool algorithm_adjusted)
			: value_adjusted(value_adjusted),
			  algorithm_adjusted(algorithm_adjusted) {}

		bool value_adjusted{false};
		bool algorithm_adjusted{false};
	};

	struct dummy_value_limit {};

	struct dummy_algorithm_limit {};
}

namespace datagen
{
	template <>
	struct value_generation_algorithm<dummy>
	{
		dummy get_random(random_source_base&)
		{
			return dummy{false, algorithm_adjusted};
		}

		bool algorithm_adjusted{false};
	};

	namespace limits
	{
		void adjust_algorithm(random_source_base&,
		                      dummy_algorithm_limit const& limit,
		                      value_generation_algorithm<dummy>& alg)
		{
			alg.algorithm_adjusted = true;
		}

		void adjust_value(random_source_base&,
		                  dummy_value_limit const& limit,
		                  dummy& value)
		{
			value.value_adjusted = true;
		}
	}
}

TEST_CASE("scoped limits tests")
{
	random_source_impl r_source;

	SECTION("scoped limits impl tests")
	{
		GIVEN("value & it\'s scoped value limit")
		{
			dummy d;
			REQUIRE_FALSE(d.value_adjusted);
			REQUIRE_FALSE(d.algorithm_adjusted);

			auto limit = dummy_value_limit{};
			REQUIRE((datagen::limits::internal::is_value_limit<dummy, dummy_value_limit>::value));
			REQUIRE_FALSE((datagen::limits::internal::is_algorithm_limit<dummy, dummy_value_limit>::value));

			limits::internal::type_scoped_limit_impl<dummy, dummy_value_limit> scoped(limit);

			WHEN("appling this limit to value")
			{
				scoped.adjust_value(r_source, d);

				THEN("limit is applyed")
				{
					REQUIRE(d.value_adjusted);
				}
			}
			AND_WHEN("applying this limit to algorithm")
			{
				value_generation_algorithm<dummy> alg;
				scoped.adjust_algorithm(r_source, alg);

				THEN("limit is not applyed")
				{
					REQUIRE_FALSE(alg.algorithm_adjusted);
				}
			}
		}

		GIVEN("value & it\'s scoped algorithm limit")
		{
			dummy d;
			REQUIRE_FALSE(d.value_adjusted);
			REQUIRE_FALSE(d.algorithm_adjusted);

			auto limit = dummy_algorithm_limit{};
			REQUIRE_FALSE((datagen::limits::internal::is_value_limit<dummy, decltype(limit)>::value));
			REQUIRE((datagen::limits::internal::is_algorithm_limit<dummy, decltype(limit)>::value));

			limits::internal::type_scoped_limit_impl<dummy, decltype(limit)> scoped(limit);

			WHEN("appling this limit to value")
			{
				scoped.adjust_value(r_source, d);

				THEN("limit is not applyed")
				{
					REQUIRE_FALSE(d.value_adjusted);
				}
			}
			AND_WHEN("applying this limit to algorithm")
			{
				value_generation_algorithm<dummy> alg;
				scoped.adjust_algorithm(r_source, alg);

				THEN("algorithm is adjusted")
				{
					REQUIRE(alg.algorithm_adjusted);
				}
			}
		}

		GIVEN("some value\'s limit")
		{
			dummy_algorithm_limit l;
			WHEN("scoply applying this limit to value")
			{
				auto g = scoped_limit(l).apply_to_type<dummy>();

				THEN("it is registered in value\'s global list of scoped limits")
				{
					REQUIRE((type_scoped_limits<dummy>::registered_limits().size() == 1));
				}
			}
			AND_WHEN("limit guard is destroyed")
			{
				auto g = scoped_limit(l).apply_to_type<dummy>();
				g.reset();
				THEN("type\'s limit is unregistered")
				{
					REQUIRE((type_scoped_limits<dummy>::registered_limits().empty()));
				}
			}
		}
	}

	SECTION("scoped limits normal usage tests")
	{
		GIVEN("type scoped limit for type algorithm")
		{
			auto g = scoped_limit(dummy_algorithm_limit{}).apply_to_type<dummy>();
			WHEN("constructing this type")
			{
				auto d = r_source.create<dummy>();
				THEN("this limit takes effect")
				{
					REQUIRE(d.algorithm_adjusted);
					REQUIRE_FALSE(d.value_adjusted);
				}
			}
		}

		GIVEN("type scoped limit for type value")
		{
			auto g = scoped_limit(dummy_value_limit{}).apply_to_type<dummy>();
			WHEN("constructing this type")
			{
				auto d = r_source.create<dummy>();
				THEN("this limit takes effect")
				{
					REQUIRE_FALSE(d.algorithm_adjusted);
					REQUIRE(d.value_adjusted);
				}
			}
		}
	}
}
