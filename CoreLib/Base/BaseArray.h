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
	
	StaticArray(size_type size)
	{
		Resize(size);
	}
	
	StaticArray(size_type size, T const& initval)
	{
		Resize(size, initval);
	}

	StaticArray(StaticArray const& a)
	{
		CopyFrom(a);
	}

	StaticArray(StaticArray&& a)
	{
		if (this != &a) {
			buf_ = a.buf_;
			size_ = a.size_;
			a.buf_ = nullptr;
			a.size_ = 0;
		}
	}

	~StaticArray()
	{
		if (buf_) {
			for (size_type i = 0; i < size_; i++) {
				buf_[i].~T();
			}

			GameFree(buf_);
		}
	}

	StaticArray& operator =(StaticArray const& a)
	{
		CopyFrom(a);
		return *this;
	}

	StaticArray& operator =(StaticArray&& a)
	{
		if (this != &a) {
			buf_ = a.buf_;
			size_ = a.size_;
			a.buf_ = nullptr;
			a.size_ = 0;
		}

		return *this;
	}

	void CopyFrom(StaticArray const& a)
	{
		clear();

		if (a.size_ != size_) {
			Resize(a.size_);
			for (size_type i = 0; i < size_; i++) {
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

	inline T const& operator [] (size_type index) const
	{
		return buf_[index];
	}

	inline T& operator [] (size_type index)
	{
		return buf_[index];
	}

	void clear()
	{
		Resize(0);
	}

	void Resize(size_type newSize, T const& initval)
	{
		if (size_ != newSize) {
			T* newBuf;
			if (newSize > 0) {
				newBuf = GameMemoryAllocator::NewRaw<T>(newSize);
			} else {
				newBuf = nullptr;
			}

			for (size_type i = 0; i < std::min(size_, newSize); i++) {
				new (newBuf + i) T(std::move(buf_[i]));
			}
			
			for (size_type i = std::min(size_, newSize); i < newSize; i++) {
				new (newBuf + i) T(initval);
			}

			if (buf_ != nullptr) {
				for (size_type i = 0; i < size_; i++) {
					buf_[i].~T();
				}

				GameFree(buf_);
			}

			buf_ = newBuf;
			size_ = newSize;
		}
	}

	void Resize(size_type newSize)
	{
		if (size_ != newSize) {
			T* newBuf;
			if (newSize > 0) {
				newBuf = GameMemoryAllocator::NewRaw<T>(newSize);
			} else {
				newBuf = nullptr;
			}

			for (size_type i = 0; i < std::min(size_, newSize); i++) {
				new (newBuf + i) T(std::move(buf_[i]));
			}
			
			for (size_type i = std::min(size_, newSize); i < newSize; i++) {
				new (newBuf + i) T();
			}

			if (buf_ != nullptr) {
				for (size_type i = 0; i < size_; i++) {
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
	size_type size_{ 0 };
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

	inline constexpr Array() noexcept {}

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
			for (size_type i = 0; i < size_; i++) {
				new (buf_ + i) T(a[i]);
			}
		}
	}

	inline constexpr T* raw_buf() const noexcept
	{
		return buf_;
	}

	inline constexpr bool empty() const noexcept
	{
		return size_ == 0;
	}

	inline constexpr size_type size() const noexcept
	{
		return size_;
	}

	inline constexpr size_type capacity() const noexcept
	{
		return capacity_;
	}

	inline constexpr size_type Size() const noexcept
	{
		return size_;
	}

	inline T const& operator [] (size_type index) const
	{
		return buf_[index];
	}

	inline T& operator [] (size_type index)
	{
		return buf_[index];
	}

	constexpr size_type CapacityIncrement() const noexcept
	{
		if (capacity_ > 0) {
			return 2 * capacity_;
		} else {
			return 1;
		}
	}

	void clear()
	{
		for (size_type i = 0; i < size_; i++) {
			buf_[i].~T();
		}

		size_ = 0;
	}

	void Reallocate(size_type newCapacity)
	{
		auto newBuf = GameMemoryAllocator::NewRaw<T>(newCapacity);
		for (size_type i = 0; i < std::min(size_, newCapacity); i++) {
			new (newBuf + i) T(buf_[i]);
		}

		if (buf_ != nullptr) {
			for (size_type i = 0; i < size_; i++) {
				buf_[i].~T();
			}

			GameFree(buf_);
		}

		buf_ = newBuf;
		capacity_ = newCapacity;
	}

	void resize(size_type newSize)
	{
		if (newSize > capacity_) {
			Reallocate(newSize);
		}

		if (size_ > newSize) {
			for (size_type i = newSize; i < size_; i++) {
				buf_[i].~T();
			}
		} else {
			for (size_type i = size_; i < newSize; i++) {
				new (buf_ + i) T();
			}
		}

		size_ = newSize;
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

	void ordered_insert_at(size_type index, T const& value)
	{
		assert(index <= size_);
		if (capacity_ <= size_) {
			Reallocate(CapacityIncrement());
		}

		new (&buf_[size_++]) T();

		for (size_type i = size_ - 1; i > index; i++) {
			buf_[i] = buf_[i - 1];
		}

		buf_[index] = value;
	}

	void insert_at(size_type index, T const& value)
	{
		assert(index <= size_);
		if (capacity_ <= size_) {
			Reallocate(CapacityIncrement());
		}

		new (&buf_[size_]) T(buf_[index]);
		buf_[index] = value;
		size_++;
	}

	void remove_at(size_type index)
	{
		assert(index < size_);

		for (size_type i = index; i < size_ - 1; i++) {
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
	size_type capacity_{ 0 };
	size_type size_{ 0 };
};


template <class T>
class Queue
{
public:
	using value_type = T;
	using reference = T&;
	using const_reference = T const&;
	using difference_type = int32_t;
	using size_type = int32_t;

	inline Queue() {}

	Queue(Queue const& o)
	{
		copy_from(o);
	}

	Queue(Queue&& o)
		: buf_(o.buf_),
		capacity_(o.capacity_),
		size_(o.size_),
		readIndex_(o.readIndex_),
		writeIndex_(o.writeIndex_)
	{
		o.buf_ = nullptr;
		o.capacity_ = 0;
		o.readIndex_ = -1;
		o.writeIndex_ = 0;
	}

	~Queue()
	{
		if (buf_) {
			GameDeleteArray<T>(buf_, capacity_);
		}
	}

	Queue& operator =(Queue const& o)
	{
		copy_from(o);
		return *this;
	}

	Queue& operator =(Queue&& o)
	{
		buf_ = o.buf_;
		capacity_ = o.capacity_;
		size_ = o.size_;
		readIndex_ = o.readIndex_;
		writeIndex_ = o.writeIndex_;

		o.buf_ = nullptr;
		o.capacity_ = 0;
		o.size_ = 0;
		o.readIndex_ = -1;
		o.writeIndex_ = 0;
		return *this;
	}

	size_type size() const
	{
		return size_;
	}

	bool empty() const
	{
		return size_ == 0;
	}

	size_type capacity() const
	{
		return capacity_;
	}

	inline T const& operator [] (size_type index) const
	{
		return buf_[(readIndex_ + index) % capacity_];
	}

	inline T& operator [] (size_type index)
	{
		return buf_[(readIndex_ + index) % capacity_];
	}

	void push_back(T const& value)
	{
		if (capacity_ == 0 || (size_ > 0 && readIndex_ == writeIndex_)) {
			resize(capacity_increment());
		}

		if (size_ == 0) {
			buf_[0] = value;
			size_ = 1;
			writeIndex_ = 1;
			readIndex_ = 0;
		} else {
			buf_[writeIndex_] = value;
			size_++;
			writeIndex_++;
		}

		if (writeIndex_ == capacity_) {
			writeIndex_ = 0;
		}
	}

	T pop()
	{
		assert(size_ > 0);

		T val = buf_[readIndex_];
		size_--;
		if (++readIndex_ == capacity_) {
			readIndex_ = 0;
		}

		if (readIndex_ == writeIndex_) {
			readIndex_ = -1;
			writeIndex_ = 0;
		}

		return val;
	}

	void resize(size_type newCapacity)
	{
		auto newBuf = GameAllocArray<T>(newCapacity);

		if (readIndex_ < writeIndex_) {
			auto wr = 0;
			for (size_type i = readIndex_ + 1; i < writeIndex_; i++, wr++) {
				newBuf[wr] = buf_[i];
			}
		} else {
			auto wr = 0;
			for (size_type i = readIndex_ + 1; i < capacity_; i++, wr++) {
				newBuf[wr] = buf_[i];
			}

			for (size_type i = 0; i < writeIndex_; i++, wr++) {
				newBuf[wr] = buf_[i];
			}
		}

		size_ = std::min(size_, capacity_);
		for (size_type i = 0; i < std::min(size_, newCapacity); i++) {
			newBuf[i] = buf_[i];
		}

		if (buf_ != nullptr) {
			GameDeleteArray<T>(buf_, capacity_);
		}

		buf_ = newBuf;
		capacity_ = newCapacity;
		readIndex_ = 0;
		writeIndex_ = size_;
	}

	void clear()
	{
		size_ = 0; 
		readIndex_ = 0;
		writeIndex_ = 0;
	}

private:
	T* buf_{ nullptr };
	size_type capacity_{ 0 };
	size_type size_{ 0 };
	difference_type readIndex_{ -1 };
	difference_type writeIndex_{ 0 };

	void copy_from(Queue const& a)
	{
		readIndex_ = a.readIndex_;
		writeIndex_ = a.writeIndex_;

		reallocate(a.capacity_);
		size_ = a.size_;
		for (size_type i = 0; i < capacity_; i++) {
			buf_[i] = a.buf_[i];
		}
	}

	size_type capacity_increment() const
	{
		if (capacity_ > 0) {
			return 2 * capacity_;
		} else {
			return 1;
		}
	}
};

END_SE()
