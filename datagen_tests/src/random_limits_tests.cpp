//
// Created by ukhegg on 11.04.2017.
//

#include <catch.hpp>
#include <datagen/random.hpp>
#include <list>
#include "datagen/random_algorithms/std_array.hpp"

namespace
{
	struct dummy
	{
		int val;

		std::list<int> values;
	};
}

namespace datagen
{
	template <>
	struct value_generation_algorithm<dummy>
	{
		dummy get_random(random_source_base& r_source)
		{
			return dummy{this->dummy_default_value_};
		}

		int dummy_default_value_{5};

		std::list<int> applied_defaults;
	};
}

namespace
{
	struct default_value_limit { };

	struct dummy_value_limit { };

	struct custom_alg_limit { };

	struct custom_val_limit { };

	struct uni_limit { };

	struct adjust_dummy_alg_value
	{
		explicit adjust_dummy_alg_value(int val) : val(val) { }

		int val{0};
	};

	struct adjust_dummy_inst_value
	{
		explicit adjust_dummy_inst_value(int val) : val(val) { }

		int val{0};
	};
}

namespace datagen
{
	namespace limits
	{
		void adjust_algorithm(random_source_base&, default_value_limit const& l, value_generation_algorithm<dummy>& a)
		{
			a.dummy_default_value_ = 6;
		}

		void adjust_value(random_source_base&, dummy_value_limit const& l, dummy& a)
		{
			a.val = 666;
		}

		void adjust_algorithm(random_source_base&, custom_alg_limit const&, value_generation_algorithm<dummy>&) { }

		void adjust_value(random_source_base&, custom_val_limit const&, dummy&) { }

		void adjust_algorithm(random_source_base&, uni_limit const&, value_generation_algorithm<dummy>&) { }

		void adjust_value(random_source_base&, uni_limit const&, dummy&) { }

		void adjust_algorithm(random_source_base&, adjust_dummy_alg_value const& l, value_generation_algorithm<dummy>& alg)
		{
			alg.applied_defaults.push_back(l.val);
		}

		void adjust_value(random_source_base&, adjust_dummy_inst_value const& l, dummy& val)
		{
			val.values.push_back(l.val);
		}
	}
}

TEST_CASE("random limits tests")
{
	SECTION("check dummy is correct")
	{
		REQUIRE(datagen::internal::has_random_generation_algorithm<dummy>::value);
	}

	SECTION("if struct has apply method defined with proper parameter,than it becomes an algorithm limit")
	{
		REQUIRE((datagen::limits::internal::is_algorithm_limit<dummy, custom_alg_limit>::value));
	}

	SECTION("if struct has apply method defined with proper parameter,than it becomes a value limit")
	{
		REQUIRE((datagen::limits::internal::is_value_limit<dummy, custom_val_limit>::value));
	}

	SECTION("struct can be both value and alg limit")
	{
		REQUIRE((datagen::limits::internal::is_algorithm_limit<dummy, uni_limit>::value));
		REQUIRE((datagen::limits::internal::is_value_limit<dummy, uni_limit>::value));
	}

	SECTION("if constructed with no limits,default alg is used")
	{
		auto d = datagen::random<dummy>();
		REQUIRE(d.val == 5);
	}

	SECTION("if constructed with alg limits,default alg is not used")
	{
		auto d = datagen::random<dummy>(default_value_limit());
		REQUIRE(d.val == 6);
	}

	SECTION("if constructed with value limits, they are applyied")
	{
		auto d = datagen::random<dummy>(dummy_value_limit());
		REQUIRE(d.val == 666);
	}

	SECTION("can enum and apply single algorithm limit")
	{
		datagen::random_source_impl r_source;

		datagen::value_generation_algorithm<dummy> alg;
		datagen::limits::apply_algorithm_limits(r_source, alg, adjust_dummy_alg_value{1});

		REQUIRE(alg.applied_defaults.size() == 1);
		REQUIRE(alg.applied_defaults.front() == 1);
	}

	SECTION("algorithm_tuner applies given algorithm limit on algorithm")
	{
		using alg_tuner = datagen::limits::internal::algorithm_tuner<dummy,
		                                                             adjust_dummy_alg_value,
		                                                             true>;

		datagen::random_source_impl r_source;
		datagen::value_generation_algorithm<dummy> alg;
		adjust_dummy_alg_value limit{3};
		alg_tuner::apply(r_source, limit, alg);
		REQUIRE((alg.applied_defaults == std::list<int>{3}));
	}

	SECTION("algorithm_tuner does not apply given limit on algorithm")
	{
		using alg_tuner = datagen::limits::internal::algorithm_tuner<dummy,
		                                                             adjust_dummy_alg_value,
		                                                             false>;

		datagen::random_source_impl r_source;
		datagen::value_generation_algorithm<dummy> alg;
		adjust_dummy_alg_value limit{3};
		alg_tuner::apply(r_source, limit, alg);
		REQUIRE(alg.applied_defaults.empty());
	}

	SECTION("can enum and apply several algorithm limits")
	{
		datagen::random_source_impl r_source;

		datagen::value_generation_algorithm<dummy> alg;
		datagen::limits::apply_algorithm_limits(r_source,
		                                        alg,
		                                        adjust_dummy_alg_value{1},
		                                        adjust_dummy_alg_value{2},
		                                        adjust_dummy_alg_value{3});

		REQUIRE(alg.applied_defaults.size() == 3);
		REQUIRE((alg.applied_defaults == std::list<int>{ 1,2,3 }));
	}

	SECTION("can enum limits of mized types and apply algorithm limits")
	{
		datagen::random_source_impl r_source;

		datagen::value_generation_algorithm<dummy> alg;
		datagen::limits::apply_algorithm_limits(r_source,
		                                        alg,
		                                        adjust_dummy_alg_value{1},
		                                        adjust_dummy_inst_value{2},
		                                        adjust_dummy_alg_value{3});

		REQUIRE(alg.applied_defaults.size() == 2);
		REQUIRE((alg.applied_defaults == std::list<int>{ 1, 3 }));
	}
}
