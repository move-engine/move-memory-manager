#pragma once

#include <stddef.h>
#include <stdint.h>
#include "export.h"

// Basic allocation functions
MOVEMM_EXPORT void* movemm_alloc(size_t bytes);
MOVEMM_EXPORT void movemm_free(void* ptr);

typedef struct
{
    // Current amount of virtual memory mapped, all of which might not have been
    // committed (only if ENABLE_STATISTICS=1)
    size_t mapped;

    // Peak amount of virtual memory mapped, all of which might not have been
    // committed (only if ENABLE_STATISTICS=1)
    size_t mapped_peak;

    // Current amount of memory in global caches for small and medium sizes
    // (<32KiB)
    size_t cached;

    // Current amount of memory allocated in huge allocations, i.e larger than
    // LARGE_SIZE_LIMIT which is 2MiB by default (only if ENABLE_STATISTICS=1)
    size_t huge_alloc;

    // Peak amount of memory allocated in huge allocations, i.e larger than
    // LARGE_SIZE_LIMIT which is 2MiB by default (only if ENABLE_STATISTICS=1)
    size_t huge_alloc_peak;

    // Total amount of memory mapped since initialization (only if
    // ENABLE_STATISTICS=1)
    size_t mapped_total;

    // Total amount of memory unmapped since initialization  (only if
    // ENABLE_STATISTICS=1)
    size_t unmapped_total;
} movemm_statistics_t;

MOVEMM_EXPORT void movemm_get_statistics(movemm_statistics_t* statistics);

// Separate heaps.  Only safe to use from one thread at a time.
typedef void* movemm_heap_t;
MOVEMM_EXPORT movemm_heap_t movemm_create_heap();
MOVEMM_EXPORT void movemm_destroy_heap(movemm_heap_t);

MOVEMM_EXPORT void* movemm_heap_alloc(movemm_heap_t heap, size_t bytes);
MOVEMM_EXPORT void movemm_heap_free(movemm_heap_t heap, void* ptr);

// Temporary allocations
typedef struct
{
    uint64_t tag;
} movemm_temp_tag_t;

#define MOVE_ENABLE_TAGGED_HEAP

#if defined(MOVE_ENABLE_TAGGED_HEAP)
MOVEMM_EXPORT void* movemm_tagged_heap_alloc(
    movemm_temp_tag_t tag, size_t bytes);

// TODO: Implement destructors
typedef void (*movemm_destructor_cb_t)(void*);
MOVEMM_EXPORT void movemm_register_destructor(
    movemm_temp_tag_t tag, void* ptr, movemm_destructor_cb_t destructor);

MOVEMM_EXPORT void movemm_tagged_heap_free(movemm_temp_tag_t tag);
#endif