include(cpm/cmake/cpm.cmake)

# Using this fork/branch from https://github.com/microsoft/mimalloc/pull/673 until the fix
# is merged for Windows.
CPMAddPackage("gh:Unsttopabull/mimalloc#5b241ade81984c43335d229f8b4d8486683139b2")

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