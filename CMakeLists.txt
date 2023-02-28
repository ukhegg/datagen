cmake_minimum_required(VERSION 3.0)
project(datagen_lib)

set(CMAKE_TOOLCHAIN_FILE, ${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
find_package(Boost REQUIRED COMPONENTS system filesystem random)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ALL_NO_LIB -DBOOST_LOG_DYN_LINK")

add_subdirectory(datagen)
add_subdirectory(datagen_tests)