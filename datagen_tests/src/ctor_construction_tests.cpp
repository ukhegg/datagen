//
// Created by ukhegg on 07.04.2017.
//

#include <catch.hpp>
#include "datagen/internal/ctor_construction.hpp"
#include <tuple>
using namespace datagen::internal;

namespace
{
	template<class ... Args>
	struct dummy {
		explicit dummy(Args ... args) {}
	};

	struct dummy_random_source_t {
		template<class C>
		C create() {
			return C();
		}

		int t{ 0 };
	};

	template<>
	int dummy_random_source_t::create<int>()
	{
		return ++this->t;
	}
}


TEST_CASE("ctor constrcution tests") {
    SECTION("can get ctor parameters count") {
        REQUIRE(ctor_traits<dummy<>>::params_count == 0);
        REQUIRE(ctor_traits<dummy<int>>::params_count == 1);
        REQUIRE((ctor_traits<dummy<int, float>>::params_count == 2));
    }

    SECTION("non constructible class ctor params count is 0") {
        struct non_constructible {
            non_constructible() = delete;
        };
        REQUIRE_FALSE(ctor_traits<non_constructible>::is_constructible);
        REQUIRE(ctor_traits<non_constructible>::params_count == 0);
    }


    SECTION("direct ctor invoker can invoke ctor") {
        direct_ctor_invoker<std::tuple<int, int>> invoker;
        dummy_random_source_t injector;
		datagen::value_generation_algorithm<std::tuple<int, int>> alg;
        auto arr = invoker.create(alg, injector);
        REQUIRE(injector.t == 2);
    }

	SECTION("direct ctor invoker can invoke ctor with no parameters")
    {
		direct_ctor_invoker<dummy<>> invoker;
		datagen::value_generation_algorithm<dummy<>> alg;
		dummy_random_source_t injector;
		auto arr = invoker.create(alg, injector);
    }
}
