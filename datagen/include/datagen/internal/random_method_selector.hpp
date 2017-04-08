//
// Created by ukhegg on 07.04.2017.
//

#ifndef DATAGEN_RANDOM_METHOD_SELECTOR_HPP
#define DATAGEN_RANDOM_METHOD_SELECTOR_HPP

#include "braces_construction.hpp"
#include "ctor_construction.hpp"
#include "explicit_random_invoker.hpp"

namespace datagen
{
	namespace internal
	{
		struct none_t
		{
		};

		template<class TFirst, class ... TOthers>
		struct select_first_not_none
		{
			using type = TFirst;
		};

		template<class ... TOthers>
		struct select_first_not_none<none_t, TOthers ...>
		{
			using type = typename select_first_not_none<TOthers...>::type;
		};

		template<>
		struct select_first_not_none<none_t>
		{
			using type = none_t;
		};

		template <class TValue>
		struct random_method_selector
		{
		private:
			using braces_ctor = braces_ctor_traits<TValue>;
			using ctor = ctor_traits<TValue>;

			static const auto has_get_random_overload = has_explicit_random<TValue>::value;
		public:
			using braces_ctor_invoker = typename std::conditional<braces_ctor::is_constructible, braces_initializer_invoker, none_t>::type;
			using direct_ctor_invoker = typename std::conditional<ctor::is_constructible, direct_ctor_invoker, none_t>::type;
			using direct_random_invoker = typename std::conditional<has_get_random_overload, direct_get_random_invoker, none_t>::type;

			using invoker = typename select_first_not_none<direct_random_invoker, direct_ctor_invoker, braces_ctor_invoker>::type;
		};
	}
}
#endif //DATAGEN_RANDOM_METHOD_SELECTOR_HPP
