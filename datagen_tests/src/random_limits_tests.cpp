//
// Created by ukhegg on 11.04.2017.
//

#include <catch2/catch.hpp>
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
    template<>
    struct value_generation_algorithm<dummy>
    {
        dummy get_random(random_source_base &r_source)
        {
            return dummy{this->dummy_default_value_};
        }

        int dummy_default_value_{5};

        std::list<int> applied_defaults;
    };
}

namespace datagen::limits
{
    struct default_value_limit
    {
    };

    struct dummy_value_limit
    {
    };

    struct custom_alg_limit
    {
    };

    struct custom_val_limit
    {
    };

    struct uni_limit
    {
    };

    struct adjust_dummy_alg_value
    {
        explicit adjust_dummy_alg_value(int val) : val(val)
        {
        }

        int val{0};
    };

    struct adjust_dummy_inst_value
    {
        explicit adjust_dummy_inst_value(int val) : val(val)
        {
        }

        int val{0};
    };

    void adjust_algorithm(random_source_base &, default_value_limit const &l, value_generation_algorithm<dummy> &a)
    {
        a.dummy_default_value_ = 6;
    }

    void adjust_value(random_source_base &, dummy_value_limit const &l, dummy &a)
    {
        a.val = 666;
    }

    void adjust_algorithm(random_source_base &, custom_alg_limit const &, value_generation_algorithm<dummy> &)
    {
    }

    void adjust_value(random_source_base &, custom_val_limit const &, dummy &)
    {
    }

    void adjust_algorithm(random_source_base &, uni_limit const &, value_generation_algorithm<dummy> &)
    {
    }

    void adjust_value(random_source_base &, uni_limit const &, dummy &)
    {
    }

    void adjust_algorithm(random_source_base &, adjust_dummy_alg_value const &l, value_generation_algorithm<dummy> &alg)
    {
        alg.applied_defaults.push_back(l.val);
    }

    void adjust_value(random_source_base &, adjust_dummy_inst_value const &l, dummy &val)
    {
        val.values.push_back(l.val);
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
        REQUIRE((datagen::limits::internal::is_algorithm_limit_v<dummy, datagen::limits::custom_alg_limit>));
    }

    SECTION("if struct has apply method defined with proper parameter,than it becomes a value limit")
    {
        REQUIRE((datagen::limits::internal::is_value_limit_v<dummy, datagen::limits::custom_val_limit>));
    }

    SECTION("struct can be both value and alg limit")
    {
        REQUIRE((datagen::limits::internal::is_algorithm_limit_v<dummy, datagen::limits::uni_limit>));
        REQUIRE((datagen::limits::internal::is_value_limit_v<dummy, datagen::limits::uni_limit>));
    }

    SECTION("if constructed with no limits,default alg is used")
    {
        auto d = datagen::random<dummy>();
        REQUIRE(d.val == 5);
    }

    SECTION("if constructed with alg limits,default alg is not used")
    {
        auto d = datagen::random<dummy>(datagen::limits::default_value_limit());
        REQUIRE(d.val == 6);
    }

    SECTION("if constructed with value limits, they are applyied")
    {
        auto d = datagen::random<dummy>(datagen::limits::dummy_value_limit());
        REQUIRE(d.val == 666);
    }

    SECTION("can enum and apply single algorithm limit")
    {
        datagen::random_source_impl r_source;

        datagen::value_generation_algorithm<dummy> alg;
        datagen::limits::apply_algorithm_limits(r_source, alg, datagen::limits::adjust_dummy_alg_value{1});

        REQUIRE(alg.applied_defaults.size() == 1);
        REQUIRE(alg.applied_defaults.front() == 1);
    }

    SECTION("algorithm_tuner applies given algorithm limit on algorithm")
    {
        datagen::random_source_impl r_source;
        datagen::value_generation_algorithm<dummy> alg;
        datagen::limits::adjust_dummy_alg_value limit{3};
        datagen::limits::limits_applicator::do_adjust_algorithm(r_source, limit, alg);
        REQUIRE((alg.applied_defaults == std::list<int>{3}));
    }

    SECTION("algorithm_tuner does not apply given value limit on algorithm")
    {
        datagen::random_source_impl r_source;
        datagen::value_generation_algorithm<dummy> alg;
        datagen::limits::adjust_dummy_inst_value limit{3};
        datagen::limits::limits_applicator::do_adjust_algorithm(r_source, limit, alg);
        REQUIRE(alg.applied_defaults.empty());
    }

    SECTION("can enum and apply several algorithm limits")
    {
        datagen::random_source_impl r_source;

        datagen::value_generation_algorithm<dummy> alg;
        datagen::limits::apply_algorithm_limits(r_source,
                                                alg,
                                                datagen::limits::adjust_dummy_alg_value{1},
                                                datagen::limits::adjust_dummy_alg_value{2},
                                                datagen::limits::adjust_dummy_alg_value{3});

        REQUIRE(alg.applied_defaults.size() == 3);
        REQUIRE((alg.applied_defaults == std::list<int>{1, 2, 3}));
    }

    SECTION("can enum limits of mized types and apply algorithm limits")
    {
        datagen::random_source_impl r_source;

        datagen::value_generation_algorithm<dummy> alg;
        datagen::limits::apply_algorithm_limits(r_source,
                                                alg,
                                                datagen::limits::adjust_dummy_alg_value{1},
                                                datagen::limits::adjust_dummy_inst_value{2},
                                                datagen::limits::adjust_dummy_alg_value{3});

        REQUIRE(alg.applied_defaults.size() == 2);
        REQUIRE((alg.applied_defaults == std::list<int>{1, 3}));
    }
}
