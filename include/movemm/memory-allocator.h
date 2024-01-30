#pragma once

#include <stddef.h>
#include <stdint.h>
#include "export.h"

// Basic allocation functions
MOVEMM_EXPORT void* movemm_alloc(size_t bytes);
MOVEMM_EXPORT void movemm_free(void* ptr);

// Separate heaps.  These can only be freed in their entirety - not individual
// allocations.
typedef void* movemm_heap_t;
MOVEMM_EXPORT movemm_heap_t movemm_create_heap();
MOVEMM_EXPORT void movemm_destroy_heap(movemm_heap_t);

MOVEMM_EXPORT void* movemm_heap_alloc(movemm_heap_t heap, size_t bytes);

// Temporary allocations
typedef struct
{
    uint64_t tag;
} movemm_heap_tag_t;

MOVEMM_EXPORT void* movemm_tagged_heap_alloc(
    movemm_heap_tag_t tag, size_t bytes);

typedef void (*movemm_destructor_cb_t)(void*);
MOVEMM_EXPORT void movemm_register_tagged_heap_destructor(
    movemm_heap_tag_t tag, void* ptr, movemm_destructor_cb_t destructor);

MOVEMM_EXPORT void movemm_tagged_heap_free(movemm_heap_tag_t tag);

MOVEMM_EXPORT size_t movemm_tagged_heap_get_current_storage();

MOVEMM_EXPORT size_t movemm_tagged_heap_get_current_tag_storage(
    movemm_heap_tag_t tag);

#if defined(MOVEMM_WINDOWS)
#define movemm_stack_alloc(bytes) _alloca(bytes)
#elif defined(MOVEMM_UNIX)
#define movemm_stack_alloc(bytes) alloca(bytes)
#endif

#ifdef __cplusplus
#include <utility>
namespace movemm
{
    template <typename T, typename... Args>
    inline T* mmnew(Args&&... args)
    {
        return new (movemm_alloc(sizeof(T))) T(std::forward<Args>(args)...);
    }

    template <typename T>
    inline void mmdelete(T* ptr)
    {
        ptr->~T();
        movemm_free(ptr);
    }

    inline void* alloc(size_t bytes)
    {
        return movemm_alloc(bytes);
    }

    inline void free(void* ptr)
    {
        movemm_free(ptr);
    }

    inline void* tagged_alloc(movemm_heap_tag_t tag, size_t bytes)
    {
        return movemm_tagged_heap_alloc(tag, bytes);
    }

    inline void tagged_free(movemm_heap_tag_t tag)
    {
        movemm_tagged_heap_free(tag);
    }
}  // namespace movemm
#endif