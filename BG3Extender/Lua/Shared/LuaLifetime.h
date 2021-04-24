#pragma once

BEGIN_NS(lua)

template <class T, unsigned Size>
class HierarchicalPoolAllocator
{
public:
	static constexpr unsigned PageBits = 64;
	static constexpr unsigned PageShift = 6;

	HierarchicalPoolAllocator()
	{
		pool_ = new T[Size];
		memset(l1_, 0xff, sizeof(l1_));
		memset(l2_, 0xff, sizeof(l2_));
		memset(l3_, 0xff, sizeof(l3_));

		auto l1buckets = Size / (PageBits * PageBits);
		if (l1buckets % PageBits) {
			l1_[std::size(l1_) - 1] = 0xffffffffffffffffull << (PageBits - (l1buckets & (PageBits - 1)));
		}
	}

	~HierarchicalPoolAllocator()
	{
		delete [] pool_;
	}

	T* Allocate()
	{
		for (auto i = 0; i < std::size(l1_); i++) {
			auto l1 = __lzcnt64(l1_[i]);
			if (l1 < PageBits) {
				auto l2off = (i << PageShift) + l1;
				auto l2 = __lzcnt64(l2_[l2off]);
				assert(l2 < PageBits);
				auto l3off = (i << (2 * PageShift)) + (l1 << PageShift) + l2;
				auto l3 = __lzcnt64(l3_[l3off]);
				assert(l3 < PageBits);
				l3_[l3off] &= ~(0x8000000000000000ull >> l3);
				if (l3_[l3off] == 0) {
					l2_[l2off] &= ~(0x8000000000000000ull >> l2);
					if (l2_[l2off] == 0) {
						l1_[i] &= ~(0x8000000000000000ull >> l1);
					}
				}

				auto off = (i << (3 * PageShift)) + (l1 << (2 * PageShift)) + (l2 << PageShift) + l3;
				new (pool_ + off) T();
				return pool_ + off;
			}
		}

		return nullptr;
	}

	void Free(T* ptr)
	{
		assert(ptr >= pool_ && ptr < pool_ + Size);
		auto index = ptr - pool_;
		auto off = index;
		auto l3 = index & (PageBits - 1);
		index >>= PageShift;
		auto l3off = index;
		auto l2 = index & (PageBits - 1);
		index >>= PageShift;
		auto l2off = index;
		auto l1 = index & (PageBits - 1);
		index >>= PageShift;
		auto l1off = index;

		bool l3set = (l3_[l3off] == 0);
		l3_[l3off] |= 0x8000000000000000ull >> l3;
		if (l3set) {
			bool l2set = (l2_[l2off] == 0);
			l2_[l2off] |= 0x8000000000000000ull >> l2;
			if (l2set) {
				l1_[l1off] |= 0x8000000000000000ull >> l1;
			}
		}

		ptr->~T();
	}

private:
	static_assert((Size % 4096) == 0, "Size must be a multiple of 4096");

	uint64_t l1_[Size / 262144 + ((Size % 262144) ? 1 : 0)];
	uint64_t l2_[Size / 4096];
	uint64_t l3_[Size / 64];
	T* pool_;
};

class LifetimePool;

class Lifetime : public Noncopyable<Lifetime>
{
public:
	Lifetime()
		: isAlive_(true), references_(0)
	{}

	inline bool IsAlive() const
	{
		return isAlive_;
	}

	inline unsigned References() const
	{
		return references_;
	}

	inline void Kill()
	{
		assert(isAlive_);
		isAlive_ = false;
	}

protected:
	friend class LifetimeReference;

	void AddRef()
	{
		references_++;
	}

	unsigned DecRef()
	{
		assert(references_ > 0);
		return --references_;
	}

private:
	std::atomic<unsigned> references_;
	bool isAlive_;
};

class LifetimePool : Noncopyable<LifetimePool>
{
public:
	inline Lifetime* Allocate()
	{
		return pool_.Allocate();
	}

	inline void Release(Lifetime* lifetime)
	{
		assert(lifetime->References() == 0);
		pool_.Free(lifetime);
	}

private:
	HierarchicalPoolAllocator<Lifetime, 262144> pool_;
};

class LifetimeHolder
{
public:
	inline LifetimeHolder(LifetimePool& pool, Lifetime* lifetime)
		: lifetime(lifetime), pool(pool)
	{}

	LifetimeHolder(LifetimeHolder&& o) noexcept
		: lifetime(o.lifetime), pool(o.pool)
	{}

	LifetimeHolder(LifetimeHolder const& o)
		: lifetime(o.lifetime), pool(o.pool)
	{}

	Lifetime* lifetime;
	LifetimePool& pool;
};

class LifetimeReference
{
public:
	inline LifetimeReference(LifetimePool& pool, Lifetime* lifetime)
		: lifetime_(lifetime), pool_(pool)
	{
		if (lifetime_) {
			lifetime_->AddRef();
		}
	}

	inline ~LifetimeReference()
	{
		if (lifetime_ && lifetime_->DecRef() == 0) {
			pool_.Release(lifetime_);
		}
	}

	LifetimeReference(LifetimeReference&& o) noexcept
		: lifetime_(o.lifetime_), pool_(o.pool_)
	{
		o.lifetime_ = nullptr;
	}

	LifetimeReference(LifetimeReference const& o)
		: lifetime_(o.lifetime_), pool_(o.pool_)
	{
		if (lifetime_) {
			lifetime_->AddRef();
		}
	}

	LifetimeReference(LifetimeHolder&& o) noexcept
		: lifetime_(o.lifetime), pool_(o.pool)
	{}

	LifetimeReference(LifetimeHolder const& o)
		: lifetime_(o.lifetime), pool_(o.pool)
	{
		if (lifetime_) {
			lifetime_->AddRef();
		}
	}

	LifetimeReference& operator = (LifetimeReference const&) = delete;
	LifetimeReference& operator = (LifetimeReference &&) = delete;

	inline bool IsAlive() const
	{
		return lifetime_ && lifetime_->IsAlive();
	}

	inline LifetimeHolder Get() const
	{
		return LifetimeHolder(pool_, lifetime_);
	}

	inline Lifetime* GetLifetime() const
	{
		return lifetime_;
	}

private:
	Lifetime* lifetime_;
	LifetimePool& pool_;
};

class LifetimeStack
{
public:
	LifetimeStack(LifetimePool& pool)
		: pool_(pool)
	{}

	bool IsEmpty() const
	{
		return stack_.empty();
	}

	Lifetime* Push()
	{
		assert(stack_.size() < 0x1000);
		auto lifetime = pool_.Allocate();
		stack_.push_back(LifetimeReference(pool_, lifetime));
		return lifetime;
	}

	void Pop()
	{
		assert(!stack_.empty());
		auto lifetime = stack_.rbegin()->GetLifetime();
		if (lifetime != nullptr) {
			lifetime->Kill();
		}
		stack_.pop_back();
	}

	void Pop(Lifetime* lifetime)
	{
		assert(!stack_.empty());
		assert(stack_.rbegin()->GetLifetime() == lifetime);
		if (lifetime != nullptr) {
			lifetime->Kill();
		}
		stack_.pop_back();
	}

	inline LifetimeHolder GetCurrent() const
	{
		assert(!stack_.empty());
		return stack_.rbegin()->Get();
	}

private:
	LifetimePool& pool_;
	std::vector<LifetimeReference> stack_;
};

class LifetimePin
{
public:
	LifetimePin(LifetimeStack& stack)
		: stack_(stack)
	{
		lifetime_ = stack_.Push();
	}

	~LifetimePin()
	{
		stack_.Pop(lifetime_);
	}

private:
	LifetimeStack& stack_;
	Lifetime* lifetime_;

};

template <class T>
class LifetimeTrackedObject : public Noncopyable<LifetimeTrackedObject<T>>
{
public:
	inline LifetimeTrackedObject(T* obj, LifetimePool& lifetimePool, Lifetime* lifetime)
		: object_(obj), reference_(lifetimePool, lifetime)
	{}
	
	inline LifetimeTrackedObject(T* obj, LifetimeReference const& lifetime)
		: object_(obj), reference_(lifetime)
	{}
	
	inline LifetimeTrackedObject(T* obj, LifetimeHolder const& lifetime)
		: object_(obj), reference_(lifetime)
	{}

	inline T* Get() const
	{
		if (reference_.IsAlive()) {
			return object_;
		} else {
			return nullptr;
		}
	}

	inline LifetimeHolder GetLifetime() const
	{
		return reference_.Get();
	}

private:
	T* object_;
	LifetimeReference reference_;
};

END_NS()
