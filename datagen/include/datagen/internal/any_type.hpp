//
// Created by ukhegg on 07.04.2017.
//

#ifndef DATAGEN_ANY_TYPE_HPP
#define DATAGEN_ANY_TYPE_HPP

namespace datagen {
    namespace internal {
        template<class TForbidden>
        struct any_type {
            template<class C,
                    class = typename std::enable_if<!std::is_same<C, TForbidden>::value, void>::type>
            constexpr operator C() const{
                return C();
            }
        };

        template<class TForbidden, class TRandomSource>
        struct any_type_with_rsrc {
            any_type_with_rsrc(TRandomSource& random_source) : random_source_(random_source){}

            template<class C,
                    class = typename std::enable_if<!std::is_same<C, TForbidden>::value, void>::type>
            constexpr operator C() const {
                return std::move(this->random_source_.template create<C>());
            }
        private:
            TRandomSource& random_source_;
        };
    }
}

#endif //DATAGEN_ANY_TYPE_HPP
