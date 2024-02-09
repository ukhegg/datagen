cmake_minimum_required(VERSION 3.12)
set(CMAKE_HOMEPAGE_COMPILER g++)

project(datagen
        VERSION 1.0.5
        DESCRIPTION "Header-only library for generating random test data & structures"
        HOMEPAGE_URL "https://github.com/ukhegg/datagen")


include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 17)

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

add_library(datagen INTERFACE)
target_include_directories(datagen INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_features(datagen INTERFACE cxx_std_17)


install(TARGETS datagen
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

write_basic_package_version_file("datagenConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/datagen DESTINATION include)




