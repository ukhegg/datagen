cmake_minimum_required(VERSION 3.0)
project(datagen)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDATAGEN_DLL_EXPORTS")
if (${BUILD_SHARED_LIBS})
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DDATAGEN_DLL")
endif ()

set(SOURCES
        include/datagen/datagen.hpp
        include/datagen/random_source.hpp
        src/random_source.cpp
        )

include_directories("${PROJECT_SOURCE_DIR}/include")

add_library(datagen ${SOURCES})


