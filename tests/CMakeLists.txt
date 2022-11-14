file(
    GLOB_RECURSE TEST_SUITE_SOURCES
    "src/*.cpp"
)

file(
    GLOB_RECURSE TEST_SUITE_HEADERS
    "include/*.hpp"
    "include/*.h"
)

add_executable(move-mm-tests ${TEST_SUITE_SOURCES} ${TEST_SUITE_HEADERS})
target_include_directories(move-mm-tests PUBLIC ${MOVE_INCLUDES})

if (Catch2_ADDED)
    message(STATUS Using existing catch2 library)
else()
    CPMAddPackage("gh:catchorg/Catch2@2.13.7")
endif()

target_link_libraries(move-mm-tests PUBLIC Catch2 move-mm)