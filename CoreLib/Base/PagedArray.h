#pragma once

#include <cstdint>
#include <span>

BEGIN_SE()

struct DefaultPagedAllocator
{
    inline void* alloc(std::size_t size)
    {
        return GameAllocRaw(size);
    }
    
    template <class T>
    inline T* allocArray(std::size_t size)
    {
        return GameAllocArray<T>(size);
    }

    inline void free(void* ptr)
    {
        GameFree(ptr);
    }
};

struct PageLayout
{
    inline PageLayout(uint16_t bitsPerPage)
        : BitsPerPage(bitsPerPage),
        NumPages(0)
    {}

    uint16_t BitsPerPage;
    uint16_t NumPages;

    inline uint32_t bucket_size() const
    {
        return 1u << BitsPerPage;
    }

    inline uint32_t capacity() const
    {
        return NumPages * bucket_size();
    }

    template <class T>
    inline T* at(T** pages, uint32_t index) const
    {
        se_assert(index < capacity());
        return &pages[index >> BitsPerPage][index & ((1 << BitsPerPage) - 1)];
    }
};

template <class T, class TAllocator>
struct PagedOps
{
    static void Resize(uint32_t newSize, T**& pages, PageLayout& layout, TAllocator& allocator)
    {
        if (newSize) {
            auto bucketSize = (int32_t)layout.bucket_size();
            auto newNumBuckets = (uint32_t)((-bucketSize & (bucketSize + newSize - 1)) >> layout.BitsPerPage);
            if (newNumBuckets != layout.NumPages) {
                for (auto i = newNumBuckets; i < layout.NumPages; i++) {
                    allocator.free(pages[i]);
                }

                auto newAllocBuckets = (newNumBuckets + 15u) & 0xFFF0u;
                auto oldAllocBuckets = (layout.NumPages + 15u) & 0xFFF0u;
                if (oldAllocBuckets < newAllocBuckets) {
                    auto newBuckets = allocator.allocArray<T*>(newAllocBuckets);
                    if (pages) {
                        if (layout.NumPages < newNumBuckets)
                            layout.NumPages = (uint16_t)newNumBuckets;
                        memcpy(newBuckets, pages, sizeof(T*) * std::min(newNumBuckets, (uint32_t)layout.NumPages));
                        allocator.free(pages);
                    }
                    pages = newBuckets;
                }

                for (auto i = layout.NumPages; i < newNumBuckets; i++) {
                    pages[i] = static_cast<T*>(allocator.alloc(sizeof(T) * bucketSize));
                }

                layout.NumPages = newNumBuckets;
            }
        } else {
            if (layout.NumPages) {
                for (auto i = 0; i < layout.NumPages; i++) {
                    allocator.free(pages[i]);
                }

                if (pages != nullptr) {
                    allocator.free(pages);
                }
            }

            layout.NumPages = 0;
        }
    }
};

template <class T, class TAllocator = DefaultPagedAllocator>
class PagedArray : protected TAllocator
{
public:
    PagedArray(uint16_t bitsPerPage, TAllocator const& allocator)
        : TAllocator(allocator), layout_(bitsPerPage)
    {}

    // Can't copy these for now
    PagedArray(PagedArray const&) = delete;

    inline TAllocator& allocator()
    {
        return *this;
    }

    inline uint32_t size() const
    {
        return size_;
    }

    inline uint32_t bucket_size() const
    {
        return layout_.bucket_size();
    }

    inline PageLayout const& layout() const
    {
        return layout_;
    }

    inline uint32_t capacity() const
    {
        return layout_.capacity();
    }

    void resize(uint32_t newSize)
    {
        PagedOps<T, TAllocator>::Resize(newSize, pages_, layout_, *this);
    }

    T* add()
    {
        if (capacity() <= size()) {
            resize(capacity() + layout_.bucket_size());
        }

        auto val = layout_.at(pages_, size_++);
        new (val) T();
        return val;
    }

    T* add_uninitialized()
    {
        if (capacity() <= size()) {
            resize(capacity() + layout_.bucket_size());
        }

        return layout_.at(pages_, size_++);
    }

    inline T const& operator [] (uint32_t index) const
    {
        return *layout_.at(pages_, index);
    }

    inline T& operator [] (uint32_t index)
    {
        return *layout_.at(pages_, index);
    }

private:
    T** pages_{ nullptr };
    PageLayout layout_;
    uint32_t size_{ 0 };
};

template <class TAllocator = DefaultPagedAllocator>
struct PagedBitSet
{
    PagedArray<uint64_t, TAllocator> Storage;

    inline uint32_t size() const
    {
        return Storage.size() << 6;
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
        se_assert(index < size());
        auto qword = index >> 6;
        Storage[qword] |= (1ull << (index & 0x3f));
    }

    inline void clear(uint32_t index)
    {
        se_assert(index < size());
        auto qword = index >> 6;
        Storage[qword] &= ~(1ull << (index & 0x3f));
    }
};


template <class TKey, class TValue, class TAllocator = DefaultPagedAllocator>
struct DoubleIndexedPagedArray
{
    static constexpr uint16_t InvalidIndex = 0xffff;

    PagedBitSet<TAllocator> Bitmap;
    PagedArray<uint16_t, DefaultPagedAllocator> LookupTable;
    PagedArray<TKey, TAllocator> Keys;
    PagedArray<TValue, TAllocator> Values;

    uint16_t FindIndex(TKey const& key) const
    {
        auto hash = (uint32_t)SparseHashMapHash(key);
        if (!Bitmap[hash]) return InvalidIndex;

        auto keyIndex = LookupTable[hash];
        if (keyIndex == InvalidIndex || Keys[keyIndex] != key) return InvalidIndex;
        return keyIndex;
    }

    TValue const* Find(TKey const& key) const
    {
        auto index = FindIndex(key);
        if (index == InvalidIndex) return nullptr;

        return &Values[index];
    }

    TValue* Find(TKey const& key)
    {
        auto index = FindIndex(key);
        if (index == InvalidIndex) return nullptr;

        return &Values[index];
    }

    template <class... Args>
    TValue* Add(TKey const& key, Args... args)
    {
        auto hash = (uint32_t)SparseHashMapHash(key);
        Bitmap.set(hash);
        auto nextIndex = (uint16_t)Keys.size();
        LookupTable[hash] = nextIndex;
        new (Keys.add_uninitialized()) TKey(key);
        return new (Values.add_uninitialized()) TValue(std::forward<Args>(args)...);
    }
};

END_SE()
