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
        if (!mask_[index]) {
            return nullptr;
        } else {
            se_assert(index < values_.size());
            return &values_[index];
        }
    }

    inline T* try_get(size_type index)
    {
        if (!mask_[index]) {
            return nullptr;
        } else {
            se_assert(index < values_.size());
            return &values_[index];
        }
    }

    void clear()
    {
        mask_.clear();
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

END_SE()
