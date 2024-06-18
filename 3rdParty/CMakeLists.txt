include(cpm/cmake/cpm.cmake)

CPMAddPackage("gh:microsoft/mimalloc#03020fbf81541651e24289d2f7033a772a50f480")

# CPMAddPackage(
#     NAME rpmalloc
#     GIT_REPOSITORY "https://github.com/mjansson/rpmalloc.git"
#     GIT_TAG "1.4.4"
#     DOWNLOAD_ONLY YES
# )

# if (rpmalloc_ADDED)
#     add_library(rpmalloc STATIC ${rpmalloc_SOURCE_DIR}/rpmalloc/rpmalloc.c)
#     target_include_directories(rpmalloc PUBLIC "${rpmalloc_SOURCE_DIR}")
#     target_compile_definitions(rpmalloc PUBLIC -DENABLE_STATISTICS=1 -DRPMALLOC_FIRST_CLASS_HEAPS=1)
# else()
#     message(FATAL_ERROR "Failed to acquire rpmalloc")
# endif()