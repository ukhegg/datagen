cmake_minimum_required(VERSION 3.0)
project(datagen_lib)

set(BOOST_ROOT "C:/Projects/boost_1_61_0")
find_package(Boost REQUIRED COMPONENTS system filesystem random)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ALL_NO_LIB -DBOOST_LOG_DYN_LINK")
set(Catch_DIR "${CMAKE_SOURCE_DIR}/3rd_party/catch")

add_subdirectory(datagen)
add_subdirectory(datagen_tests)