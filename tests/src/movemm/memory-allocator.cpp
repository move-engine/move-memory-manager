
#include <catch2/catch.hpp>

#include <movemm/memory-allocator.h>

SCENARIO("Testing the memory allocator API")
{
    // TODO: Write tests for the memory allocator
}

#if defined(MOVE_ENABLE_TAGGED_HEAP)
SCENARIO("Testing tagged heap")
{
    GIVEN("A tagged heap tag and an ")
    {
        movemm_heap_tag_t tag = {100};

        WHEN("An allocation is made")
        {
        }
    }
}
#endif