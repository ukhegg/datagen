cmake_minimum_required(VERSION 3.0)
project(datagen_tests)


set(SOURCES
        buildin_random_tests.cpp
        main.cpp
        )

include_directories("${datagen_SOURCE_DIR}/include")
include_directories("${Catch_DIR}/include")
include_directories("${Gsl_DIR}/include")

add_executable(datagen_tests ${SOURCES})
#target_link_libraries(datagen_tests datagen)