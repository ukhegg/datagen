//
// Created by Zeliukin Nick on 30.06.22.
//

#ifndef DATAGEN_LIB_STL_SELECT_RANDOM_HPP
#define DATAGEN_LIB_STL_SELECT_RANDOM_HPP

#include <set>
#include <list>
#include <vector>
#include <string>
#include <map>
#include "datagen/random.hpp"

namespace datagen
{
    template<class TValue, class TAllocator>
    TValue &select_random(std::vector<TValue, TAllocator> &v)
    {
        return v.at(random<size_t>(limits::between(0, v.size())));
    }

    template<class TValue, class TAllocator>
    TValue const &select_random(std::vector<TValue, TAllocator> const &v)
    {
        return v.at(random<size_t>(limits::between(0, v.size())));
    }

    template<class TValue, class TAllocator>
    TValue &select_random(std::list<TValue, TAllocator> &l)
    {
        auto it = l.begin();
        std::advance(it, random<size_t>(limits::between(0, l.size())));
        return *it;
    }

    template<class TValue, class TAllocator>
    TValue const &select_random(std::list<TValue, TAllocator> const &l)
    {
        auto it = l.begin();
        std::advance(it, random<size_t>(limits::between(0, l.size())));
        return *it;
    }

    template<class TChar, class TCharTraits, class TAllocator>
    TChar &select_random(std::basic_string<TChar, TCharTraits, TAllocator> &s)
    {
        return s.at(random<size_t>(limits::between(0, s.size())));
    }

    template<class TChar, class TCharTraits, class TAllocator>
    TChar const &select_random(std::basic_string<TChar, TCharTraits, TAllocator> const &s)
    {
        return s.at(random<size_t>(limits::between(0, s.size())));
    }

    template<class TValue, class TCompare, class TAllocator>
    TValue const &select_random(std::set<TValue, TCompare, TAllocator> const &s)
    {
        auto it = s.begin();
        std::advance(it, random<size_t>(limits::between(0, s.size())));
        return *it;
    }

    template<class TKey, class TValue, class TCompare, class TAllocator>
    std::pair<TKey const, TValue> &select_random(std::map<TKey, TValue, TCompare, TAllocator> &m)
    {
        auto it = m.begin();
        std::advance(it, random<size_t>(limits::between(0, m.size())));
        return *it;
    }

    template<class TKey, class TValue, class TCompare, class TAllocator>
    std::pair<TKey, TValue> const& select_random(std::map<TKey, TValue, TCompare, TAllocator> const &m)
    {
        auto it = m.begin();
        std::advance(it, random<size_t>(limits::between(0, m.size())));
        return *it;
    }
}
#endif //DATAGEN_LIB_STL_SELECT_RANDOM_HPP
