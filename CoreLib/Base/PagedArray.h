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

    ~PagedArray()
    {
        clear();
    }

    // Can't copy these for now
    PagedArray(PagedArray const&) = delete;
    PagedArray(PagedArray &&) = delete;

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

    void realloc(uint32_t newSize)
    {
        PagedOps<T, TAllocator>::Resize(newSize, pages_, layout_, *this);
    }

    template <class TAlloc>
    void reallocExtern(uint32_t newSize, TAlloc& alloc)
    {
        PagedOps<T, TAlloc>::Resize(newSize, pages_, layout_, alloc);
    }

    void resize(uint32_t newSize)
    {
        for (auto i = newSize; i < size_; i++) {
            layout_.at(pages_, i)->~T();
        }

        realloc(newSize);

        for (auto i = size_; i < newSize; i++) {
            new (layout_.at(pages_, i)) T();
        }

        size_ = newSize;
    }

    void clear()
    {
        PagedOps<T, TAllocator>::Resize(0, pages_, layout_, *this);
    }

    template <class TAlloc>
    void clearExtern(TAlloc& alloc)
    {
        PagedOps<T, TAlloc>::Resize(0, pages_, layout_, alloc);
    }

    T* add()
    {
        if (capacity() <= size()) {
            realloc(capacity() + layout_.bucket_size());
        }

        auto val = layout_.at(pages_, size_++);
        new (val) T();
        return val;
    }

    T* add_uninitialized()
    {
        if (capacity() <= size()) {
            realloc(capacity() + layout_.bucket_size());
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
public:
    inline TAllocator& allocator()
    {
        return storage_.allocator();
    }

    inline uint32_t size() const
    {
        return storage_.size() << 6;
    }

    inline bool get(uint32_t index) const
    {
        if (index >= size()) {
            return false;
        } else {
            auto qword = index >> 6;
            return _bittest64((int64_t const*)&storage_[qword], index & 0x3f);
        }
    }

    inline bool operator [] (uint32_t index) const
    {
        return get(index);
    }

    void resize(uint32_t newSize)
    {
        if (newSize > size()) {
            storage_.resize(newSize / 64 + ((newSize % 64) ? 1 : 0));
        }
    }

    inline void set(uint32_t index)
    {
        if (index >= size()) {
            resize(index + 1);
        }

        auto qword = index >> 6;
        storage_[qword] |= (1ull << (index & 0x3f));
    }

    inline void clear(uint32_t index)
    {
        if (index < size()) {
            auto qword = index >> 6;
            storage_[qword] &= ~(1ull << (index & 0x3f));
        }
    }

private:
    PagedArray<uint64_t, TAllocator> storage_;
};

END_SE()
