#include <movemm/memory-allocator.h>

#if defined(MOVE_ENABLE_TAGGED_HEAP)
#include <functional>
#include <iostream>
#include <mutex>
#include <unordered_map>
#include <vector>

#include <movemm/stl_allocator.hpp>

template <typename T>
using vec = std::vector<T, movemm::stl_allocator<T>>;

template <typename K, typename V>
using umap = std::unordered_map<K, V, std::hash<K>, std::equal_to<K>,
    movemm::stl_allocator<std::pair<const K, V>>>;

class tagged_heap_global;

struct registered_tagged_heap_destructor
{
    registered_tagged_heap_destructor(
        void* ptr, movemm_destructor_cb_t destructor)
        : ptr(ptr), destructor(destructor)
    {
    }

    ~registered_tagged_heap_destructor()
    {
        destructor(ptr);
    }

    void* ptr;
    movemm_destructor_cb_t destructor;
};

struct tagged_heap_page
{
    void* allocate(size_t bytes)
    {
        // Don't allow it to allocate beyond the page boundary
        if (nextOffset + bytes >= allocationSize) return 0;

        auto ptr = buffer + nextOffset;

        // Always 8 byte aligned
        auto mod = bytes % 8;
        nextOffset += mod ? 8 - mod : 0;

        return ptr;
    }

    size_t nextOffset;
    size_t allocationSize;
    char buffer[];
};

constexpr size_t tagged_heap_page_size = 2 * 1024 * 1024;
constexpr size_t compute_required_page_size_for_alloc(size_t alloc)
{
    // We store the tagged_heap_page at the head of the page, so we need to
    // ensure that it fits.
    return (((sizeof(tagged_heap_page) + alloc) / tagged_heap_page_size) + 1) *
           tagged_heap_page_size;
}

// Each temp page is 1MB
struct tagged_heap_tag_storage
{
    ~tagged_heap_tag_storage()
    {
        for (auto& it : _pages)
        {
            movemm_free(it);
        }
    }

    inline void* allocate(size_t bytes)
    {
        void* res = 0;

        while (!res)
        {
            if (_nextPage >= _pages.size())
            {
                auto allocSize = compute_required_page_size_for_alloc(bytes);
                // Allocate the next page
                auto ptr = reinterpret_cast<tagged_heap_page*>(
                    movemm_alloc(allocSize));

                // Initialize the page
                auto pg = new (ptr) tagged_heap_page();
                pg->nextOffset = 0;
                pg->allocationSize = allocSize;
                _pages.push_back(ptr);
            }

            auto& tgPage = _pages[_nextPage];
            res = tgPage->allocate(bytes);

            if (!res) ++_nextPage;
        }
        return res;
    }

    size_t total_allocated()
    {
        size_t res = 0;
        for (auto& it : _pages)
        {
            if (it) res += it->allocationSize;
        }
        return res;
    }

private:
    vec<tagged_heap_page*> _pages;
    size_t _nextPage;
};

namespace std
{
    template <>
    struct hash<movemm_heap_tag_t>
    {
        typedef movemm_heap_tag_t argument_type;
        typedef std::size_t result_type;

        inline result_type operator()(const argument_type& a) const noexcept
        {
            return result_type(a.tag);
        }
    };

    template <>
    struct equal_to<movemm_heap_tag_t>
    {
        constexpr bool operator()(
            const movemm_heap_tag_t& a, const movemm_heap_tag_t& b) const
        {
            return a.tag == b.tag;
        }
    };
}  // namespace std

struct registered_tagged_heap_destructor_set
{
    ~registered_tagged_heap_destructor_set()
    {
        // Destruy elements back-to-front
        while (!_vec.empty())
        {
            _vec.pop_back();
        }
    }

    vec<registered_tagged_heap_destructor> _vec;
};

class tagged_heap_tls
{
public:
    tagged_heap_tls(tagged_heap_global& parent);
    ~tagged_heap_tls();

public:
    void* allocate(movemm_heap_tag_t tag, size_t bytes)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        auto& storage = get_or_create_unsafe(tag);
        return storage.allocate(bytes);
    }

    void free_tag(movemm_heap_tag_t tag)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_tagStorage.count(tag))
        {
            _tagStorage.erase(tag);
        }
    }

    size_t total_cache_size()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        size_t res = 0;
        for (auto& storage : _tagStorage)
        {
            res += storage.second.total_allocated();
        }
        return res;
    }

    size_t tag_cache_size(movemm_heap_tag_t tag)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_tagStorage.count(tag))
        {
            return _tagStorage[tag].total_allocated();
        }
        return 0;
    }

private:
    inline tagged_heap_tag_storage& get_or_create_unsafe(movemm_heap_tag_t tag)
    {
        if (!_tagStorage.count(tag))
        {
            _tagStorage.insert({tag, {}});
        }
        return _tagStorage.at(tag);
    }

private:
    umap<movemm_heap_tag_t, tagged_heap_tag_storage> _tagStorage;
    std::mutex _mutex;
    tagged_heap_global* _parent;
};

class tagged_heap_global
{
public:
    void register_tls(tagged_heap_tls* tls);
    void deregister_tls(tagged_heap_tls* tls);

    size_t get_current_storage()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        size_t res = 0;
        for (auto& it : _threadLocal)
        {
            res += it->total_cache_size();
        }
        return res;
    }

    size_t get_current_tag_storage(movemm_heap_tag_t tag)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        size_t res = 0;
        for (auto& it : _threadLocal)
        {
            res += it->tag_cache_size(tag);
        }
        return res;
    }

public:
    void register_destructor(
        movemm_heap_tag_t tag, void* ptr, movemm_destructor_cb_t destructor)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (!_destructor_sets.count(tag))
        {
            _destructor_sets.insert({tag, {}});
        }

        auto& destructors = _destructor_sets[tag];
        destructors._vec.push_back(
            registered_tagged_heap_destructor(ptr, destructor));
    }

    void free_tag(movemm_heap_tag_t tag)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_destructor_sets.count(tag))
        {
            // Destroys elements in reverse insertion order due to the type's
            // destructor
            _destructor_sets.erase(tag);
        }

        // Clears out any thread local caches
        for (auto& it : _threadLocal)
        {
            it->free_tag(tag);
        }
    }

private:
    std::mutex _mutex;
    vec<tagged_heap_tls*> _threadLocal;
    umap<movemm_heap_tag_t, registered_tagged_heap_destructor_set>
        _destructor_sets;
};

tagged_heap_tls::tagged_heap_tls(tagged_heap_global& parent) : _parent(&parent)
{
    parent.register_tls(this);
}

tagged_heap_tls::~tagged_heap_tls()
{
    if (_parent)
    {
        _parent->deregister_tls(this);
    }
}

void tagged_heap_global::register_tls(tagged_heap_tls* tls)
{
    std::unique_lock<std::mutex> lock(_mutex);
    _threadLocal.push_back(tls);
}

void tagged_heap_global::deregister_tls(tagged_heap_tls* tls)
{
    std::unique_lock<std::mutex> lock(_mutex);
    for (auto it = _threadLocal.begin(); it != _threadLocal.end(); ++it)
    {
        if (*it == tls)
        {
            _threadLocal.erase(it);
            return;
        }
    }

    throw std::runtime_error(
        "Failed to deregister TLS for temporary allocator");
}

static tagged_heap_global s_TempHeap;
struct temp_tls_container
{
    temp_tls_container() : tls(s_TempHeap)
    {
    }

    tagged_heap_tls tls;
};

thread_local temp_tls_container tls_container;

MOVEMM_EXPORT void* movemm_tagged_heap_alloc(
    movemm_heap_tag_t tag, size_t bytes)
{
    return tls_container.tls.allocate(tag, bytes);
}

MOVEMM_EXPORT void movemm_register_destructor(
    movemm_heap_tag_t tag, void* ptr, movemm_destructor_cb_t destructor)
{
    s_TempHeap.register_destructor(tag, ptr, destructor);
}

MOVEMM_EXPORT void movemm_tagged_heap_free(movemm_heap_tag_t tag)
{
    return s_TempHeap.free_tag(tag);
}

MOVEMM_EXPORT size_t movemm_tagged_heap_get_current_storage()
{
    return s_TempHeap.get_current_storage();
}

MOVEMM_EXPORT size_t movemm_tagged_heap_get_current_tag_storage(
    movemm_heap_tag_t tag)
{
    return s_TempHeap.get_current_tag_storage(tag);
}
#endif