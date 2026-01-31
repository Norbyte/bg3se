#pragma once

#include <CoreLib/Base/BaseArray.h>

BEGIN_SE()

template <class T, class Allocator = GameMemoryAllocator, bool StoreSize = false, class TSize = uint32_t>
struct CompactSet
{
    using value_type = T;
    using reference = T&;
    using const_reference = T const&;
    using iterator = ContiguousIterator<T>;
    using const_iterator = ContiguousConstIterator<T>;
    using difference_type = int32_t;
    using size_type = uint32_t;
    static constexpr unsigned size_offset = (alignof(T) < sizeof(uint64_t) ? sizeof(uint64_t) : alignof(T));

    T* Buf{ nullptr };
    TSize Capacity{ 0 };
    TSize Size{ 0 };

    inline CompactSet() {}

    CompactSet(CompactSet const& other)
    {
        Reallocate(other.Size);
        Size = other.Size;
        for (uint32_t i = 0; i < other.Size; i++) {
            new (Buf + i) T(other.Buf[i]);
        }
    }

    ~CompactSet()
    {
        if (Buf) {
            clear();
            FreeBuffer(Buf);
        }
    }

    CompactSet& operator = (CompactSet const& other)
    {
        clear();
        Reallocate(other.Size);
        Size = other.Size;
        for (uint32_t i = 0; i < other.Size; i++) {
            new (Buf + i) T(other.Buf[i]);
        }
        return *this;
    }

    inline unsigned int size() const
    {
        return Size;
    }

    inline T const& operator [] (uint32_t index) const
    {
        se_assert(index < Size);
        return Buf[index];
    }

    inline T& operator [] (uint32_t index)
    {
        se_assert(index < Size);
        return Buf[index];
    }

    void FreeBuffer(void* buf)
    {
        if (StoreSize) {
            if (buf != nullptr) {
                Allocator::Free((void*)((std::ptrdiff_t)buf - size_offset));
            }
        } else {
            if (buf != nullptr) {
                Allocator::Free(buf);
            }
        }
    }

    T* RawReallocate(TSize newCapacity)
    {
        Capacity = newCapacity;

        if (newCapacity > 0) {
            if (StoreSize) {
                auto newBuf = Allocator::Alloc(newCapacity * sizeof(T) + size_offset);
                *(uint64_t*)newBuf = newCapacity;

                return (T*)((std::ptrdiff_t)newBuf + size_offset);
            } else {
                return Allocator::template New<T>(newCapacity);
            }
        } else {
            return nullptr;
        }
    }

    void Reallocate(TSize newCapacity)
    {
        auto oldBuf = Buf;
        auto oldCapacity = Capacity;
        auto newBuf = RawReallocate(newCapacity);

        auto itemsToMove = std::min(Size, newCapacity);
        if constexpr (std::is_move_constructible_v<T>) {
            for (uint32_t i = 0; i < itemsToMove; i++) {
                new (newBuf + i) T(std::move(oldBuf[i]));
            }

            for (uint32_t i = itemsToMove; i < newCapacity; i++) {
                new (newBuf + i) T();
            }

            // Late reassignment of 'Buf' should also reduce the likelihood of concurrent access
            // to the not yet initialized array data (unfortunate, but happens).
            // It's still possible for other threads to see the moved-from objects, but it should 
            // not cause access to uninitialized memory and crash.
            Buf = newBuf;

            for (uint32_t i = itemsToMove; i < oldCapacity; i++) {
                oldBuf[i].~T();
            }
        } else {
            for (uint32_t i = 0; i < itemsToMove; i++) {
                new (newBuf + i) T(oldBuf[i]);
            }

            for (uint32_t i = itemsToMove; i < newCapacity; i++) {
                new (newBuf + i) T();
            }

            Buf = newBuf;

            for (uint32_t i = 0; i < oldCapacity; i++) {
                oldBuf[i].~T();
            }
        }

        FreeBuffer(oldBuf);
    }

    void resize(TSize newCapacity)
    {
        Reallocate(newCapacity);
        Size = newCapacity;
    }

    void ordered_remove_at(uint32_t index)
    {
        se_assert(index < Size);

        for (unsigned i = index; i < (unsigned)Size - 1; i++) {
            Buf[i] = Buf[i + 1];
        }

        Buf[Size - 1] = T();
        Size--;
    }

    void remove_at(uint32_t index)
    {
        se_assert(index < Size);

        if (index == Size - 1) {
            Buf[index] = T();
        } else {
            Buf[index] = std::move(Buf[Size - 1]);
            Buf[Size - 1] = T();
        }
        Size--;
    }

    void erase(iterator const& it)
    {
        se_assert(it != end());
        ordered_remove_at((size_type)(it.get() - Buf));
    }

    void clear()
    {
        for (uint32_t i = 0; i < Size; i++) {
            Buf[i] = T();
        }

        Size = 0;
    }

    TSize CapacityIncrement() const
    {
        if (this->Capacity > 0) {
            return 2 * this->Capacity;
        } else {
            return 1;
        }
    }

    void push_back(T const& value)
    {
        if (this->Capacity <= this->Size) {
            this->Reallocate(CapacityIncrement());
        }

        new (&this->Buf[this->Size++]) T(value);
    }

    ContiguousIterator<T> begin()
    {
        return ContiguousIterator<T>(Buf);
    }

    ContiguousConstIterator<T> begin() const
    {
        return ContiguousConstIterator<T>(Buf);
    }

    ContiguousIterator<T> end()
    {
        return ContiguousIterator<T>(Buf + Size);
    }

    ContiguousConstIterator<T> end() const
    {
        return ContiguousConstIterator<T>(Buf + Size);
    }

    T const* data() const
    {
        return Buf;
    }

    T* data()
    {
        return Buf;
    }
};

template <class T>
using TrackedCompactSet = CompactSet<T, GameMemoryAllocator, true>;

template <class T>
using MiniCompactSet = CompactSet<T, GameMemoryAllocator, true, uint8_t>;

template <class T, class Allocator = GameMemoryAllocator, bool StoreSize = false>
struct Set : public CompactSet<T, Allocator, StoreSize>
{
    uint64_t CapacityIncrementSize{ 0 };

    uint32_t CapacityIncrement() const
    {
        if (CapacityIncrementSize != 0) {
            return this->Capacity + (uint32_t)CapacityIncrementSize;
        }
        else if (this->Capacity > 0) {
            return 2 * this->Capacity;
        }
        else {
            return 1;
        }
    }

    void push_back(T const& value)
    {
        if (this->Capacity <= this->Size) {
            this->Reallocate(CapacityIncrement());
        }

        new (&this->Buf[this->Size++]) T(value);
    }

    void Add(T const& value)
    {
        if (this->Capacity <= this->Size) {
            this->Reallocate(CapacityIncrement());
        }

        new (&this->Buf[this->Size++]) T(value);
    }

    void InsertAt(uint32_t index, T const& value)
    {
        if (this->Capacity <= this->Size) {
            Reallocate(CapacityIncrement());
        }

        for (auto i = this->Size; i > index; i--) {
            this->Buf[i] = this->Buf[i - 1];
        }

        this->Buf[index] = value;
        this->Size++;
    }

    void remove_last()
    {
        se_assert(this->Size > 0);
        this->Size--;
    }

    T pop_last()
    {
        se_assert(this->Size > 0);
        return this->Buf[--this->Size];
    }
};

template <class T, class Allocator = GameMemoryAllocator>
struct PrimitiveSmallSet : public CompactSet<T, Allocator, false>
{
    virtual ~PrimitiveSmallSet() {}

    uint32_t CapacityIncrement() const
    {
        if (this->Capacity > 0) {
            return 2 * this->Capacity;
        }
        else {
            return 1;
        }
    }

    void Add(T const& value)
    {
        if (this->Capacity <= this->Size) {
            Reallocate(CapacityIncrement());
        }

        new (&this->Buf[this->Size++]) T(value);
    }
};

template <class T, class Allocator = GameMemoryAllocator, bool StoreSize = false>
struct ObjectSet : public Set<T, Allocator, StoreSize>
{
};

template <class T, class Allocator = GameMemoryAllocator>
struct PrimitiveSet : public ObjectSet<T, Allocator, false>
{
};

END_SE()
