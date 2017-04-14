cmake_minimum_required(VERSION 3.0)
project(datagen)

set(SOURCES
        include/datagen/internal/any_type.hpp
        include/datagen/internal/braces_construction.hpp
        include/datagen/internal/config.hpp
        include/datagen/internal/ctor_construction.hpp
        include/datagen/internal/explicit_random_invoker.hpp
        include/datagen/internal/random_method_selector.hpp
        include/datagen/limits/detail/stl_limits_details.hpp
        include/datagen/limits/builtin_limits.hpp
        include/datagen/limits/random_limits.hpp
        include/datagen/limits/scoped_limit.hpp
        include/datagen/limits/stl_limits.hpp
        include/datagen/random_algorithms/std_string.hpp
        include/datagen/random_algorithms/std_vector.hpp
        include/datagen/random_algorithms/std_set.hpp
        include/datagen/random_algorithms/std_map.hpp
        include/datagen/random_algorithms/std_array.hpp
        include/datagen/random_source_fwd.hpp
        include/datagen/random_source.hpp
        include/datagen/random.hpp
        include/datagen/stl_random.hpp
        include/datagen/value_generation_algorithm.hpp
        )

if (DEFINED Boost_INCLUDE_DIR)
    set(SOURCES ${SOURCES}
            include/datagen/random_algorithms/boost_optional.hpp
            include/datagen/random_algorithms/boost_posix_time.hpp
            include/datagen/random_algorithms/boost_ip_address.hpp
            include/datagen/boost_random.hpp
            )
	include_directories(${Boost_INCLUDE_DIR})
endif ()
include_directories(${PROJECT_SOURCE_DIR}/include)

add_custom_target(datagen SOURCES ${SOURCES})