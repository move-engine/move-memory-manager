
#include <catch2/catch.hpp>

#include <movemm/ptr_types.hpp>

SCENARIO("Testing pointer types")
{
    GIVEN("A unique pointer created with make_unique")
    {
        auto ptr = movemm::make_unique<int>();
        WHEN("The pointer is ")
        {
            THEN("The pointer is not null")
            {
                CHECK(ptr.get());
            }
        }
    }
}