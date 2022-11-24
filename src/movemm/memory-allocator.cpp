#include <movemm/memory-allocator.h>
#include <cstring>

#define RPMALLOC_FIRST_CLASS_HEAPS 1

#include <rpmalloc/rpmalloc.h>

struct rpmalloc_thread_instance
{
    inline rpmalloc_thread_instance()
    {
        // spdlog::info("Initializing rpmalloc for this thread");
        rpmalloc_thread_initialize();
    }

    ~rpmalloc_thread_instance()
    {
        // spdlog::info("Finalizing rpmalloc for this thread");
        rpmalloc_thread_finalize(0);
    }

    inline void noop()
    {
    }

    bool has_initialized = false;
};

struct heap_initializer
{
    heap_initializer();
    ~heap_initializer();

    inline void noop()
    {
    }
};

heap_initializer::heap_initializer()
{
    // spdlog::info("Initializing rpmalloc");
    rpmalloc_config_t cfg = {0};
    cfg.enable_huge_pages = 1;
    cfg.page_name = "MMM_P";
    cfg.huge_page_name = "MMM_HP";
    rpmalloc_initialize_config(&cfg);
}

heap_initializer::~heap_initializer()
{
    // spdlog::info("Initializing rpmalloc");
    rpmalloc_finalize();
}

static heap_initializer _heapInit;
static thread_local rpmalloc_thread_instance _heapThreadInfo;

MOVEMM_EXPORT void* movemm_alloc(size_t bytes)
{
    // Also ensuring that the execution order of any static objects is valid
    _heapInit.noop();

    // Just ensuring that the thread local object has been initiailzed (and
    // therefore the rpmalloc thread local data has been initialized)
    _heapThreadInfo.noop();

    return rpmalloc(bytes);
}

MOVEMM_EXPORT void movemm_free(void* memory)
{
    // Also ensuring that the execution order of any static objects is valid
    _heapInit.noop();

    // Just ensuring that the thread local object has been initiailzed (and
    // therefore the rpmalloc thread local data has been initialized)
    _heapThreadInfo.noop();

    rpfree(memory);
}

MOVEMM_EXPORT void movemm_thread_collect()
{
    rpmalloc_thread_collect();
}

MOVEMM_EXPORT void movemm_get_statistics(movemm_statistics_t* statistics)
{
    static_assert(
        sizeof(rpmalloc_global_statistics_t) == sizeof(movemm_statistics_t),
        "rpmalloc statistics size mismatch with movemm statistics size");

    rpmalloc_global_statistics_t stats;
    rpmalloc_global_statistics(&stats);

    memcpy(statistics, &stats, sizeof(movemm_statistics_t));
}

MOVEMM_EXPORT movemm_heap_t movemm_create_heap()
{
    // Also ensuring that the execution order of any static objects is valid
    _heapInit.noop();

    // Just ensuring that the thread local object has been initiailzed (and
    // therefore the rpmalloc thread local data has been initialized)
    _heapThreadInfo.noop();

    return rpmalloc_heap_acquire();
}

MOVEMM_EXPORT void movemm_destroy_heap(movemm_heap_t heap)
{
    // Also ensuring that the execution order of any static objects is valid
    _heapInit.noop();

    // Just ensuring that the thread local object has been initiailzed (and
    // therefore the rpmalloc thread local data has been initialized)
    _heapThreadInfo.noop();

    rpmalloc_heap_release((rpmalloc_heap_t*)heap);
}

MOVEMM_EXPORT void* movemm_heap_alloc(movemm_heap_t heap, size_t bytes)
{
    // Also ensuring that the execution order of any static objects is valid
    _heapInit.noop();

    // Just ensuring that the thread local object has been initiailzed (and
    // therefore the rpmalloc thread local data has been initialized)
    _heapThreadInfo.noop();

    return rpmalloc_heap_alloc((rpmalloc_heap_t*)heap, bytes);
}

MOVEMM_EXPORT void movemm_heap_free(movemm_heap_t heap, void* ptr)
{
    // Also ensuring that the execution order of any static objects is valid
    _heapInit.noop();

    // Just ensuring that the thread local object has been initiailzed (and
    // therefore the rpmalloc thread local data has been initialized)
    _heapThreadInfo.noop();

    rpmalloc_heap_free((rpmalloc_heap_t*)heap, ptr);
}