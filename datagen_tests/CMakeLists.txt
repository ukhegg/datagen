cmake_minimum_required(VERSION 3.6)
project(datagen_tests)
set(CMAKE_CXX_STANDARD 17)

find_package(Catch2 REQUIRED)


set(SOURCE_FILES
        src/any_type_tests.cpp
        src/braces_contruction_tests.cpp
        src/ctor_construction_tests.cpp
        src/explicit_random_invoker_tests.cpp
        src/random_method_selector_tests.cpp
        src/random_tests.cpp
        src/stl_random_tests.cpp
        src/builtin_limits_tests.cpp
        src/random_limits_tests.cpp
        src/limit_traits_tests.cpp
        src/stl_select_random_tests.cpp)


add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME}
        PRIVATE ${datagen_SOURCE_DIR}/include
                ${Catch2_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${Catch2_LIBRARIES})

if (DEFINED Boost_INCLUDE_DIR)
    target_link_libraries(${PROJECT_NAME} Boost::system Boost::filesystem Boost::random)
endif ()