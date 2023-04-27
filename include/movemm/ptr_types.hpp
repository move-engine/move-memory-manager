#pragma once
#include <atomic>
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
                _data = 0;
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

        inline bool valid() const
        {
            return _data;
        }

    public:
        inline operator bool() const
        {
            return valid();
        }

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

        inline unique_ptr<T>& operator=(const unique_ptr<T>& rhs) = delete;
        inline unique_ptr<T>& operator=(unique_ptr<T>&& rhs)
        {
            destroy();
            _data = rhs._data;
            rhs._data = 0;
            return *this;
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

    template <typename T>
    class shared_ptr
    {
        struct refcounted_data
        {
            template <typename... Args>
            inline refcounted_data(Args&&... args)
                : value(std::forward<Args>(args)...), refcount(1)
            {
            }

            inline ~refcounted_data()
            {
            }

            T value;
            std::atomic_size_t refcount;
        };

    public:
        // Creates an invalid shared_ptr
        inline shared_ptr() : _data(0)
        {
        }

        inline shared_ptr(shared_ptr<T>& rhs) : _data(rhs._data)
        {
            if (_data)
            {
                ++_data->refcount;
            }
        }

        inline shared_ptr(shared_ptr<T>&& rhs) : _data(rhs._data)
        {
            rhs._data = 0;
        }

        inline ~shared_ptr()
        {
            destroy();
        }

    public:
        template <typename... Args>
        inline T& create(Args&&... args)
        {
            destroy();
            return (_data = movemm::mmnew<refcounted_data>(
                        std::forward<Args>(args)...))
                ->value;
        }

        inline void destroy()
        {
            if (_data)
            {
                if ((--_data->refcount) == 0)
                {
                    movemm::mmdelete<refcounted_data>(_data);
                }
                _data = 0;
            }
        }

        inline size_t refcount() const
        {
            return _data ? _data->refcount.load() : 0;
        }

        inline T* get()
        {
            return &_data->value;
        }

        inline T const* get() const
        {
            return &_data->value;
        }

        inline bool valid() const
        {
            return _data;
        }

    public:
        inline operator bool() const
        {
            return valid();
        }

        inline T* operator->()
        {
            return &_data->value;
        }

        inline T const* operator->() const
        {
            return &_data->value;
        }

        inline T& operator*()
        {
            return _data->value;
        }

        inline const T& operator*() const
        {
            return _data->value;
        }

    public:
        inline shared_ptr<T>& operator=(shared_ptr<T>& rhs)
        {
            if (_data != rhs._data)
            {
                destroy();

                _data = rhs._data;
                if (_data)
                {
                    ++_data->refcount;
                }
            }
            return *this;
        }

        inline shared_ptr<T>& operator=(shared_ptr<T>&& rhs)
        {
            if (_data != rhs._data)
            {
                destroy();
                _data = rhs._data;
                rhs._data = 0;
            }
            return *this;
        }

    public:
        inline bool operator==(const shared_ptr<T>& rhs) const
        {
            return _data == rhs._data;
        }

        inline bool operator!=(const shared_ptr<T>& rhs) const
        {
            return _data != rhs._data;
        }

    private:
        refcounted_data* _data;
    };

    template <typename T, typename... Args>
    inline shared_ptr<T> make_shared(Args&&... args)
    {
        shared_ptr<T> res;
        res.create(std::forward<Args>(args)...);
        return res;
    }
}  // namespace movemm
