#pragma once

#include <type_traits>
#include <utility>

#include "memory-allocator.h"

namespace movemm
{
    template <typename T>
    class unique_ptr
    {
    public:
        typedef void (*deallocate_fn_t)(T*);

    public:
        inline unique_ptr() : _data(0)
        {
        }

        inline unique_ptr(T* ptrToManage) : _data(ptrToManage)
        {
        }

        inline unique_ptr(const unique_ptr<T>& rhs) = delete;

        inline unique_ptr(unique_ptr<T>&& rhs) : _data(rhs._data)
        {
            // static_assert(
            //     std::is_same_v<T, R> || std::is_assignable_v<T, R>,
            //     "R must be assignable to T");

            rhs._data = 0;
        }

        inline ~unique_ptr()
        {
            destroy();
        }

    public:
        template <typename... Args>
        inline void create(Args&&... args)
        {
            destroy();
            _data = movemm::mmnew<T>(std::forward<Args>(args)...);
        }

        inline bool destroy()
        {
            if (_data)
            {
                mmdelete(_data);
                return true;
            }
            return false;
        }

        inline T* get()
        {
            return _data;
        }

        inline T const* get() const
        {
            return _data;
        }

    public:
        inline T* operator->()
        {
            return _data;
        }

        inline T const* operator->() const
        {
            return _data;
        }

        inline T& operator*()
        {
            return *_data;
        }

        inline const T& operator*() const
        {
            return *_data;
        }

        inline operator bool() const
        {
            return _data;
        }

    private:
        T* _data;
    };

    template <typename T, typename... Args>
    inline unique_ptr<T> make_unique(Args&&... args)
    {
        return unique_ptr<T>(movemm::mmnew<T>(std::forward<Args>(args)...));
    }

    template <typename T>
    class weak_ptr
    {
    public:
        inline weak_ptr() : _data(0)
        {
        }

        inline weak_ptr(T* ptrToManage) : _data(ptrToManage)
        {
        }

        inline weak_ptr(unique_ptr<T>& ptrToManage) : _data(ptrToManage.get())
        {
        }

        inline weak_ptr(const weak_ptr<T>& rhs) = default;

    public:
        inline void set(T* value)
        {
            _data = value;
        }

        inline T* get()
        {
            return _data;
        }

        inline T const* get() const
        {
            return _data;
        }

    public:
        inline T* operator->()
        {
            return _data;
        }

        inline T const* operator->() const
        {
            return _data;
        }

        inline T& operator*()
        {
            return *_data;
        }

        inline const T& operator*() const
        {
            return *_data;
        }

        inline operator bool() const
        {
            return _data;
        }

    private:
        T* _data;
    };
}  // namespace movemm
