//
// Created by ukhegg on 10.04.2017.
//

#ifndef DATAGEN_LIB_RANDOM_LIMITS_HPP
#define DATAGEN_LIB_RANDOM_LIMITS_HPP

#include "datagen/value_generation_algorithm.hpp"

namespace datagen
{
	class random_source_base;

	namespace limits
	{
		namespace internal
		{
			//void apply(){}
			// resolves to std::true_type is datagen::limits::apply(random_source_base&,
			//                                                      TLimit const&,
			//                                                      value_generation_algorithm<TValue>&) is defined
			template <class TValue, class TLimit>
			struct is_algorithm_limit
			{
			private:
				static std::false_type test(...);

				template <class V, class L>
				static typename std::enable_if<std::is_same<decltype(adjust_algorithm(std::declval<random_source_base&>(),
				                                                                      std::declval<L const&>(),
				                                                                      std::declval<value_generation_algorithm<V>&>())),
				                                            void>::value,
				                               std::true_type>::type test(V, L);
			public:
				static constexpr auto value = decltype(test(std::declval<TValue>(), std::declval<TLimit>())){};
			};

			// resolves to std::true_type is datagen::limits::apply(random_source_base&,
			//                                                      TLimit const&,
			//                                                      TValue&) is defined
			template <class TValue, class TLimit>
			struct is_value_limit
			{
			private:
				static std::false_type test(...);

				template <class V, class L>
				static typename std::enable_if<std::is_same<decltype(adjust_value(std::declval<random_source_base&>(),
				                                                                  std::declval<L const&>(),
				                                                                  std::declval<V&>())),
				                                            void>::value,
				                               std::true_type>::type test(V, L);
			public:
				static constexpr auto value = decltype(test(std::declval<TValue>(), std::declval<TLimit>())){};
			};

			template <class TValue, class TLimit, bool IsAlgorithmLimit>
			struct algorithm_tuner
			{
				static void apply(random_source_base& r_source, TLimit const& limit, value_generation_algorithm<TValue>& alg)
				{
				}
			};

			template <class TValue, class TLimit>
			struct algorithm_tuner<TValue, TLimit, true>
			{
				static_assert(is_algorithm_limit<TValue, TLimit>::value,
					"TLimit is not a algorithm limit for value_generation_algorithm<TValue>");

				static void apply(random_source_base& r_source, TLimit const& limit, value_generation_algorithm<TValue>& alg)
				{
					limits::adjust_algorithm(r_source, limit, alg);
				}
			};

			template <class TValue, class TLimit, bool IsValueLimit>
			struct value_tuner
			{
				static void apply(random_source_base& r_source, TLimit const& limit, TValue& value)
				{
				}
			};

			template <class TValue, class TLimit>
			struct value_tuner<TValue, TLimit, true>
			{
				static void apply(random_source_base& r_source, TLimit const& limit, TValue& value)
				{
					static_assert(is_value_limit<TValue, TLimit>::value,
						"TLimit is not a TValue limit");

					limits::adjust_value(r_source, limit, value);
				}
			};

			template <class TValue, class ... TLimits>
			struct enum_limits
			{
				static void apply_algorithm_limits(random_source_base& r_source,
				                                   value_generation_algorithm<TValue>& alg,
				                                   TLimits&& ... limits)
				{
				}

				static void apply_value_limits(random_source_base& r_source,
				                               TValue& value,
				                               TLimits&& ... limits)
				{
				}
			};

			template <class TValue>
			struct enum_limits<TValue, void>
			{
				static void apply_algorithm_limits(random_source_base& r_source, value_generation_algorithm<TValue>& alg)
				{
				}

				static void apply_value_limits(random_source_base& r_source, TValue& value)
				{
				}
			};

			template <class TValue, class TLimit, class ... TOtherLimits>
			struct enum_limits<TValue, TLimit, TOtherLimits...>
			{
				static_assert(is_algorithm_limit<TValue, TLimit>::value || is_value_limit<TValue, TLimit>::value,
					"TLimit is nor algorithm, nor value limit");
				using algorith_tuner_t = algorithm_tuner<TValue, TLimit, is_algorithm_limit<TValue, TLimit>::value>;
				using value_tuner_t = value_tuner<TValue, TLimit, is_value_limit<TValue, TLimit>::value>;

				static void apply_algorithm_limits(random_source_base& r_source,
				                                   value_generation_algorithm<TValue>& alg,
				                                   TLimit&& limit,
				                                   TOtherLimits&& ... other_limits)
				{
					algorith_tuner_t::apply(r_source, std::forward<TLimit>(limit), alg);
					enum_limits<TValue, TOtherLimits...>::apply_algorithm_limits(r_source, alg, std::forward<TOtherLimits>(other_limits)...);
				}

				static void apply_value_limits(random_source_base& r_source,
				                               TValue& value,
				                               TLimit&& limit,
				                               TOtherLimits&& ... other_limits)
				{
					value_tuner_t::apply(r_source, std::forward<TLimit>(limit), value);
					enum_limits<TValue, TOtherLimits...>::apply_value_limits(r_source, value, std::forward<TOtherLimits>(other_limits)...);
				}
			};
		}


		template <class TValue, class ... TLimits>
		void apply_algorithm_limits(random_source_base& r_source,
		                            value_generation_algorithm<TValue>& alg,
		                            TLimits&& ... limits)
		{
			internal::enum_limits<TValue, TLimits...>::apply_algorithm_limits(r_source, alg, std::forward<TLimits>(limits)...);
		}

		template <class TValue, class ... TLimits>
		void apply_value_limits(random_source_base& r_source,
		                        TValue& value,
		                        TLimits&& ... limits)
		{
			internal::enum_limits<TValue, TLimits...>::apply_value_limits(r_source, value, std::forward<TLimits>(limits)...);
		}
	}
}
#endif //DATAGEN_LIB_RANDOM_LIMITS_HPP
