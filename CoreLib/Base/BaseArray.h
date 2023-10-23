#pragma once

#include <cstdint>
#include <span>

BEGIN_SE()

template <class T>
class ContiguousIterator
{
public:
	using value_type = T;
	using reference = T&;
	using pointer = T*;
	using difference_type = int32_t;
	using size_type = uint32_t;
	using iterator_category = std::random_access_iterator_tag;

	ContiguousIterator(T* p) : ptr_(p) {}

	ContiguousIterator operator ++ ()
	{
		ContiguousIterator<T> it(ptr_);
		ptr_++;
		return it;
	}

	ContiguousIterator& operator ++ (int)
	{
		ptr_++;
		return *this;
	}

	bool operator == (ContiguousIterator const& it) const
	{
		return it.ptr_ == ptr_;
	}

	bool operator != (ContiguousIterator const& it) const
	{
		return it.ptr_ != ptr_;
	}

	ContiguousIterator operator + (difference_type n) const
	{
		return ContiguousIterator(ptr_ + n);
	}

	ContiguousIterator operator - (difference_type n) const
	{
		return ContiguousIterator(ptr_ - n);
	}

	difference_type operator - (ContiguousIterator const& o) const
	{
		return (difference_type)(ptr_ - o.ptr_);
	}

	T& operator * () const
	{
		return *ptr_;
	}

	T* operator -> () const
	{
		return ptr_;
	}

	T* get () const
	{
		return ptr_;
	}

private:
	T* ptr_;
};


template <class T>
class ContiguousConstIterator
{
public:
	using value_type = T;
	using reference = T const&;
	using pointer = T const*;
	using difference_type = int32_t;
	using size_type = uint32_t;
	using iterator_category = std::random_access_iterator_tag;

	ContiguousConstIterator(T const* p) : ptr_(p) {}

	ContiguousConstIterator operator ++ ()
	{
		ContiguousConstIterator<T> it(ptr_);
		ptr_++;
		return it;
	}

	ContiguousConstIterator& operator ++ (int)
	{
		ptr_++;
		return *this;
	}

	bool operator == (ContiguousConstIterator const& it) const
	{
		return it.ptr_ == ptr_;
	}

	bool operator != (ContiguousConstIterator const& it) const
	{
		return it.ptr_ != ptr_;
	}

	ContiguousConstIterator operator + (difference_type n) const
	{
		return ContiguousConstIterator(ptr_ + n);
	}

	ContiguousConstIterator operator - (difference_type n) const
	{
		return ContiguousConstIterator(ptr_ - n);
	}

	difference_type operator - (ContiguousConstIterator const& o) const
	{
		return (difference_type)(ptr_ - o.ptr_);
	}

	T const& operator * () const
	{
		return *ptr_;
	}

	T const* operator -> () const
	{
		return ptr_;
	}

	T const* get() const
	{
		return ptr_;
	}

private:
	T const* ptr_;
};


template <class T, class Allocator = GameMemoryAllocator, bool StoreSize = false>
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
	uint32_t Capacity{ 0 };
	uint32_t Size{ 0 };

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
		return Buf[index];
	}

	inline T& operator [] (uint32_t index)
	{
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

	void RawReallocate(uint32_t newCapacity)
	{
		if (newCapacity > 0) {
			if (StoreSize) {
				auto newBuf = Allocator::Alloc(newCapacity * sizeof(T) + 8);
				*(uint64_t*)newBuf = newCapacity;

				Buf = (T*)((std::ptrdiff_t)newBuf + 8);
			}
			else {
				Buf = Allocator::New<T>(newCapacity);
			}
		} else {
			Buf = nullptr;
		}

		Capacity = newCapacity;
	}

	void Reallocate(uint32_t newCapacity)
	{
		auto oldBuf = Buf;
		RawReallocate(newCapacity);

		for (uint32_t i = 0; i < std::min(Size, newCapacity); i++) {
			new (Buf + i) T(oldBuf[i]);
		}

		for (uint32_t i = 0; i < Size; i++) {
			oldBuf[i].~T();
		}

		FreeBuffer(oldBuf);
	}

	void remove_at(uint32_t index)
	{
		assert(index < Size);

		for (auto i = index; i < Size - 1; i++) {
			Buf[i] = Buf[i + 1];
		}

		Buf[Size - 1].~T();
		Size--;
	}

	void clear()
	{
		for (uint32_t i = 0; i < Size; i++) {
			Buf[i].~T();
		}

		Size = 0;
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
		assert(this->Size > 0);
		this->Size--;
	}

	T pop_last()
	{
		assert(this->Size > 0);
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

template <unsigned TDWords>
struct BitArray
{
	uint32_t Bits[TDWords];

	inline bool Set(uint32_t index)
	{
		if (index <= 0 || index > (TDWords * 32)) {
			return false;
		}

		Bits[(index - 1) >> 5] |= (1 << ((index - 1) & 0x1f));
		return true;
	}

	inline bool Clear(uint32_t index)
	{
		if (index <= 0 || index > (TDWords * 32)) {
			return false;
		}

		Bits[(index - 1) >> 5] &= ~(1 << ((index - 1) & 0x1f));
		return true;
	}

	inline bool IsSet(uint32_t index) const
	{
		if (index <= 0 || index > (TDWords * 32)) {
			return false;
		}

		return (Bits[(index - 1) >> 5] & (1 << ((index - 1) & 0x1f))) != 0;
	}
};

template <class T>
class StaticArray
{
public:
	using value_type = T;
	using reference = T&;
	using const_reference = T const&;
	using iterator = ContiguousIterator<T>;
	using const_iterator = ContiguousConstIterator<T>;
	using difference_type = int32_t;
	using size_type = uint32_t;

	inline StaticArray() {}
	
	StaticArray(uint32_t size)
	{
		Resize(size);
	}
	
	StaticArray(uint32_t size, T const& initval)
	{
		Resize(size, initval);
	}

	StaticArray(StaticArray const& a)
	{
		CopyFrom(a);
	}

	~StaticArray()
	{
		if (buf_) {
			Resize(0);
		}
	}

	StaticArray& operator =(StaticArray const& a)
	{
		CopyFrom(a);
		return *this;
	}

	void CopyFrom(StaticArray const& a)
	{
		clear();

		if (a.size_ != size_) {
			Resize(a.size_);
			for (uint32_t i = 0; i < size_; i++) {
				new (buf_ + i) T(a[i]);
			}
		}
	}

	inline T* raw_buf() const
	{
		return buf_;
	}

	inline unsigned int size() const
	{
		return size_;
	}

	inline unsigned int Size() const
	{
		return size_;
	}

	inline T const& operator [] (uint32_t index) const
	{
		return buf_[index];
	}

	inline T& operator [] (uint32_t index)
	{
		return buf_[index];
	}

	void clear()
	{
		Resize(0);
	}

	void Resize(uint32_t newSize, T const& initval = T{})
	{
		if (size_ != newSize) {
			T* newBuf;
			if (newSize > 0) {
				newBuf = GameMemoryAllocator::NewRaw<T>(newSize);
			} else {
				newBuf = nullptr;
			}

			for (uint32_t i = 0; i < std::min(size_, newSize); i++) {
				new (newBuf + i) T(buf_[i]);
			}
			
			for (uint32_t i = std::min(size_, newSize); i < newSize; i++) {
				new (newBuf + i) T(initval);
			}

			if (buf_ != nullptr) {
				for (uint32_t i = 0; i < size_; i++) {
					buf_[i].~T();
				}

				GameFree(buf_);
			}

			buf_ = newBuf;
			size_ = newSize;
		}
	}

	iterator begin()
	{
		return iterator(buf_);
	}

	const_iterator begin() const
	{
		return const_iterator(buf_);
	}

	iterator end()
	{
		return iterator(buf_ + size_);
	}

	const_iterator end() const
	{
		return const_iterator(buf_ + size_);
	}

private:
	T* buf_{ nullptr };
	unsigned int size_{ 0 };
};

template <class T>
class Array
{
public:
	using value_type = T;
	using reference = T&;
	using const_reference = T const&;
	using iterator = ContiguousIterator<T>;
	using const_iterator = ContiguousConstIterator<T>;
	using difference_type = int32_t;
	using size_type = uint32_t;

	inline Array() {}

	Array(Array const& a)
	{
		CopyFrom(a);
	}

	~Array()
	{
		if (buf_) {
			clear();
			GameFree(buf_);
		}
	}

	Array& operator =(Array const& a)
	{
		CopyFrom(a);
		return *this;
	}

	void CopyFrom(Array const& a)
	{
		clear();

		if (a.size_ > 0) {
			Reallocate(a.size_);
			size_ = a.size_;
			for (uint32_t i = 0; i < size_; i++) {
				new (buf_ + i) T(a[i]);
			}
		}
	}

	inline T* raw_buf() const
	{
		return buf_;
	}

	inline unsigned int size() const
	{
		return size_;
	}

	inline unsigned int capacity() const
	{
		return capacity_;
	}

	inline unsigned int Size() const
	{
		return size_;
	}

	inline T const& operator [] (uint32_t index) const
	{
		return buf_[index];
	}

	inline T& operator [] (uint32_t index)
	{
		return buf_[index];
	}

	uint32_t CapacityIncrement() const
	{
		if (capacity_ > 0) {
			return 2 * capacity_;
		} else {
			return 1;
		}
	}

	void clear()
	{
		for (uint32_t i = 0; i < size_; i++) {
			buf_[i].~T();
		}

		size_ = 0;
	}

	void Reallocate(uint32_t newCapacity)
	{
		auto newBuf = GameMemoryAllocator::NewRaw<T>(newCapacity);
		for (uint32_t i = 0; i < std::min(size_, newCapacity); i++) {
			new (newBuf + i) T(buf_[i]);
		}

		if (buf_ != nullptr) {
			for (uint32_t i = 0; i < size_; i++) {
				buf_[i].~T();
			}

			GameFree(buf_);
		}

		buf_ = newBuf;
		capacity_ = newCapacity;
	}

	void Add(T const& value)
	{
		if (capacity_ <= size_) {
			Reallocate(CapacityIncrement());
		}

		new (&buf_[size_++]) T(value);
	}

	void push_back(T const& value)
	{
		if (capacity_ <= size_) {
			Reallocate(CapacityIncrement());
		}

		new (&buf_[size_++]) T(value);
	}

	void remove_at(uint32_t index)
	{
		assert(index < size_);

		for (auto i = index; i < size_ - 1; i++) {
			buf_[i] = buf_[i + 1];
		}

		buf_[size_ - 1].~T();
		size_--;
	}

	void remove_last()
	{
		assert(size_ > 0);
		buf_[size_ - 1].~T();
		size_--;
	}

	T pop_last()
	{
		assert(size_ > 0);
		return std::move(buf_[--size_]);
	}

	iterator begin()
	{
		return iterator(buf_);
	}

	const_iterator begin() const
	{
		return const_iterator(buf_);
	}

	iterator end()
	{
		return iterator(buf_ + size_);
	}

	const_iterator end() const
	{
		return const_iterator(buf_ + size_);
	}

private:
	T* buf_{ nullptr };
	unsigned int capacity_{ 0 };
	unsigned int size_{ 0 };
};

END_SE()
