cmake_minimum_required(VERSION 3.17)
project(MetamorphicTestFramework)

set(CMAKE_CXX_STANDARD 14)

include_directories(./include ./modules)

aux_source_directory(./src SOURCE_FILES)

add_subdirectory(modules/lib_pthread)

add_executable(MetamorphicTestFramework main.cpp ${SOURCE_FILES})

target_link_libraries(MetamorphicTestFramework pthread LibPthread)