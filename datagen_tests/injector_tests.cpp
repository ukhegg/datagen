//
// Created by ukhegg on 25.03.2017.
//

#include <catch.hpp>
#include "datagen/injector.hpp"

using namespace datagen;

TEST_CASE("injector_tests") {
    SECTION("can get default constructed type") {
    struct default_constructed{};

        injector inj;
        auto t = inj.get<default_constructed>();

        REQUIRE(true);
    }
}