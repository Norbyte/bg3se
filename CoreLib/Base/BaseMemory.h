#pragma once

#include <cstdint>
#include <atomic>
#include <vector>

namespace bg3se
{
    using ls__GlobalAllocator__GetProc = void* ();
    using ls__GlobalAllocator__AllocProc = void* (void* globalAllocator, std::size_t size, int pool, int unused2, uint64_t alignment);
    using ls__GlobalAllocator__FreeProc = void (void* globalAllocator, void* ptr);

    _Post_writable_byte_size_(size) void* GameAllocRaw(std::size_t size);
    void GameFree(_Post_ptr_invalid_ _Pre_valid_ void*) noexcept;

    template <class T, class ...Args>
    _Post_writable_size_(1) T* GameAlloc(Args... args)
    {
        auto ptr = static_cast<T*>(GameAllocRaw(sizeof(T)));
        new (ptr) T(args...);
        return ptr;
    }

    template <class T>
    void GameDelete(_Post_ptr_invalid_ _Pre_valid_ T* obj)
    {
        obj->~T();
        GameFree(obj);
    }

    template <class T, class ...Args>
    _Post_writable_size_(n) T* GameAllocArray(std::size_t n, Args... args)
    {
        auto ptr = static_cast<T*>(GameAllocRaw(sizeof(T) * n));
        for (auto i = 0; i < n; i++) {
            new (ptr + i) T(args...);
        }
        return ptr;
    }

    template <class T>
    void GameDeleteArray(_Post_ptr_invalid_ _Pre_writable_size_(n) T* ptr, std::size_t n)
    {
        for (std::size_t i = 0; i < n; i++) {
            ptr[i].~T();
        }

        GameFree(ptr);
    }

    template <class T>
    using GameUniquePtr = std::unique_ptr<T, decltype(&GameDelete<T>)>;

    template <class T>
    class GameAllocator
    {
    public:
        using value_type = T;
        using size_type = uint32_t;
        using difference_type = int64_t;

        inline GameAllocator() noexcept {}
        template <class U>
        inline GameAllocator(GameAllocator<U> const&) noexcept {}

        _Post_writable_size_(cnt) inline T* allocate(std::size_t cnt)
        {
            return static_cast<T*>(GameAllocRaw(cnt * sizeof(T)));
        }

        inline void deallocate(_Post_ptr_invalid_ _Pre_writable_size_(cnt) T* p, std::size_t cnt) noexcept
        {
            GameFree(p);
        }
    };

    template <class T>
    class GameStdAllocator
    {
    public:
        using value_type = T;

        inline GameStdAllocator() noexcept {}
        template <class U>
        inline GameStdAllocator(GameStdAllocator<U> const&) noexcept {}

        _Post_writable_size_(cnt) inline T* allocate(std::size_t cnt)
        {
            return static_cast<T*>(GameAllocRaw(cnt * sizeof(T)));
        }

        inline void deallocate(_Post_ptr_invalid_ _Pre_writable_size_(cnt) T* p, std::size_t cnt) noexcept
        {
            GameFree(p);
        }
    };

    template <class T, class U>
    bool operator == (GameAllocator<T> const&, GameAllocator<U> const&) noexcept
    {
        return true;
    }

    template <class T, class U>
    bool operator != (GameAllocator<T> const& x, GameAllocator<U> const& y) noexcept
    {
        return !(x == y);
    }

    // Identifies types that can be allocated by us (using the game allocator)
    template <class>
    std::false_type IsAllocatable(...);

#define MARK_ALLOCATABLE(ty) template <class T> \
    std::true_type IsAllocatable(T*)


    template <class T>
    using Vector = std::vector<T, GameAllocator<T>>;


    struct GameMemoryAllocator
    {
        static _Post_writable_byte_size_(size) void* Alloc(std::size_t size)
        {
            return GameAllocRaw(size);
        }

        template <class T>
        static _Post_writable_size_(1) T* New()
        {
            return GameAlloc<T>();
        }

        template <class T>
        static _Post_writable_size_(count) T* New(std::size_t count)
        {
            return GameAllocArray<T>(count);
        }

        template <class T>
        static _Post_writable_size_(1) T* NewRaw()
        {
            return static_cast<T*>(GameAllocRaw(sizeof(T)));
        }

        template <class T>
        static _Post_writable_size_(count) T* NewRaw(std::size_t count)
        {
            return static_cast<T*>(GameAllocRaw(count * sizeof(T)));
        }

        template <class T>
        static void Free(_Post_ptr_invalid_ _Pre_valid_ T* ptr)
        {
            return GameFree(ptr);
        }

        template <class T>
        static void FreeArray(_Post_ptr_invalid_ _Pre_valid_ T* ptr)
        {
            return GameFree(ptr);
        }
    };
}
