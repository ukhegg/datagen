cmake_minimum_required(VERSION 3.6)
project(datagen_tests)

set(CMAKE_CXX_STANDARD 14)

include_directories("${Catch_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/datagen/include")
set(SOURCE_FILES 
        src/any_type_tests.cpp
        src/braces_contruction_tests.cpp.cpp
        src/ctor_construction_tests.cpp
        src/explicit_random_invoker_tests.cpp
		src/main.cpp
        src/random_method_selector_tests.cpp
        )
add_executable(datagen_tests ${SOURCE_FILES})