#include <cstddef>
#include <cstring>

#include <catch2/catch.hpp>

#include <movemm/memory-allocator.h>
#include <string.h>

// SCENARIO("Testing the C allocator API")
// {
//     GIVEN("An empty stack array of pointers")
//     {
//         void* generated[100];
//         memset(generated, 0, sizeof(generated));

//         movemm_statistics_t start_stats;
//         movemm_get_statistics(&start_stats);

//         INFO("Total memory mapped = " << start_stats.mapped);

//         WHEN("100 allocations are made")
//         {
//             for (size_t i = 0; i < sizeof(generated) / sizeof(generated[0]);
//                  ++i)
//             {
//                 generated[i] = movemm_alloc(1024 * 1024);
//             }

//             THEN("All allocations should be valid")
//             {
//                 for (size_t i = 0; i < sizeof(generated) /
//                 sizeof(generated[0]);
//                      ++i)
//                 {
//                     REQUIRE(generated[i] != 0);
//                 }
//             }

//             AND_THEN(
//                 "The amount of mapped memory should be higher than it was at
//                 " "the start")
//             {
//                 movemm_statistics_t cur_stats;
//                 movemm_get_statistics(&cur_stats);

//                 REQUIRE(cur_stats.mapped > start_stats.mapped);
//             }

//             AND_WHEN("All of the allocations are freed")
//             {
//                 for (size_t i = 0; i < sizeof(generated) /
//                 sizeof(generated[0]);
//                      ++i)
//                 {
//                     REQUIRE_NOTHROW(movemm_free(generated[i]));
//                 }
//             }
//         }
//     }
// }

// SCENARIO("Testing stack allocations")
// {
//     GIVEN("A zero'd out stack allocated string")
//     {
//         constexpr size_t alloc = 64;
//         char* data = (char*)movemm_stack_alloc(alloc);
//         memset(data, 0, alloc);

//         WHEN("Operations on that string should be valid")
//         {
//             const static char* tstr = "This is a test";
//             REQUIRE_NOTHROW(strcpy(data, tstr));
//             REQUIRE(!strcmp(data, tstr));
//         }
//     }
// }