//
// Created by Zeliukin Nick on 30.06.22.
//
#include <datagen/stl_select_random.hpp>
#include <catch2/catch.hpp>

using namespace datagen;

namespace datagen::tests
{
    TEST_CASE("stl_select random tests")
    {
        SECTION("select from vector") {

            std::vector<char> v = {'a', 'b', 'c', 'd'};

            REQUIRE(select_random(v) >= 'a');
            REQUIRE(select_random(v) <= 'd');
        }

        SECTION("select from list") {
            std::list<char> l = {'a', 'b', 'c', 'd'};
            REQUIRE(select_random(l) >= 'a');
            REQUIRE(select_random(l) <= 'd');
        }

        SECTION("select from string") {
            std::string s = "abcd";
            REQUIRE(select_random(s) >= 'a');
            REQUIRE(select_random(s) <= 'd');
        }

        SECTION("select from set") {
            std::set<char> s = {'a', 'b', 'c', 'd'};

            REQUIRE(select_random(s) >= 'a');
            REQUIRE(select_random(s) <= 'd');
        }

        SECTION("select from map") {
            std::map<char, char> m = {{'a', 'a'},
                                      {'b', 'b'},
                                      {'c', 'c'},
                                      {'d', 'd'}};

            REQUIRE(select_random(m).first >= 'a');
            REQUIRE(select_random(m).second >= 'a');
            REQUIRE(select_random(m).first <= 'd');
            REQUIRE(select_random(m).second <= 'd');
        }
    }
}