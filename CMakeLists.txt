cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 17)
project(
    move-memory-manager
    LANGUAGES C CXX
    VERSION 0.4.0
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


if (WIN32)
    target_compile_definitions(move-mm PUBLIC -DMOVEMM_WINDOWS=1)
elseif (UNIX)
target_compile_definitions(move-mm PUBLIC -DMOVEMM_UNIX=1)
else()
    message(FATAL_ERROR "OS is not supported")
endif()

target_link_libraries(move-mm PUBLIC mimalloc-static)
target_include_directories(move-mm PUBLIC "include")

option(MOVE_MEMORY_MANAGER_TRACKING_MODE "Adds a global allocator lock and tracks all allocations, validating all calls to movemm_free.  Tracking mode will be SIGNIFICANTLY slower than non-tracking mode." off)
option(MOVE_MEMORY_MANAGER_WITH_TESTS "Determines whether or not to build the test suite" off)

if (MOVE_MEMORY_MANAGER_TRACKING_MODE)
    target_compile_definitions(move-mm PUBLIC -DMOVEMM_TRACKING_MODE=1)
endif()

if (MOVE_MEMORY_MANAGER_WITH_TESTS)
    add_subdirectory(tests)
endif()