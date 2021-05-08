cmake_minimum_required(VERSION 3.17)
project(MetamorphicTestFramework)

set(CMAKE_CXX_STANDARD 14)

include_directories(./include ./modules)

file(GLOB_RECURSE SOURCE_FILES ./src/*)

add_subdirectory(modules/my_pthread)

add_subdirectory(modules/mem_pool)

add_executable(MetamorphicTestFramework main.cpp ${SOURCE_FILES})

target_link_libraries(MetamorphicTestFramework MyPthread MemPool)