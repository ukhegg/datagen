//
// Created by ukhegg on 15.03.2017.
//

#ifndef DATAGEN_LIB_RANDOM_SOURCE_HPP
#define DATAGEN_LIB_RANDOM_SOURCE_HPP
#include "datagen/datagen.hpp"

namespace datagen{
    DATAGEN_API int get_five();

    class DATAGEN_API random_source
    {
    public:
        virtual ~random_source() = default;


    };
}
#endif //DATAGEN_LIB_RANDOM_SOURCE_HPP
