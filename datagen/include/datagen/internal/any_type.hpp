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

        template<class TForbidden, class TInjector>
        struct any_type_with_injector {
            any_type_with_injector(TInjector& injector) : injector_(injector){}

            template<class C,
                    class = typename std::enable_if<!std::is_same<C, TForbidden>::value, void>::type>
            constexpr operator C() const {
                return std::move(this->injector_.template create<C>());
            }
        private:
            TInjector& injector_;
        };
    }
}

#endif //DATAGEN_ANY_TYPE_HPP
