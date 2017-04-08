cmake_minimum_required(VERSION 3.0)
project(datagen)

set(SOURCES
        include/datagen/random.hpp
        include/datagen/internal/braces_construction.hpp
        include/datagen/internal/ctor_construction.hpp
        include/datagen/internal/any_type.hpp
        include/datagen/internal/config.hpp
        include/datagen/internal/explicit_random_invoker.hpp
        include/datagen/internal/random_method_selector.hpp
        )
add_custom_target(datagen SOURCES ${SOURCES})