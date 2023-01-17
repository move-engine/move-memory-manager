#include <movemm/memory-allocator.h>
#include <cstring>
#include <mutex>
#include <unordered_set>

#include <mimalloc.h>

#if defined(MOVEMM_TRACKING_MODE)
static std::mutex _globalMutex;
static std::unordered_set<void*> _allocations;
#endif

MOVEMM_EXPORT void* movemm_alloc(size_t bytes)
{
#if defined(MOVEMM_TRACKING_MODE)
    std::lock_guard<std::mutex> lock(_globalMutex);
#endif
    auto res = mi_malloc(bytes);
#if defined(MOVEMM_TRACKING_MODE)
    _allocations.insert(res);
#endif
    return res;
}

MOVEMM_EXPORT void movemm_free(void* memory)
{
    if (!memory) return;

#if defined(MOVEMM_TRACKING_MODE)
    std::lock_guard<std::mutex> lock(_globalMutex);
    auto it = _allocations.find(memory);
    if (it == _allocations.end())
    {
        throw std::runtime_error(
            "Attempted to free memory that was not allocated by movemm");
    }
    _allocations.erase(it);
#endif
    mi_free(memory);
}

// MOVEMM_EXPORT void movemm_get_statistics(movemm_statistics_t* statistics)
// {
//     static_assert(
//         sizeof(mimalloc) == sizeof(movemm_statistics_t),
//         "rpmalloc statistics size mismatch with movemm statistics size");

//     rpmalloc_global_statistics_t stats;
//     rpmalloc_global_statistics(&stats);

//     memcpy(statistics, &stats, sizeof(movemm_statistics_t));
// }

MOVEMM_EXPORT movemm_heap_t movemm_create_heap()
{
    return mi_heap_new();
}

MOVEMM_EXPORT void movemm_destroy_heap(movemm_heap_t heap)
{
    mi_heap_destroy((mi_heap_t*)(heap));
}

MOVEMM_EXPORT void* movemm_heap_alloc(movemm_heap_t heap, size_t bytes)
{
    return mi_heap_alloc_new((mi_heap_t*)heap, bytes);
}

// MOVEMM_EXPORT void movemm_heap_free(movemm_heap_t heap, void* ptr)
// {
//     mi_heap_((mi_heap_t*)heap, ptr);
// }