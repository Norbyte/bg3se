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
		assert(index < mask_.size());
		assert(index < values_.size());
		assert(mask_[index]);
		return values_[index];
	}

	inline T& operator [] (size_type index)
	{
		assert(index < mask_.size());
		assert(index < values_.size());
		assert(mask_[index]);
		return values_[index];
	}

	void clear()
	{
		mask_.clear();
		values_.clear();
	}

	T& push_back(T const& value)
	{
		mask_.push_back(true);
		return values_.push_back(value);
	}

	T& push_back(T&& value)
	{
		mask_.push_back(true);
		return values_.push_back(std::move(value));
	}

private:
	BitSet<> mask_;
	Array<T> values_;
};

END_SE()
