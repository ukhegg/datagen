//
// Created by ukhegg on 13.04.2017.
//
#include <catch2/catch.hpp>
#include <datagen/boost_random.hpp>
#include "datagen/random.hpp"

using namespace datagen;
using namespace boost::posix_time;

TEST_CASE("boost random tests")
{
	SECTION("ptime tests")
	{
		GIVEN("no generation limits")
		{
			WHEN("generating sequence of ptime\'s")
			{
				auto has_normal{false},
						has_pos_inf{false},
						has_neg_inf{false},
						has_not_a_date_time{false};
				for(auto i = 0 ;i < 100 ;++i)
				{
					auto p = random<ptime>();
					if(p.is_not_a_date_time()) has_not_a_date_time = true;
					if(p.is_pos_infinity()) has_pos_inf = true;
					if(p.is_neg_infinity()) has_neg_inf = true;
					if(!p.is_special()) has_normal = true;
					if(has_normal && has_pos_inf && has_neg_inf && has_not_a_date_time) break;
				}

				THEN("generated values has normal and special values")
				{
					REQUIRE(has_normal);
					REQUIRE(has_pos_inf);
					REQUIRE(has_neg_inf);
					REQUIRE(has_not_a_date_time);
				}
			}

			WHEN("generating sequence of normal ptime\'s")
			{
				std::vector<ptime> generated;
				while(generated.size() != 1000)
				{
					auto p = random<ptime>();
					if(p.is_special()) continue;
					if(p.is_not_a_date_time()) continue;
					generated.push_back(p);
				}

				THEN("the distribution is between [min_date_time, max_date_time]")
				{
					sort(generated.begin(), generated.end());

					REQUIRE(generated[0] >= ptime(min_date_time));
					REQUIRE(generated.back() <= ptime(max_date_time));
				}
			}
		}

		GIVEN("between dates generation limit")
		{
			auto l = limits::between(ptime(boost::gregorian::date(1970, boost::date_time::Jan, 1)),
			                         ptime(boost::gregorian::date(1970, boost::date_time::Dec, 31)));

			WHEN("generating sequence of ptime\'s")
			{
				std::vector<ptime> generated;
				while(generated.size() != 1000)
				{
					auto p = random<ptime>(l);
					generated.push_back(p);
				}

				THEN("there are no specials")
				{
					REQUIRE_FALSE((std::any_of(generated.begin(), generated.end(), [](ptime const& p) { return p.is_special(); })));
				}

				THEN("the distribution is between in given limit")
				{
					sort(generated.begin(), generated.end());

					REQUIRE(generated[0] >= l.from);
					REQUIRE(generated.back() <= l.to);
				}
			}
		}
	}

	SECTION("time_duration tests")
	{
		GIVEN("no generation limits")
		{
			WHEN("generating sequence of time_duration\'s")
			{
				auto has_normal{false},
						has_pos_inf{false},
						has_neg_inf{false};
				for(auto i = 0 ;i < 100 ;++i)
				{
					auto p = random<time_duration>();
					if(p.is_pos_infinity()) has_pos_inf = true;
					if(p.is_neg_infinity()) has_neg_inf = true;
					if(!p.is_special()) has_normal = true;
					if(has_normal && has_pos_inf && has_neg_inf) break;
				}

				THEN("generated values has normal and special values")
				{
					REQUIRE(has_normal);
					REQUIRE(has_pos_inf);
					REQUIRE(has_neg_inf);
				}
			}

			WHEN("generating sequence of normal time_duration\'s")
			{
				std::vector<time_duration> generated;
				while(generated.size() != 1000)
				{
					auto p = random<time_duration>();
					if(p.is_special()) continue;
					if(p.is_not_a_date_time()) continue;
					generated.push_back(p);
				}

				THEN("the distribution is between [neg_infin, pos_infin]")
				{
					sort(generated.begin(), generated.end());

					REQUIRE(generated[0] >= time_duration(neg_infin));
					REQUIRE(generated.back() <= time_duration(pos_infin));
				}
			}
		}

		GIVEN("between dates generation limit")
		{
			auto l = limits::between(seconds(5), seconds(15));

			WHEN("generating sequence of ptime\'s")
			{
				std::vector<time_duration> generated;
				while(generated.size() != 1000)
				{
					auto p = random<time_duration>(l);
					generated.push_back(p);
				}

				THEN("there are no specials")
				{
					REQUIRE_FALSE((std::any_of(generated.begin(), generated.end(),
						[](time_duration const& p) { return p.is_special(); })));
				}

				THEN("the distribution is between in given limit")
				{
					sort(generated.begin(), generated.end());

					REQUIRE(generated[0] >= l.from);
					REQUIRE(generated.back() <= l.to);
				}
			}
		}
	}

	SECTION("random ip_address tests")
	{
		GIVEN("no generation limits")
		{
			WHEN("generating random ip_address")
			{
				std::vector<boost::asio::ip::address> generated;
				for (int i = 0; i < 100; ++i)
					generated.push_back(datagen::random<boost::asio::ip::address>());

				THEN("both v4 and v6 are present")
				{
					REQUIRE((std::any_of(generated.begin(), generated.end(),
						[](boost::asio::ip::address const& addr) { return addr.is_v4(); })));
					REQUIRE((std::any_of(generated.begin(), generated.end(),
						[](boost::asio::ip::address const& addr) { return addr.is_v6(); })));
				}
			}
		}
	}
}
