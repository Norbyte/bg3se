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
                Allocator::Free((void*)((std::ptrdiff_t)buf - 8));
            }
        } else {
            if (buf != nullptr) {
                Allocator::Free(buf);
            }
        }
    }

    void RawReallocate(TSize newCapacity)
    {
        if (newCapacity > 0) {
            if (StoreSize) {
                auto newBuf = Allocator::Alloc(newCapacity * sizeof(T) + 8);
                *(uint64_t*)newBuf = newCapacity;

                Buf = (T*)((std::ptrdiff_t)newBuf + 8);
            } else {
                Buf = Allocator::template New<T>(newCapacity);
            }
        } else {
            Buf = nullptr;
        }

        Capacity = newCapacity;
    }

    void Reallocate(TSize newCapacity)
    {
        auto oldBuf = Buf;
        auto oldCapacity = Capacity;
        RawReallocate(newCapacity);

        for (uint32_t i = 0; i < std::min(Size, newCapacity); i++) {
            new (Buf + i) T(oldBuf[i]);
        }

        for (uint32_t i = std::min(Size, newCapacity); i < newCapacity; i++) {
            new (Buf + i) T();
        }

        for (uint32_t i = 0; i < oldCapacity; i++) {
            oldBuf[i].~T();
        }

        FreeBuffer(oldBuf);
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
