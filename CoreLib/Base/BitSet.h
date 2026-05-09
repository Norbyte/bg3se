#pragma once

BEGIN_SE()

inline std::optional<uint32_t> BitSetScan(uint64_t const* begin, uint64_t const* end)
{
    DWORD index;
    for (auto buf = begin; buf < end; buf++) {
        if (_BitScanForward64(&index, *buf)) {
            return (uint32_t)(buf - begin) * 64 + index;
        }
    }

    return {};
}

inline std::optional<uint32_t> BitSetScan(uint64_t const* begin, uint64_t const* end, uint32_t prev)
{
    auto buf = begin + (prev / 64);

    DWORD index;
    auto off = (prev % 64) + 1;
    if (off != 64) {
        // Mask off already visited bits
        auto val = (*buf >> off) << off;
        // Scan remainder of current qword
        if (_BitScanForward64(&index, val)) {
            return (uint32_t)(buf - begin) * 64 + index;
        }
    }

    buf++;
    for (; buf < end; buf++) {
        if (_BitScanForward64(&index, *buf)) {
            return (uint32_t)(buf - begin) * 64 + index;
        }
    }

    return {};
}


template <class TWord, unsigned NumWords>
struct BitArray
{
    using value_type = bool;

    static constexpr uint32_t BitsPerWord = sizeof(TWord) * CHAR_BIT;
    static constexpr uint32_t IndexBitsPerWord = (sizeof(TWord) == 4) ? 5 : 6;
    static constexpr uint32_t NumBits = NumWords * BitsPerWord;

    TWord Bits[NumWords]{ 0 };

    inline bool operator [] (uint32_t index) const
    {
        if (index >= NumBits) {
            return false;
        }

        return (Bits[index >> IndexBitsPerWord] & (TWord(1) << (index & (BitsPerWord - 1)))) != 0;
    }

    inline bool Set(uint32_t index)
    {
        if (index >= NumBits) {
            return false;
        }

        Bits[index >> IndexBitsPerWord] |= (TWord(1) << (index & (BitsPerWord - 1)));
        return true;
    }

    inline bool AtomicSet(uint32_t index)
    {
        if (index >= NumBits) {
            return false;
        }

        InterlockedOr64((LONG64*)(Bits + (index >> IndexBitsPerWord)), (TWord(1) << (index & (BitsPerWord - 1))));
        return true;
    }

    inline bool Clear(uint32_t index)
    {
        if (index >= NumBits) {
            return false;
        }

        Bits[index >> IndexBitsPerWord] &= ~(TWord(1) << (index & (BitsPerWord - 1)));
        return true;
    }

    inline void Clear()
    {
        for (unsigned i = 0; i < NumWords; i++) {
            Bits[i] = 0u;
        }
    }

    inline bool IsSet(uint32_t index) const
    {
        if (index >= NumBits) {
            return false;
        }

        return (Bits[index >> IndexBitsPerWord] & (TWord(1) << (index & (BitsPerWord - 1)))) != 0;
    }

    inline uint32_t size() const
    {
        return NumWords * sizeof(TWord) * CHAR_BIT;
    }

    inline std::optional<uint32_t> FindFirst()
    {
        return BitSetScan(Bits, Bits + NumWords);
    }

    inline std::optional<uint32_t> FindNext(uint32_t prev)
    {
        return BitSetScan(Bits, Bits + NumWords, prev);
    }
};

template <class Allocator = GameMemoryAllocator>
struct BitSet
{
    union {
        uint64_t* Buf{ nullptr };
        uint64_t InlineValue;
    };
    uint32_t Capacity{ 64 };
    uint32_t Size{ 0 };

    inline BitSet() {}

    inline BitSet(uint64_t val)
        : InlineValue(val),
        Size(64)
    {}

    BitSet(BitSet const& other)
    {
        Reallocate(other.Size);
        Size = other.Size;
        for (uint32_t i = 0; i < other.NumQwords(); i++) {
            GetBuf()[i] = other.GetBuf()[i];
        }
    }

    BitSet(BitSet&& other) noexcept
    {
        InlineValue = other.InlineValue;
        Capacity = other.Capacity;
        Size = other.Size;

        other.InlineValue = 0;
        other.Capacity = 64;
        other.Size = 0;
    }

    ~BitSet()
    {
        Clear();
        if (Capacity > 64) {
            Allocator::Free(Buf);
        }
    }

    BitSet& operator = (BitSet const& other)
    {
        Clear();
        Reallocate(other.Size);
        Size = other.Size;
        for (uint32_t i = 0; i < other.NumQwords(); i++) {
            GetBuf()[i] = other.GetBuf()[i];
        }
        return *this;
    }

    BitSet& operator = (BitSet&& other) noexcept
    {
        InlineValue = other.InlineValue;
        Capacity = other.Capacity;
        Size = other.Size;

        other.InlineValue = 0;
        other.Capacity = 64;
        other.Size = 0;

        return *this;
    }

    inline uint32_t NumQwords() const
    {
        return (Size / 64) + ((Size % 64) ? 1 : 0);
    }

    inline uint64_t const* GetBuf() const
    {
        if (Capacity > 64) {
            return Buf;
        } else {
            return &InlineValue;
        }
    }

    inline uint64_t* GetBuf()
    {
        if (Capacity > 64) {
            return Buf;
        } else {
            return &InlineValue;
        }
    }

    inline bool Get(uint32_t index) const
    {
        if (Size < index) {
            return false;
        } else {
            return (GetBuf()[index / 64] & (1ull << (index % 64))) != 0;
        }
    }

    inline bool UnsafeGet(uint32_t index) const
    {
        return (GetBuf()[index / 64] & (1ull << (index % 64))) != 0;
    }

    inline bool operator [] (uint32_t index) const
    {
        if (Size < index) {
            return false;
        } else {
            return (GetBuf()[index / 64] & (1ull << (index % 64))) != 0;
        }
    }

    inline void Set(uint32_t index)
    {
        EnsureSize(index + 1);
        GetBuf()[index / 64] |= (1ull << (index % 64));
    }

    inline void Clear(uint32_t index)
    {
        EnsureSize(index + 1);
        GetBuf()[index / 64] &= ~(1ull << (index % 64));
    }

    inline std::optional<uint32_t> FindFirst()
    {
        auto start = GetBuf();
        return BitSetScan(start, start + NumQwords());
    }

    inline std::optional<uint32_t> FindNext(uint32_t prev)
    {
        auto start = GetBuf();
        return BitSetScan(start, start + NumQwords(), prev);
    }

    void EnsureSize(uint32_t size)
    {
        if (Size < size) {
            Reallocate(size);
            Size = size;
        }
    }

    void Reallocate(uint32_t newCapacity)
    {
        newCapacity = ((newCapacity / 64) + ((newCapacity % 64) ? 1 : 0)) * 64;

        if (Capacity == newCapacity) {
            return;
        }

        auto oldCapacity = Capacity;
        auto oldBuf = Buf;
        auto oldInline = InlineValue;

        if (newCapacity > 64) {
            Buf = Allocator::template New<uint64_t>(newCapacity);
        } else {
            // Don't touch inline value
        }

        Capacity = newCapacity;

        if (newCapacity < Size) {
            Size = newCapacity;
        }

        if (oldCapacity <= 64 && newCapacity <= 64) {
            // Nothing to do, inline value untouched
        } else if (oldCapacity <= 64 && newCapacity > 64) {
            Buf[0] = oldInline;
        } else if (oldCapacity > 64 && newCapacity <= 64) {
            InlineValue = oldBuf[0];
        } else {
            for (uint32_t i = 0; i < NumQwords(); i++) {
                Buf[i] = oldBuf[i];
            }
        }
                
        if (oldCapacity > 64) {
            Allocator::Free(oldBuf);
        }
    }

    void Clear()
    {
        for (uint32_t i = 0; i < NumQwords(); i++) {
            GetBuf()[i] = 0;
        }

        Size = 0;
    }
};

template <class Allocator = GameMemoryAllocator>
struct StaticBitSet
{
    uint64_t* Buf{ nullptr };
    uint32_t Size{ 0 };

    inline StaticBitSet() {}

    StaticBitSet(StaticBitSet const& other)
    {
        Reallocate(other.Size);
        Size = other.Size;
        for (uint32_t i = 0; i < other.NumQwords(); i++) {
            Buf[i] = other.Buf[i];
        }
    }

    StaticBitSet(StaticBitSet&& other)
    {
        Buf = other.Buf;
        Size = other.Size;

        other.Buf = 0;
        other.Size = 0;
    }

    ~StaticBitSet()
    {
        if (Buf) {
            Allocator::Free(Buf);
        }
    }

    StaticBitSet& operator = (StaticBitSet const& other)
    {
        Clear();
        Reallocate(other.Size);
        Size = other.Size;
        for (uint32_t i = 0; i < other.NumQwords(); i++) {
            Buf[i] = other.Buf[i];
        }
        return *this;
    }

    StaticBitSet& operator = (StaticBitSet&& other)
    {
        Buf = other.Buf;
        Size = other.Size;

        other.Buf = 0;
        other.Size = 0;

        return *this;
    }

    inline uint32_t NumQwords() const
    {
        return (Size / 64);
    }

    inline bool Get(uint32_t index) const
    {
        se_assert(index < Size);
        return (Buf[index / 64] & (1ull << (index % 64))) != 0;
    }

    inline bool operator [] (uint32_t index) const
    {
        se_assert(index < Size);
        return (Buf[index / 64] & (1ull << (index % 64))) != 0;
    }

    inline void Set(uint32_t index)
    {
        se_assert(index < Size);
        Buf[index / 64] |= (1ull << (index % 64));
    }

    inline void Clear(uint32_t index)
    {
        se_assert(index < Size);
        Buf[index / 64] &= ~(1ull << (index % 64));
    }

    void EnsureSize(uint32_t size)
    {
        if (Size < size) {
            Reallocate(size);
            Size = size;
        }
    }

    void Reallocate(uint32_t newSize)
    {
        newSize = ((newSize / 64) + ((newSize % 64) ? 1 : 0)) * 64;

        if (Size == newSize) {
            return;
        }

        auto oldSize = Size;
        auto oldBuf = Buf;

        if (newSize > 0) {
            Buf = Allocator::template New<uint64_t>(newSize);

            for (uint32_t i = 0; i < std::min(newSize / 64, oldSize / 64); i++) {
                Buf[i] = oldBuf[i];
            }
        } else {
            Buf = nullptr;
        }

        Size = newSize;
                
        if (oldBuf) {
            Allocator::Free(oldBuf);
        }
    }

    void Clear()
    {
        for (uint32_t i = 0; i < NumQwords(); i++) {
            Buf[i] = 0;
        }
    }
};

END_SE()
