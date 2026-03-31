#pragma once

BEGIN_SE()

template <class T>
class SparseArray
{
public:
    using value_type = T;
    using reference = T&;
    using const_reference = T const&;
    using iterator = ContiguousIterator<T>;
    using const_iterator = ContiguousConstIterator<T>;
    using difference_type = int32_t;
    using size_type = uint32_t;

    inline constexpr SparseArray() noexcept {}

    SparseArray(SparseArray const& a)
        : mask_(a.mask_),
        values_(a.values_)
    {}

    SparseArray(SparseArray&& a) noexcept
    {
        if (this != &a) {
            mask_ = std::move(a.mask_);
            values_ = std::move(a.values_);
        }
    }

    SparseArray& operator =(SparseArray const& a)
    {
        mask_ = a.mask_;
        values_ = a.values_;
        return *this;
    }

    SparseArray& operator =(SparseArray&& a) noexcept
    {
        if (this != &a) {
            mask_ = std::move(a.mask_);
            values_ = std::move(a.values_);
        }
        return *this;
    }

    inline constexpr bool empty() const noexcept
    {
        return values_.empty();
    }

    inline constexpr size_type size() const noexcept
    {
        return values_.size();
    }

    inline constexpr size_type capacity() const noexcept
    {
        return values_.capacity();
    }

    inline T const& operator [] (size_type index) const
    {
        se_assert(mask_[index]);
        se_assert(index < values_.size());
        return values_[index];
    }

    inline T& operator [] (size_type index)
    {
        se_assert(mask_[index]);
        se_assert(index < values_.size());
        return values_[index];
    }

    inline T const* try_get(size_type index) const
    {
        se_assert(index < values_.size());
        if (!mask_[index]) {
            return nullptr;
        } else {
            return &values_[index];
        }
    }

    inline T* try_get(size_type index)
    {
        se_assert(index < values_.size());
        if (!mask_[index]) {
            return nullptr;
        } else {
            return &values_[index];
        }
    }

    void clear()
    {
        mask_.Clear();
        values_.clear();
    }

    void clear(uint32_t index)
    {
        se_assert(index < values_.size());
        mask_.Clear(index);
        values_[index] = T{};
    }

    void set(uint32_t index, T const& value)
    {
        se_assert(index < values_.size());
        mask_.Set(index);
        values_[index] = value;
    }

    void set(uint32_t index, T&& value)
    {
        se_assert(index < values_.size());
        mask_.Set(index);
        values_[index] = std::move(value);
    }

    T& push_back(T const& value)
    {
        mask_.Set(values_.size());
        return values_.push_back(value);
    }

    T& push_back(T&& value)
    {
        mask_.Set(values_.size());
        return values_.push_back(std::move(value));
    }

private:
    BitSet<> mask_;
    Array<T> values_;
};

struct HashTablePagedAllocator
{
    inline void* alloc(std::size_t size)
    {
        // The parent sparse structure controls allocations for the table; we shouldn't get here
        se_assert(false && "Trying to invoke sparse hash table allocator but it shouldn't be used!");
        terminate();
    }

    template <class T>
    inline T* allocArray(std::size_t size)
    {
        se_assert(false && "Trying to invoke sparse hash table allocator but it shouldn't be used!");
        terminate();
    }

    inline void free(void* ptr)
    {
        se_assert(false && "Trying to invoke sparse hash table allocator but it shouldn't be used!");
        terminate();
    }
};

template <class T, class TAllocator>
class SparsePagedArray
{
public:
    using value_type = T;
    using reference = T&;
    using const_reference = T const&;
    using difference_type = int32_t;
    using size_type = uint32_t;

    SparsePagedArray(uint16_t maskBitsPerPage, uint16_t valueBitsPerPage, TAllocator const& allocator)
        : mask_(maskBitsPerPage, allocator),
        values_(valueBitsPerPage, allocator)
    {}

    ~SparsePagedArray()
    {
        values_.clearExtern<TAllocator>(*this);
    }

    // Can't copy these for now
    SparsePagedArray(SparsePagedArray const&) = delete;
    SparsePagedArray(SparsePagedArray &&) = delete;

    void set(uint32_t index, T const& value)
    {
        if (mask_[index]) {
            // Value in slot already constructed
            values_[index] = value;
        } else {
            // Need to construct in place, slot is empty
            if (index >= mask_.size()) {
                mask_.resize(index + 1);
                values_.reallocExtern<TAllocator>(index + 1, mask_.allocator());
            }

            mask_.set(index);
            new (&values_[index]) T(value);
        }
    }

    void set(uint32_t index, T&& value)
    {
        if (mask_[index]) {
            // Value in slot already constructed
            values_[index] = std::move(value);
        } else {
            // Need to construct in place, slot is empty
            if (index >= mask_.size()) {
                mask_.resize(index + 1);
                values_.reallocExtern<TAllocator>(index + 1, mask_.allocator());
            }

            mask_.set(index);
            new (&values_[index]) T(std::move(value));
        }
    }

    inline T const* get(uint32_t index) const
    {
        if (mask_[index]) {
            return &values_[index];
        } else {
            return nullptr;
        }
    }

    inline T* get(uint32_t index)
    {
        if (mask_[index]) {
            return &values_[index];
        } else {
            return nullptr;
        }
    }

private:
    PagedBitSet<TAllocator> mask_;
    PagedArray<uint16_t, HashTablePagedAllocator> values_;
};



template <class TKey, class TValue, class TAllocator = DefaultPagedAllocator>
struct DoubleIndexedPagedArray
{
    static constexpr uint16_t InvalidIndex = 0xffff;

    SparsePagedArray<uint16_t, TAllocator> LookupTable;
    PagedArray<TKey, TAllocator> Keys;
    PagedArray<TValue, TAllocator> Values;

    uint16_t FindIndex(TKey const& key) const
    {
        auto hash = (uint32_t)SparseHashMapHash(key);
        auto keyIndex = LookupTable.get(hash);
        if (keyIndex && *keyIndex != InvalidIndex && Keys[*keyIndex] == key) {
            return *keyIndex;
        } else {
            return InvalidIndex;
        }
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
        auto nextIndex = (uint16_t)Keys.size();
        LookupTable.set(hash, nextIndex);
        new (Keys.add_uninitialized()) TKey(key);
        return new (Values.add_uninitialized()) TValue(std::forward<Args>(args)...);
    }
};

END_SE()
