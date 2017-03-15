//
// Created by ukhegg on 15.03.2017.
//

#ifndef DATAGEN_LIB_BUILDIN_HPP
#define DATAGEN_LIB_BUILDIN_HPP

#include "datagen/datagen.hpp"
#include "datagen/random.hpp"
#include <stdint.h>
#include <type_traits>

namespace datagen {
    namespace impl {
        template<>
        struct DATAGEN_API random<int8_t> {
            inline uint8_t operator()() { return rand() % std::numeric_limits<int8_t>::max(); }
        };

        template<>
        struct DATAGEN_API random<uint8_t> {
            inline uint8_t operator()() { return rand() % std::numeric_limits<uint8_t>::max(); }
        };

        template<>
        struct DATAGEN_API random<int16_t> {
            inline int16_t operator()() { return rand() % std::numeric_limits<int16_t>::max(); }
        };

        template<>
        struct DATAGEN_API random<uint16_t> {
            inline uint16_t operator()() { return rand() % std::numeric_limits<uint16_t>::max(); }
        };

        template<>
        struct DATAGEN_API random<int32_t> {
            inline int32_t operator()() { return rand() % std::numeric_limits<int32_t>::max(); }
        };

        template<>
        struct DATAGEN_API random<uint32_t> {
            inline uint32_t operator()() { return rand() % std::numeric_limits<uint32_t>::max(); }
        };

        template<>
        struct DATAGEN_API random<int64_t> {
            inline int64_t operator()() { return rand() % std::numeric_limits<int64_t>::max(); }
        };

        template<>
        struct DATAGEN_API random<uint64_t> {
            inline uint64_t operator()() { return rand() % std::numeric_limits<uint64_t>::max(); }
        };

        template<>
        struct DATAGEN_API random<float> {
            inline float operator()() { return rand() * 1.0; }
        };

        template<>
        struct DATAGEN_API random<double> {
            inline double operator()() { return rand() * 1.0; }
        };

        template<class TNumeric>
        struct between_t : public value_limitation<TNumeric> {
            static_assert(std::is_arithmetic<TNumeric>::value, "TNumber must be arithmetic type");

            between_t(TNumeric lower, TNumeric upper) : lower_(lower), upper_(upper) {}

            void apply(TNumeric &val) const override {
                auto dt = upper_ - lower_;
                val = lower_ + val % dt;
            }

        private:
            TNumeric lower_;
            TNumeric upper_;
        };

    }

    namespace limits {
        template<class TNumeric>
        impl::between_t<TNumeric> between(TNumeric lower, TNumeric upper) {
            return impl::between_t<TNumeric>(lower, upper);
        }

        template<class TNumeric>
        impl::between_t<TNumeric> greater_than(TNumeric lower) {
            return impl::between_t<TNumeric>(lower, std::numeric_limits<TNumeric>::max());
        }

        template<class TNumeric>
        impl::between_t<TNumeric> less_than(TNumeric upper) {
            return impl::between_t<TNumeric>(std::numeric_limits<TNumeric>::min(), upper);
        }
    }
}


#endif //DATAGEN_LIB_BUILDIN_HPP
