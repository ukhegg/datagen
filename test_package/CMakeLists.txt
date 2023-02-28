cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)

find_package(datagen CONFIG REQUIRED)

add_executable(test main.cpp)
target_link_libraries(test datagen::datagen)
