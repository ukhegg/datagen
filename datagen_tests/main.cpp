//
// Created by ukhegg on 15.03.2017.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <datagen/random_source.hpp>

TEST_CASE("test can link and use datagen library", "[basic usage]"){
    REQUIRE(5 == datagen::get_five());
}

