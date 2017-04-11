//
// Created by ukhegg on 11.04.2017.
//

#ifndef DATAGEN_LIB_STD_SET_HPP
#define DATAGEN_LIB_STD_SET_HPP
#include <set>
#include <datagen/random_source_fwd.hpp>
namespace datagen {


    template<class Key, class Traits, class Allocator>
    struct value_generation_algorithm<std::set<Key, Traits, Allocator>> {
        using set_t = std::set<Key, Traits, Allocator>;

        set_t get_random(random_source_base &r_source) {
            set_t res;
            auto s = r_source.create<size_t>() % 30;
            for (auto i = 0; i < s; ++i) {
                res.insert(std::move(r_source.create<Key>()));
            }
            return std::move(res);
        };
    };
}
#endif //DATAGEN_LIB_STD_SET_HPP
