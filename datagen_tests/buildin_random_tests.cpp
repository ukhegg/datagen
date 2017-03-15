//
// Created by ukhegg on 15.03.2017.
//

#include "catch.hpp"
#include "datagen/buildin.hpp"

TEST_CASE("build in types random tests") {
    SECTION("can get random build in types") {
        SECTION("test int8_t") {
            auto n1 = datagen::random<int8_t>();
            auto n2 = datagen::random<int8_t>();
            REQUIRE(n1 != n2);
        }

        SECTION("test uint8_t") {
            auto n1 = datagen::random<uint8_t>();
            auto n2 = datagen::random<uint8_t>();
            REQUIRE(n1 != n2);
        }

        SECTION("test int16_t") {
            auto n1 = datagen::random<int16_t>();
            auto n2 = datagen::random<int16_t>();
            REQUIRE(n1 != n2);
        }

        SECTION("test uint16_t") {
            auto n1 = datagen::random<uint16_t>();
            auto n2 = datagen::random<uint16_t>();
            REQUIRE(n1 != n2);
        }

        SECTION("test int32_t") {
            auto n1 = datagen::random<int32_t>();
            auto n2 = datagen::random<int32_t>();
            REQUIRE(n1 != n2);
        }

        SECTION("test uint32_t") {
            auto n1 = datagen::random<uint32_t>();
            auto n2 = datagen::random<uint32_t>();
            REQUIRE(n1 != n2);
        }

        SECTION("test int64_t") {
            auto n1 = datagen::random<int64_t>();
            auto n2 = datagen::random<int64_t>();
            REQUIRE(n1 != n2);
        }

        SECTION("test uint64_t") {
            auto n1 = datagen::random<uint64_t>();
            auto n2 = datagen::random<uint64_t>();
            REQUIRE(n1 != n2);
        }

        SECTION("test float") {
            auto n1 = datagen::random<float>();
            auto n2 = datagen::random<float>();
            REQUIRE(n1 != n2);
        }

        SECTION("test double") {
            auto n1 = datagen::random<double>();
            auto n2 = datagen::random<double>();
            REQUIRE(n1 != n2);
        }
    }

    SECTION("can apply between limitation") {
        SECTION("test int8_t") {
            auto n = datagen::random<int8_t>(datagen::limits::between<int8_t>(-1, 1));
            REQUIRE(n >= -1);
            REQUIRE(n <= -1);
        }
    }

    SECTION("can apply greather_than limitation") {
        SECTION("test int8_t") {
            auto n = datagen::random<int8_t>(datagen::limits::greater_than<int8_t>(126));
            REQUIRE((int) n > 126);
        }
    }

    SECTION("can apply less_than limitation") {
        SECTION("test int8_t") {
            auto n = datagen::random<int8_t>(datagen::limits::less_than<int8_t>(-126));
            REQUIRE((int) n < -126);
        }
    }
}

