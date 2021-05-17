cmake_minimum_required(VERSION 3.17)
project(MetamorphicTestFramework)

set(CMAKE_CXX_STANDARD 14)

include_directories(./include ./modules)

file(GLOB_RECURSE SOURCE_FILES ./src/*)

add_subdirectory(modules)

add_executable(MetamorphicTestFramework main.cpp ${SOURCE_FILES})

target_link_libraries(MetamorphicTestFramework Modules)