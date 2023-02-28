//
// Created by Zeliukin Nick on 22.06.22.
//



#include <catch2/catch_all.hpp>
#include <datagen/limits/builtin_limits.hpp>
#include <datagen/random.hpp>

void adjust_value(datagen::random_source_base &rs, datagen::limits::between_t<int> const &l, int val)
{

}

struct A
{
    int val;

    bool operator<(A const &rhs) const
    {
        return this->val < rhs.val;
    }
};

void adjust_value(datagen::random_source_base &rs, datagen::limits::between_t<A> const &l, A val)
{
}


TEST_CASE("limit_traits tests")
{
    SECTION("has_less_operator_v tests") {
        REQUIRE(datagen::limits::details::has_less_operator_v<A>);
    }


    SECTION("some build in limits tests") {

        datagen::random_source_impl rs;
        A val = {999};
        auto limit = datagen::limits::between_t(A{10}, A{20});

        REQUIRE(std::is_same_v<decltype(adjust_value(rs, limit, val)), void>);
        datagen::limits::limits_applicator::do_adjust_value(rs, limit, val);
        REQUIRE(datagen::limits::internal::is_value_limit_v<A, datagen::limits::between_t<A>>);
        REQUIRE(datagen::limits::internal::is_value_limit_v<int, datagen::limits::between_t<int>>);

        std::string str = "hello world";
        REQUIRE_FALSE(
                datagen::limits::internal::is_value_limit_v<std::string, datagen::limits::between_t<int>>);
    }
}