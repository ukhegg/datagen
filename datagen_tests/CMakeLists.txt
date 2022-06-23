cmake_minimum_required(VERSION 3.6)
project(datagen_tests)
set(CMAKE_CXX_STANDARD 17)

find_package(Catch2 REQUIRED)


set(SOURCE_FILES
        src/any_type_tests.cpp
        src/braces_contruction_tests.cpp.cpp
        src/ctor_construction_tests.cpp
        src/explicit_random_invoker_tests.cpp
        src/main.cpp
        src/random_method_selector_tests.cpp
        src/random_tests.cpp
        src/stl_random_tests.cpp
        src/builtin_limits_tests.cpp
        src/random_limits_tests.cpp
        src/limit_traits_tests.cpp
        )


add_executable(datagen_tests ${SOURCE_FILES})
target_include_directories(datagen_tests PRIVATE ${datagen_SOURCE_DIR}/include)

if (DEFINED Boost_INCLUDE_DIR)
    target_link_libraries(datagen_tests ${Boost_LIBRARIES})
endif ()