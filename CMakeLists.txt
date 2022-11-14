cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_C_STANDARD 17)
project(move-memory-manager LANGUAGES C CXX ASM)

add_subdirectory(3rdParty)

file(
    GLOB_RECURSE MEMORY_MANAGER_SOURCES
    "src/*.cpp"
)

file(
    GLOB_RECURSE MEMORY_MANAGER_HEADERS
    "include/*.hpp"
    "include/*.h"
)

set(MOVE_MEMORY_MANAGER_SOURCES ${MEMORY_MANAGER_SOURCES} ${MEMORY_MANAGER_HEADERS})

if (MOVE_MEMORY_MANAGER_SHARED)
    add_library(move-mm SHARED ${MOVE_MEMORY_MANAGER_SOURCES})
    target_compile_definitions(move-mm PRIVATE -DMOVEMM_EXPORTING=1)
else()
    add_library(move-mm STATIC ${MOVE_MEMORY_MANAGER_SOURCES})
    target_compile_definitions(move-mm PUBLIC -DMOVEMM_STATIC=1)
endif()
# set_target_properties(move-mm PROPERTIES UNITY_BUILD ON)

target_link_libraries(move-mm PUBLIC rpmalloc)
target_include_directories(move-mm PUBLIC "include")
# target_precompile_headers(move-mm PUBLIC include/pch.hpp)

# TODO: Set up tests

option(MOVE_MEMORY_MANAGER_WITH_TESTS "Determines whether or not to build the test suite" off)
if (MOVE_MEMORY_MANAGER_WITH_TESTS)
    add_subdirectory(tests)
endif()