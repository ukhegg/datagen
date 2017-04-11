cmake_minimum_required(VERSION 3.0)
project(datagen_lib)

set(Catch_DIR "${CMAKE_SOURCE_DIR}/3rd_party/catch")
set(Gsl_DIR "${CMAKE_SOURCE_DIR}/3rd_party/gsl")

add_subdirectory(datagen)
add_subdirectory(datagen_tests)