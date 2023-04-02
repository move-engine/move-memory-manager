
#include <catch2/catch_all.hpp>

#include <movemm/ptr_types.hpp>

SCENARIO("Testing pointer types")
{
    GIVEN("A unique pointer for an int created with make_unique")
    {
        auto ptr = movemm::make_unique<int>();
        THEN("The pointer is not null")
        {
            REQUIRE(ptr);
            REQUIRE(ptr.valid());
            REQUIRE(ptr.get());
        }
        AND_WHEN("The pointer is destroyed")
        {
            ptr.destroy();
            THEN("The pointer is null")
            {
                REQUIRE(!ptr);
                REQUIRE(!ptr.valid());
                REQUIRE(!ptr.get());
            }
        }
    }

    GIVEN("A shared pointer for an int created with make_shared")
    {
        auto ptr = movemm::make_shared<int>();
        THEN("The pointer is not null and the refcount is 1")
        {
            REQUIRE(ptr);
            REQUIRE(ptr.valid());
            REQUIRE(ptr.get());
            REQUIRE(ptr.refcount() == 1);
        }

        AND_WHEN("The pointer is destroyed")
        {
            ptr.destroy();
            THEN("The pointer is null")
            {
                REQUIRE(!ptr);
                REQUIRE(!ptr.valid());
                REQUIRE(!ptr.get());
            }
        }

        AND_WHEN("The pointer is copied")
        {
            auto ptr2 = ptr;
            THEN("The pointer is not null and the refcount is 2")
            {
                REQUIRE(ptr2);
                REQUIRE(ptr2.valid());
                REQUIRE(ptr2.get());
                REQUIRE(ptr2.refcount() == 2);
            }

            AND_WHEN("The pointer is destroyed")
            {
                ptr2.destroy();
                THEN("The pointer is null")
                {
                    REQUIRE(!ptr2);
                    REQUIRE(!ptr2.valid());
                    REQUIRE(!ptr2.get());
                }
            }
        }
    }

    GIVEN(
        "A shared pointer to a more complex data structure with make_shared "
        "and a verifiable constructor/destructor")
    {
        int _count = 0;
        struct Test
        {
            Test(int& count) : _count(count)
            {
                ++_count;
            }

            Test(const Test& rhs) = delete;
            Test(Test&& rhs) = delete;

            ~Test()
            {
                --_count;
            }

            int& _count;
        };

        REQUIRE(_count == 0);

        auto ptr = movemm::make_shared<Test>(_count);
        REQUIRE(_count == 1);

        THEN("The pointer is not null and the refcount is 1")
        {
            REQUIRE(ptr);
            REQUIRE(ptr.valid());
            REQUIRE(ptr.get());
            REQUIRE(ptr.refcount() == 1);
        }

        AND_WHEN("The pointer is destroyed")
        {
            ptr.destroy();
            THEN("The pointer is null and count is 0")
            {
                REQUIRE(!ptr);
                REQUIRE(!ptr.valid());
                REQUIRE(!ptr.get());
                REQUIRE(_count == 0);
            }
        }

        AND_WHEN("The pointer is copied")
        {
            auto ptr2 = ptr;
            THEN("The pointer is not null, the refcount is 2, and count is 1")
            {
                REQUIRE(ptr2);
                REQUIRE(ptr2.valid());
                REQUIRE(ptr2.get());
                REQUIRE(ptr2.refcount() == 2);
                REQUIRE(_count == 1);
            }

            AND_WHEN("The pointer is destroyed")
            {
                ptr2.destroy();
                THEN("The pointer is null and count is 1")
                {
                    REQUIRE(!ptr2);
                    REQUIRE(!ptr2.valid());
                    REQUIRE(!ptr2.get());
                    REQUIRE(_count == 1);
                }
            }
        }

        AND_WHEN("The pointer is copied and then moved")
        {
            auto ptr2 = ptr;
            auto ptr3 = std::move(ptr2);
            THEN("The pointer is not null, the refcount is 2, and count is 1")
            {
                REQUIRE(ptr3);
                REQUIRE(ptr3.valid());
                REQUIRE(ptr3.get());
                REQUIRE(ptr3.refcount() == 2);
                REQUIRE(_count == 1);
            }

            AND_WHEN("The pointer is destroyed")
            {
                ptr3.destroy();
                THEN("The pointer is null and count is 1")
                {
                    REQUIRE(!ptr3);
                    REQUIRE(!ptr3.valid());
                    REQUIRE(!ptr3.get());
                    REQUIRE(_count == 1);
                }
            }
        }

        AND_WHEN("The pointer is moved")
        {
            auto ptr2 = std::move(ptr);
            THEN("The pointer is not null, the refcount is 1, and count is 1")
            {
                REQUIRE(ptr2);
                REQUIRE(ptr2.valid());
                REQUIRE(ptr2.get());
                REQUIRE(ptr2.refcount() == 1);
                REQUIRE(_count == 1);

                AND_WHEN("The pointer is destroyed")
                {
                    ptr2.destroy();
                    THEN("The pointer is null and count is 0")
                    {
                        REQUIRE(!ptr2);
                        REQUIRE(!ptr2.valid());
                        REQUIRE(!ptr2.get());
                        REQUIRE(_count == 0);
                    }
                }
            }
        }
    }
}