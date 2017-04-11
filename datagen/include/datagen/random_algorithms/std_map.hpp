//
// Created by ukhegg on 11.04.2017.
//

#ifndef DATAGEN_LIB_STD_MAP_HPP
#define DATAGEN_LIB_STD_MAP_HPP

#include <map>
#include <datagen/random_source_fwd.hpp>

namespace datagen {
    template<class Key, class TValue, class Traits, class Allocator>
    struct value_generation_algorithm<std::map<Key, TValue, Traits, Allocator>> {
        using map_t = std::map<Key, TValue, Traits, Allocator>;

        map_t get_random(random_source_base &r_source) {
            map_t res;
            auto s = r_source.create<size_t>() % 30;
            for (auto i = 0; i < s; ++i) {
                res[r_source.create<Key>()] = std::move(r_source.create<TValue>());
            }
            return std::move(res);
        };
    };
}
#endif //DATAGEN_LIB_STD_MAP_HPP
