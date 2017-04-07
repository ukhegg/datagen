//
// Created by ukheg on 07.04.2017.
//

#ifndef DATAGEN_EXPLICIT_RANDOM_INVOKER_HPP
#define DATAGEN_EXPLICIT_RANDOM_INVOKER_HPP

#include <type_traits>
namespace datagen {
    //template<class TInjector, class TValue>
	//TValue get_random(TInjector&, TValue*);

    namespace internal {

		template<class TValue>
		struct has_explicit_random {
		private:
			struct dummy_injector
			{
				template<class C>
				static C create();
			};

			static std::false_type test(...);

			template<class C>
			static typename std::enable_if<
				std::is_same<
					TValue,
					decltype(::datagen::get_random(std::declval<dummy_injector&>(), std::declval<C*>()))>::value,
				std::true_type>::type test(C);
		public:
			using test_result = decltype(has_explicit_random::test(std::declval<TValue>()));

			//static const bool value = std::is_same<std::true_type, test_result>::value;
			enum { value = std::is_same<std::true_type, test_result>::value };
		};

		struct direct_get_random_invoker {
			template<class C, class TInjector>
			static C create(TInjector &injector) {
				static_assert(has_explicit_random<C>::value,
					"get_random(TInjector&, C*) overload for type C not defined");
				return ::datagen::get_random(injector, static_cast<C*>(nullptr));
			}
		};
    }
}
#endif //DATAGEN_EXPLICIT_RANDOM_INVOKER_HPP
