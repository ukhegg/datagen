//
// Created by ukhegg on 15.03.2017.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.hpp"


namespace datagen {
    namespace impl {
        template<class T>
        struct random {
            //static T get() { return T(); }
        };
    }
}
namespace datagen {
    namespace impl {
        template<>
        struct random<int> {
            static int get() { return 1; }
        };
    }
}

namespace datagen {
    namespace aux {
        template<class TParent>
        struct any_type {
            template<class T, class =
            std::enable_if_t<!std::is_same<TParent, T>{}>>
            constexpr operator T();
        };

        template<class T>
        struct has_random_impl {
        private:
            static std::false_type get_type(...);

            template<class C>
            static typename std::enable_if<std::is_same<C, decltype(::datagen::impl::random<C>::get())>::value, std::true_type>::type
            get_type(C);

        public:
            static constexpr bool value = std::is_same<std::true_type, decltype(get_type(std::declval<T>()))>::value;
        };

        template<class T>
        struct direct_random_invoker {
            static T invoke() { return impl::random<T>::get(); }
        };

        template<class T>
        struct is_constructible_with_1_param{
            static constexpr bool value = std::is_constructible<T, any_type<T>>::value;
        };

        template<class T>
        struct one_param_random_constructor{
            static T invoke() {
                any_type<T> arg;
                return std::move(T(arg));
            }
        };

        template<class T>
        struct select_generator {
        private:
            struct no_generator {
            };

            static no_generator get_type(...);

            template<class C>
            static typename std::enable_if<has_random_impl<T>::value, direct_random_invoker<C>>::type get_type(C);

            template<class C>
            static typename std::enable_if<is_constructible_with_1_param<C>::value, one_param_random_constructor<C>>::type get_type(C);
        public:
            using type = decltype(get_type(std::declval<T>()));

            static_assert(!std::is_same<no_generator, type>::value, "no generator available for type");
        };
    }

    template<class T>
    T random() {
        return aux::select_generator<T>::type::invoke();
    }
}

struct constructible_with_int{
    constructible_with_int(int val) : value(val){}
    int value;
};


TEST_CASE("") {
    SECTION("test has random impl") {
        auto t1 = std::is_same<int, decltype(::datagen::impl::random<int>::get())>::value;
        REQUIRE(datagen::aux::has_random_impl<int>::value);
    }

    SECTION("has direct random invoker for types with random impl") {
        using actual_invoker_t = datagen::aux::select_generator<int>::type;
        using expected_invoker_t = datagen::aux::direct_random_invoker<int>;
        REQUIRE(typeid(actual_invoker_t).name() == typeid(expected_invoker_t).name());
    }

    SECTION("can construct int") {
        REQUIRE(datagen::random<int>() == 1);
    }

    SECTION("check is_constructible_with_1_param work") {
        REQUIRE(datagen::aux::is_constructible_with_1_param<constructible_with_int>::value);
    }

    SECTION("has one_param_random_constructor for types constructible with 1 param") {
        using actual_invoker_t = datagen::aux::select_generator<constructible_with_int>::type;
        using expected_invoker_t = datagen::aux::one_param_random_constructor<constructible_with_int>;
        REQUIRE(typeid(actual_invoker_t) == typeid(expected_invoker_t));
    }
}
