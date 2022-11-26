// #include <catch2/catch_all.hpp>

// #include <movemm/memory-allocator.h>

// #if defined(MOVE_ENABLE_TAGGED_HEAP)
// SCENARIO("Testing tagged heap")
// {
//     GIVEN("A tagged heap tag")
//     {
//         movemm_heap_tag_t tag = {100};

//         WHEN(
//             "An allocation is made, it should increase the storage size; when
//             " "the tag is freed, it should reset")
//         {
//             size_t totalSizePreAlloc =
//             movemm_tagged_heap_get_current_storage(); size_t tagSizePreAlloc
//             =
//                 movemm_tagged_heap_get_current_tag_storage(tag);

//             auto alloc = movemm_tagged_heap_alloc(tag, 512);

//             REQUIRE(alloc != 0);
//             REQUIRE(
//                 movemm_tagged_heap_get_current_storage() >
//                 totalSizePreAlloc);

//             REQUIRE(movemm_tagged_heap_get_current_tag_storage(tag) >
//                     tagSizePreAlloc);

//             REQUIRE_NOTHROW(movemm_tagged_heap_free(tag));

//             REQUIRE(
//                 movemm_tagged_heap_get_current_storage() ==
//                 totalSizePreAlloc);

//             REQUIRE(movemm_tagged_heap_get_current_tag_storage(tag) ==
//                     tagSizePreAlloc);
//         }
//     }
// }
// #endif