
#include <catch2/catch.hpp>

#include <movemm/ptr_types.hpp>

// SCENARIO("Testing pointer types")
// {
//     GIVEN("A unique pointer created with make_unique")
//     {
//         auto ptr = movemm::make_unique<int>();
//         THEN("The pointer is not null")
//         {
//             REQUIRE(ptr);
//             REQUIRE(ptr.valid());
//             REQUIRE(ptr.get());
//         }
//         AND_WHEN("The pointer is destroyed")
//         {
//             ptr.destroy();
//             THEN("The pointer is null")
//             {
//                 REQUIRE(!ptr);
//                 REQUIRE(!ptr.valid());
//                 REQUIRE(!ptr.get());
//             }
//         }
//     }
// }