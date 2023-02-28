//
// Created by ukhegg on 10.04.2017.
//

#include <catch2/catch_all.hpp>
#include <datagen/random.hpp>

#define CheckType(Type) \
SECTION(#Type " test"){\
    auto t1= datagen::random<Type>();\
    auto t2 = datagen::random<Type>();\
    REQUIRE(t1 != t2);\
}

TEST_CASE("random function tests")
{
	SECTION("bool value tests")
	{
		std::vector<bool> generated;
		generated.resize(1024);
		for (auto b : generated) b = datagen::random<bool>();

		auto true_count = std::count(generated.begin(), generated.end(), true);
		auto false_count = std::count(generated.begin(), generated.end(), false);

		REQUIRE((true_count > 450));
		REQUIRE((false_count > 450));
	}

	SECTION("buildin types generation tests")
	{
		CheckType(int8_t)
		CheckType(uint8_t)
		CheckType(int16_t)
		CheckType(uint16_t)
		CheckType(int32_t)
		CheckType(uint32_t)
		CheckType(int64_t)
		CheckType(uint64_t)
		CheckType(float)
		CheckType(double)
		CheckType(wchar_t)
	}

	SECTION("can generate simple structure")
	{
		struct dummy
		{
			int8_t t1;
			uint8_t t2;
			int16_t t3;
			uint16_t t4;
			int32_t t5;
			uint32_t t6;
			int64_t t7;
			uint64_t t8;
		};

		auto d1 = datagen::random<dummy>();
		auto d2 = datagen::random<dummy>();
		REQUIRE(d1.t1 != d2.t1);
		REQUIRE(d1.t2 != d2.t2);
		REQUIRE(d1.t3 != d2.t3);
		REQUIRE(d1.t4 != d2.t4);
		REQUIRE(d1.t5 != d2.t5);
		REQUIRE(d1.t6 != d2.t6);
		REQUIRE(d1.t7 != d2.t7);
		REQUIRE(d1.t8 != d2.t8);
	}

	SECTION("can generate class with ctor")
	{
		class dummy
		{
		public:
			dummy(int t) : t_(t) {}

			int get_t() const
			{
				return t_;
			}

		private:
			int t_{0};
		};

		auto d1 = datagen::random<dummy>();
		auto d2 = datagen::random<dummy>();
		REQUIRE(d1.get_t() != d2.get_t());
	}
}
