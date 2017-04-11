cmake_minimum_required(VERSION 3.6)
project(datagen_tests)

if (MSVC)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP -DBOOST_SYSTEM_NO_DEPRECATED -D_WIN32_WINNT=0x0601 -DTPL_DLL_EXPORTS ")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd\"4251\" /wd\"4275\" /wd\"4005\" /wd\"4503\"")
else ()
endif ()

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
		src/random_tests.cpp
		src/stl_random_tests.cpp
		src/builtin_limits_tests.cpp
        src/random_limits_tests.cpp)
add_executable(datagen_tests ${SOURCE_FILES})