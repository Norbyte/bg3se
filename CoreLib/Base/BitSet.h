#pragma once

BEGIN_SE()

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

	BitSet(BitSet&& other)
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

	BitSet& operator = (BitSet&& other)
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

END_SE()
