//
// Created by ukheg on 07.04.2017.
//
#include <catch.hpp>
#include "datagen/internal/explicit_random_invoker.hpp"

using namespace datagen::internal;

namespace {
	struct dummy_injector{};

    struct dummy {
        int value;
    };

	struct has_no_explicit_random {};
}

namespace datagen {
	template<class TInjector>
	dummy get_random(TInjector&, dummy*)
	{
		return dummy{ 666 };
	}
}

TEST_CASE("explicit random invoker tests"){
    SECTION("can check for explicit_random existance"){
        REQUIRE(has_explicit_random<dummy>::value);
    }

	SECTION("can check for explicit_random non-existance") {
		REQUIRE_FALSE(has_explicit_random<has_no_explicit_random>::value);
	}

	SECTION("can use direct random invoker to construct value") {
		dummy_injector injector;
		auto r = direct_get_random_invoker::create<dummy>(injector);
		REQUIRE(r.value == 666);
	}
}