cmake_minimum_required(VERSION 3.0)
project(datagen_lib)

include("${CMAKE_SOURCE_DIR}/3rd_party/cmake_utils/utils.cmake")
set(Catch_DIR "${CMAKE_SOURCE_DIR}/3rd_party/catch")
set(Gsl_DIR "${CMAKE_SOURCE_DIR}/3rd_party/gsl")

add_subdirectory(datagen)
add_subdirectory(datagen_tests)