#pragma once

#include <cstdint>
#include <span>

BEGIN_SE()

struct DefaultPagedAllocator {};

template <class T, class TAllocator = DefaultPagedAllocator>
struct PagedArray : public TAllocator
{
    T** Buckets;
    uint16_t BitsPerBucket;
    uint16_t NumBuckets;
    uint32_t Size;

    // Can't construct these for now
    PagedArray() = delete;
    PagedArray(PagedArray const&) = delete;

    void Resize(uint32_t newSize)
    {
        if (newSize) {
            auto bucketSize = 1u << BitsPerBucket;
            auto newNumBuckets = (~bucketSize & (bucketSize + newSize - 1)) >> BitsPerBucket;
            if (newNumBuckets != NumBuckets) {
                for (auto i = newNumBuckets; i < NumBuckets; i++) {
                    GameFree(Buckets[i]);
                }

                auto newAllocBuckets = (newNumBuckets + 15) & 0xFFF0;
                if (newAllocBuckets < NumBuckets) {
                    auto newBuckets = GameAllocArray<T*>(newAllocBuckets);
                    if (Buckets) {
                        if ((unsigned int)NumBuckets < NumBuckets)
                            NumBuckets = NumBuckets;
                        memcpy(newBuckets, Buckets, sizeof(T*) * std::min(newNumBuckets, (uint32_t)NumBuckets));
                        GameFree(Buckets);
                    }
                    Buckets = newBuckets;
                }

                for (auto i = NumBuckets; i < newNumBuckets; i++) {
                    Buckets[i] = GameAllocArray<T>(bucketSize);
                }

                NumBuckets = newNumBuckets;
            }
        } else {
            if (NumBuckets) {
                for (auto i = 0; i < NumBuckets; i++) {
                    GameFree(Buckets[i]);
                }

                if (Buckets != nullptr) {
                    GameFree(Buckets);
                }
            }

            NumBuckets = 0;
        }
    }

    inline T const& operator [] (uint32_t index) const
    {
        assert(index < ((uint32_t)NumBuckets << BitsPerBucket));
        return Buckets[index >> BitsPerBucket][index & ((1 << BitsPerBucket) - 1)];
    }

    inline T& operator [] (uint32_t index)
    {
        assert(index < ((uint32_t)NumBuckets << BitsPerBucket));
        return Buckets[index >> BitsPerBucket][index & ((1 << BitsPerBucket) - 1)];
    }
};


template <class TKey, class TAllocator = DefaultPagedAllocator>
struct PagedHashSet : public TAllocator
{
    int32_t** HashKeys;
    int32_t** NextIds;
    TKey** Keys;
    uint16_t BitsPerHashBucket;
    uint16_t NumHashBuckets;
    uint16_t BitsPerKeyBucket;
    uint16_t NumKeyBuckets;
    uint32_t HashTableSize;
    uint32_t KeysSize;

    TKey const& KeyAt(uint32_t index) const
    {
        auto keyTable = index >> BitsPerKeyBucket;
        auto keySlot = index & ((1 << BitsPerKeyBucket) - 1);
        return Keys[keyTable][keySlot];
    }

    int FindIndex(TKey const& key) const
    {
        if (HashTableSize == 0) return -1;

        auto hash = HashMapHash(key) % HashTableSize;
        auto keyIndex = HashKeys[hash >> BitsPerHashBucket][hash & ((1 << BitsPerHashBucket) - 1)];
        while (keyIndex >= 0) {
            auto keyTable = keyIndex >> BitsPerKeyBucket;
            auto keySlot = keyIndex & ((1 << BitsPerKeyBucket) - 1);
            if (Keys[keyTable][keySlot] == key) return keyIndex;
            keyIndex = NextIds[keyTable][keySlot];
        }

        return -1;
    }
};

template <class TAllocator = DefaultPagedAllocator>
struct PagedBitSet
{
    PagedArray<uint64_t, TAllocator> Storage;

    inline uint32_t size() const
    {
        return Storage.Size << 6;
    }

    inline bool get(uint32_t index) const
    {
        if (index >= size()) {
            return false;
        } else {
            auto qword = index >> 6;
            return _bittest64((int64_t const*)&Storage[qword], index & 0x3f);
        }
    }

    inline bool operator [] (uint32_t index) const
    {
        return get(index);
    }

    inline void set(uint32_t index)
    {
        assert(index < size());
        auto qword = index >> 6;
        Storage[qword] |= (1ull << (index & 0x3f));
    }

    inline void clear(uint32_t index)
    {
        assert(index < size());
        auto qword = index >> 6;
        Storage[qword] &= ~(1ull << (index & 0x3f));
    }
};


template <class TKey, class TValue, class TAllocator = DefaultPagedAllocator>
struct DoubleIndexedPagedArray
{
    PagedBitSet<TAllocator> Bitmap;
    PagedArray<uint16_t, DefaultPagedAllocator> LookupTable;
    PagedArray<TKey, TAllocator> Keys;
    PagedArray<TValue, TAllocator> Values;

    int FindIndex(TKey const& key) const
    {
        auto hash = (uint32_t)SparseHashMapHash(key);
        if (!Bitmap[hash]) return -1;

        auto keyIndex = LookupTable[hash];
        if (keyIndex == 0xFFFF || Keys[keyIndex] != key) return -1;
        return keyIndex;
    }

    TValue const* Find(TKey const& key) const
    {
        auto index = FindIndex(key);
        if (index < 0) return nullptr;

        return &Values[index];
    }
};


template <class TKey, class TValue, class TAllocator>
struct PagedHashMap : public PagedHashSet<TKey, TAllocator>
{
    PagedArray<TValue, TAllocator> Values;

    TValue* Find(TKey const& key)
    {
        auto index = this->FindIndex(key);
        if (index < 0) return nullptr;

        return &Values[index];
    }

    TValue const* Find(TKey const& key) const
    {
        auto index = this->FindIndex(key);
        if (index < 0) return nullptr;

        return &Values[index];
    }
};

END_SE()
