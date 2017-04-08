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
		dummy(Args ... args) {}
	};

	struct dummy_injector {
		template<class C>
		C create() {
			return C();
		}

		int t{ 0 };
	};

	template<>
	int dummy_injector::create<int>()
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
        direct_ctor_invoker invoker;
        dummy_injector injector;
        auto arr = invoker.create<std::tuple<int, int>>(injector);
        REQUIRE(injector.t == 2);
    }

	SECTION("direct ctor invoker can invoke ctor with no parameters")
    {
		direct_ctor_invoker invoker;
		dummy_injector injector;
		auto arr = invoker.create<dummy<>>(injector);
    }
}
