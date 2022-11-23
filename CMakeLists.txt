cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 17)
project(
    move-memory-manager
    LANGUAGES C CXX
    VERSION 0.1.2
)

if (MOVE_MEMORY_MANAGER_CI)
    message(STATUS "Writing package version to ${CMAKE_CURRENT_SOURCE_DIR}/package_version.txt")
    write_file("${CMAKE_CURRENT_SOURCE_DIR}/package_version.txt" "v${CMAKE_PROJECT_VERSION}")
endif()

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

target_link_libraries(move-mm PUBLIC rpmalloc)
target_include_directories(move-mm PUBLIC "include")

option(MOVE_MEMORY_MANAGER_WITH_TESTS "Determines whether or not to build the test suite" off)
if (MOVE_MEMORY_MANAGER_WITH_TESTS)
    add_subdirectory(tests)
endif()