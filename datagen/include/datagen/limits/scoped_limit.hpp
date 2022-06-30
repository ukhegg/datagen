//
// Created by ukhegg on 14.04.2017.
//

#ifndef DATAGEN_LIB_SCOPED_LIMIT_HPP
#define DATAGEN_LIB_SCOPED_LIMIT_HPP

#include <type_traits>
#include <stdexcept>
#include <memory>
#include "datagen/value_generation_algorithm.hpp"
#include "datagen/limits/random_limits.hpp"

namespace datagen
{
    namespace limits
    {
        namespace internal
        {
            template<class TValue>
            struct type_scoped_limit
            {
                virtual ~type_scoped_limit() = default;

                virtual void
                adjust_algorithm(random_source_base &r_source, value_generation_algorithm<TValue> &alg) = 0;

                virtual void adjust_value(random_source_base &r_source, TValue &value) = 0;
            };

            template<class TValue, class TLimit>
            struct type_scoped_limit_impl : type_scoped_limit<TValue>
            {
            private:
                TLimit limit_;

            public:
                explicit type_scoped_limit_impl(TLimit const &limit) : limit_(limit)
                {
                }

                ~type_scoped_limit_impl() override = default;

                void adjust_algorithm(random_source_base &r_source, value_generation_algorithm<TValue> &alg) override
                {
                    limits_applicator::do_adjust_algorithm(r_source, this->limit_, alg);
                }

                void adjust_value(random_source_base &r_source, TValue &value) override
                {
                    limits_applicator::do_adjust_value(r_source, this->limit_, value);
                }
            };

        }

        template<class TType>
        struct type_scoped_limits
        {
            using scoped_limit_t = internal::type_scoped_limit<TType>;

            static void adjust_algorithm(random_source_base &r_source, value_generation_algorithm<TType> &algorithm)
            {
                if (registered_limits_.empty()) return;
                for (auto l: registered_limits_) {
                    l->adjust_algorithm(r_source, algorithm);
                }
            }

            static void adjust_value(random_source_base &r_source, TType &value)
            {
                if (registered_limits_.empty()) return;
                for (auto l: registered_limits_) {
                    l->adjust_value(r_source, value);
                }
            }

            static void register_type_limit(scoped_limit_t &limit)
            {
                registered_limits_.push_back(&limit);
            }

            static void unregister_type_limit(scoped_limit_t &limit)
            {
                if (registered_limits_.empty())
                    throw std::runtime_error(
                            "trying to unregoster scoped limit that was not registered or already removed");
                if (registered_limits_.back() != &limit)
                    throw std::runtime_error("limit unregister order errro");

                registered_limits_.pop_back();
            }

            static std::vector<scoped_limit_t *> registered_limits()
            {
                return registered_limits_;
            }

        private:
            static std::vector<scoped_limit_t *> registered_limits_;
        };

        template<class TValue>
        std::vector<typename type_scoped_limits<TValue>::scoped_limit_t *> type_scoped_limits<TValue>::registered_limits_;

        namespace internal
        {
            template<class TValue, class TLimit>
            struct scoped_limit_guard
            {
                explicit scoped_limit_guard(TLimit const &limit)
                        : slimit_(limit)
                {
                    type_scoped_limits<TValue>::register_type_limit(this->slimit_);
                }

                ~scoped_limit_guard()
                {
                    type_scoped_limits<TValue>::unregister_type_limit(this->slimit_);
                }

            private:
                type_scoped_limit_impl<TValue, TLimit> slimit_;
            };
        }

        template<class TLimit>
        struct scoped_limit_t
        {
            template<class TType>
            using limit_guard_t = internal::scoped_limit_guard<TType, TLimit>;

            explicit scoped_limit_t(TLimit const &limit) : limit_(limit)
            {
            }

            template<class TType>
            std::unique_ptr<limit_guard_t<TType>> apply_to_type()
            {
                //static_assert(internal::is_value_limit<TType, TLimit>::value
                //	|| internal::is_algorithm_limit<TType, TLimit>::value,
                //	"provided limit is nor value,nor algorithm limit,so can\'t be scoped");
                return std::move(std::make_unique<limit_guard_t<TType>>(this->limit_));
            }

        private:
            TLimit limit_;
        };

        template<class TLimit>
        struct scoped_limit_t<TLimit> scoped_limit(TLimit const &l)
        {
            return std::move(scoped_limit_t<TLimit>(l));
        }
    }
}
#endif //DATAGEN_LIB_SCOPED_LIMIT_HPP
