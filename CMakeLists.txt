cmake_minimum_required(VERSION 3.17)
project(MetamorphicTestFramework)

set(CMAKE_CXX_STANDARD 14)

include_directories(./include ./modules)

file(GLOB_RECURSE SOURCE_FILES ./src/*)

add_subdirectory(modules/mem_pool)

add_subdirectory(modules/event_system)

add_subdirectory(modules/time_system)

add_subdirectory(modules/task_system)

add_subdirectory(modules/my_pthread)

add_executable(MetamorphicTestFramework main.cpp ${SOURCE_FILES})

target_link_libraries(MetamorphicTestFramework TaskSystem TimeSystem EventSystem MyPthread MemPool)