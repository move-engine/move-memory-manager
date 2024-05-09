#pragma once
#include <utility>

#include "memory-allocator.h"

namespace movemm
{
    template <typename T>
    class stl_allocator
    {
    public:
        using value_type = T;
        using reference = T&;
        using const_reference = T const&;
        using pointer = T*;
        using const_pointer = T const*;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        inline constexpr stl_allocator() noexcept
        {
        }

        template <class U>
        stl_allocator(stl_allocator<U> const&) noexcept
        {
        }

        inline constexpr stl_allocator(const stl_allocator&) noexcept
        {
        }

    public:
        inline T* allocate(size_t count)
        {
            return static_cast<T*>(movemm_alloc(count * sizeof(T)));
        }

        inline void deallocate(T* p, size_t count)
        {
            return movemm_free(p);
        }

        template <class U, class... Args>
        void construct(U* p, Args&&... args)
        {
            ::new (p) U(std::forward<Args>(args)...);
        }

        template <class U>
        void destroy(U* p) noexcept
        {
            p->~U();
        }
    };

    template <class T, class U>
    bool operator==(stl_allocator<T> const&, stl_allocator<U> const&) noexcept
    {
        return true;
    }

    template <class T, class U>
    bool operator!=(
        stl_allocator<T> const& x, stl_allocator<U> const& y) noexcept
    {
        return !(x == y);
    }
}  // namespace movemm