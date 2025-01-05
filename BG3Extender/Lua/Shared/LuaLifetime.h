#pragma once

#undef DEBUG_LIFETIMES
#undef TRACE_LIFETIMES

BEGIN_NS(lua)

template <class T, std::size_t Size>
class HierarchicalPoolAllocator
{
public:
    static constexpr unsigned PageBits = 64;
    static constexpr unsigned PageShift = 6;

    HierarchicalPoolAllocator()
    {
        pool_ = new T[Size];
        memset(pool_, 0, sizeof(T) * Size);
        memset(l1_, 0xff, sizeof(l1_));
        memset(l2_, 0xff, sizeof(l2_));
        memset(l3_, 0xff, sizeof(l3_));

        auto l1buckets = Size / (PageBits * PageBits);
        if (l1buckets % PageBits) {
            l1_[std::size(l1_) - 1] = 0xffffffffffffffffull << (PageBits - (l1buckets & (PageBits - 1)));
        }

        for (std::size_t i = 0; i < Size; i++) {
            pool_[i].SetIndex(i);
        }

        // Reserve lifetime #0 for 'null' lifetimes
        l3_[0] &= ~1;
    }

    ~HierarchicalPoolAllocator()
    {
        delete [] pool_;
    }

    T* Allocate()
    {
        for (auto i = 0; i < std::size(l1_); i++) {
            unsigned long l1;
            if (_BitScanForward64(&l1, l1_[i])) {
                auto l2off = (i << PageShift) + l1;
                unsigned long l2;
                _BitScanForward64(&l2, l2_[l2off]);
                assert(l2 < PageBits);
                auto l3off = (i << (2 * PageShift)) + (l1 << PageShift) + l2;
                unsigned long l3;
                _BitScanForward64(&l3, l3_[l3off]);
                assert(l3 < PageBits);
                l3_[l3off] &= ~(1ull << l3);
                if (l3_[l3off] == 0) {
                    l2_[l2off] &= ~(1ull << l2);
                    if (l2_[l2off] == 0) {
                        l1_[i] &= ~(1ull << l1);
                    }
                }

                auto off = (i << (3 * PageShift)) + (l1 << (2 * PageShift)) + (l2 << PageShift) + l3;
#if defined(TRACE_LIFETIMES)
                INFO("ACQ: off=%d, root=%d, l1=%d, l2=%d, l3=%d, l2off=%d, l3off=%d", off, i, l1, l2, l3, l2off, l3off);
#endif

                auto lifetime = pool_ + off;
                lifetime->Acquire();
                return lifetime;
            }
        }

        OsiErrorS("Couldn't allocate Lua lifetime - pool is full! This is very, very bad.");
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
        l3_[l3off] |= 1ull << l3;
        if (l3set) {
            bool l2set = (l2_[l2off] == 0);
            l2_[l2off] |= 1ull << l2;
            if (l2set) {
                l1_[l1off] |= 1ull << l1;
            }
        }

        ptr->Release();
    }

    T* Get(std::size_t index) const
    {
        assert(index < Size);
        return pool_ + index;
    }

public:
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
    static constexpr unsigned HandleBits = 48;
    static constexpr unsigned IndexBits = 18;
    static constexpr unsigned SaltBits = (HandleBits - IndexBits);

    static constexpr uint64_t IndexMask = (1ull << IndexBits) - 1;
    static constexpr uint64_t SaltMask = (1ull << SaltBits) - 1;
    static constexpr uint64_t HandleMask = (1ull << HandleBits) - 1;

    static constexpr uint64_t AliveBit = (1ull << HandleBits);

    inline Lifetime()
    {}

    inline ~Lifetime()
    {
        assert(!IsAlive());
    }

    inline void Acquire()
    {
        assert(!IsAlive());
        
        data_ = Index()
            | (((Salt() + 1) & SaltMask) << IndexBits)
            | AliveBit;

#if defined(TRACE_LIFETIMES)
        INFO("[%p] ACQUIRE", this);
#endif
    }

    inline void Release()
    {
#if defined(DEBUG_LIFETIMES)
        if (!IsAlive()) {
            ERR("[%p] Attempted to release a lifetime that is not alive.", this);
        }
#endif

        assert(IsAlive());
        data_ &= ~AliveBit;
#if defined(TRACE_LIFETIMES)
        INFO("[%p] RELEASE", this);
#endif
    }

    inline bool IsAlive() const
    {
        return data_ & AliveBit;
    }

    inline uint64_t Index() const
    {
        return (data_ & IndexMask);
    }

    inline uint64_t Salt() const
    {
        return (data_ >> IndexBits) & SaltMask;
    }

    inline void SetIndex(std::size_t i)
    {
        data_ = (uint32_t)i;
    }

    inline uint64_t Data() const
    {
        return data_;
    }

protected:
    friend struct LifetimeHandle;
    friend class LifetimePin;
    friend class StaticLifetimePin;

private:
    uint64_t data_{ 0 };
};


struct LifetimeHandle
{
    static constexpr unsigned HandleBits = Lifetime::HandleBits;
    static constexpr unsigned IndexBits = Lifetime::IndexBits;
    static constexpr unsigned SaltBits = Lifetime::SaltBits;

    static constexpr uint64_t IndexMask = Lifetime::IndexMask;
    static constexpr uint64_t SaltMask = Lifetime::SaltMask;
    static constexpr uint64_t HandleMask = Lifetime::HandleMask;

    static constexpr unsigned MaxPoolSize = 1 << IndexBits;

    static constexpr uint64_t NullHandle = 0ull;

    uint64_t handle_;

    explicit inline LifetimeHandle() = default;
    inline LifetimeHandle(LifetimeHandle const& oh) = default;
    inline LifetimeHandle& operator = (LifetimeHandle const& oh) = default;

    explicit inline constexpr LifetimeHandle(uint64_t handle)
        : handle_(handle)
    {}

    explicit inline LifetimeHandle(uint32_t index, uint32_t salt)
        : handle_(((uint64_t)index & IndexMask) | (((uint64_t)salt & SaltMask) << IndexBits))
    {}

    explicit inline LifetimeHandle(Lifetime const* lifetime)
        : handle_(MakeHandle(lifetime))
    {}

    static inline uint64_t MakeHandle(Lifetime const* lifetime)
    {
        if (lifetime) {
            return (uint64_t)lifetime->Index() | (((uint64_t)lifetime->Salt() & SaltMask) << IndexBits);
        } else {
            return NullHandle;
        }
    }

    inline bool operator == (LifetimeHandle const& oh) const
    {
        return handle_ == oh.handle_;
    }

    explicit inline operator bool() const
    {
        return handle_ != NullHandle;
    }

    inline bool operator !() const
    {
        return handle_ == NullHandle;
    }

    explicit inline operator uint64_t() const
    {
        return (uint64_t)handle_;
    }

    inline uint32_t GetIndex() const
    {
        return (handle_ & IndexMask);
    }

    inline uint32_t GetSalt() const
    {
        return (handle_ >> IndexBits) & SaltMask;
    }

    inline bool MatchesLifetime(Lifetime const& l) const
    {
        return (handle_ | Lifetime::AliveBit) == l.Data();
    }

    bool IsAlive(lua_State* L) const;
    Lifetime* GetLifetime(lua_State* L) const;
};

static constexpr LifetimeHandle NullLifetime{ LifetimeHandle ::NullHandle };

static_assert(std::is_pod_v<LifetimeHandle>);

class LifetimePool : Noncopyable<LifetimePool>
{
public:
    inline LifetimeHandle Allocate()
    {
        return LifetimeHandle(pool_.Allocate());
    }

    inline void DebugPrintMismatch(LifetimeHandle handle, Lifetime const* ref) const
    {
#if defined(DEBUG_LIFETIMES)
        if (ref->Index() != handle.GetIndex()) {
            ERR("[%012lx] Lifetime index mismatch; got %d, expected %d", (uint64_t)handle, ref->Index(), handle.GetIndex());
        }

        if (ref->Salt() != handle.GetSalt()) {
            ERR("[%012lx] Lifetime salt mismatch; got %d, expected %d", (uint64_t)handle, ref->Salt(), handle.GetSalt());
        }

        if (!ref->IsAlive()) {
            ERR("[%012lx] Attempted to get a dead lifetime.", (uint64_t)handle);
        }
#endif
    }

    inline Lifetime* Get(LifetimeHandle handle)
    {
        auto ref = pool_.Get(handle.GetIndex());

        if (!handle.MatchesLifetime(*ref)) [[unlikely]] {
            DebugPrintMismatch(handle, ref);
            return nullptr;
        }

        return ref;
    }

    inline bool IsValid(LifetimeHandle handle) const
    {
        auto ref = pool_.Get(handle.GetIndex());

        if (!handle.MatchesLifetime(*ref)) [[unlikely]] {
            DebugPrintMismatch(handle, ref);
            return false;
        }

        return true;
    }

    inline void Release(LifetimeHandle handle)
    {
        auto ref = Get(handle);
        if (ref) {
            pool_.Free(ref);
        }
    }

    inline auto const& GetAllocator() const
    {
        return pool_;
    }

private:
    HierarchicalPoolAllocator<Lifetime, LifetimeHandle::MaxPoolSize> pool_;
};

class LifetimeStack
{
public:
    inline LifetimeStack(LifetimePool& pool)
        : pool_(pool)
    {}

    inline LifetimePool& Pool()
    {
        return pool_;
    }

    inline bool IsEmpty() const
    {
        return stack_.empty();
    }

    inline LifetimeHandle Push()
    {
        assert(stack_.size() < 0x1000);
        auto lifetime = pool_.Allocate();
        stack_.push_back(lifetime);
        return lifetime;
    }

    inline void Push(LifetimeHandle lifetime)
    {
        assert(stack_.size() < 0x1000);
        stack_.push_back(lifetime);
    }

    inline void PopAndKill()
    {
        assert(!stack_.empty());
        auto lifetime = *stack_.rbegin();
        pool_.Release(lifetime);
        stack_.pop_back();
    }

    inline void PopAndKill(LifetimeHandle lifetime)
    {
        assert(!stack_.empty());
        assert(*stack_.rbegin() == lifetime);
        pool_.Release(lifetime);
        stack_.pop_back();
    }

    inline void Pop(LifetimeHandle lifetime)
    {
        assert(!stack_.empty());
        assert(*stack_.rbegin() == lifetime);
        stack_.pop_back();
    }

    inline LifetimeHandle GetCurrent() const
    {
        assert(!stack_.empty());
        return *stack_.rbegin();
    }

private:
    LifetimePool& pool_;
    Vector<LifetimeHandle> stack_;
};

// RAII lifetime stack guard; 
// Allocates a lifetime object on construction and pushes it onto the top of the lifetime stack;
// removes and deletes the lifetime on destruction
class LifetimeStackPin : Noncopyable<LifetimeStackPin>
{
public:
    LifetimeStackPin(lua_State* L, LifetimeStack& stack);
    LifetimeStackPin(lua_State* L);

    inline ~LifetimeStackPin()
    {
        stack_.PopAndKill(currentLifetime_);
        currentLifetime_ = previousLifetime_;
    }

    inline LifetimeHandle GetLifetime() const
    {
        return currentLifetime_;
    }

private:
    LifetimeStack& stack_;
    LifetimeHandle& currentLifetime_;
    LifetimeHandle previousLifetime_;
};


// RAII lifetime stack guard; 
// Pushes the specified a lifetime object to the top of the lifetime stack on construction;
// removes it from the stack on destruction (does not destroy the lifetime!)
class StaticLifetimeStackPin : Noncopyable<StaticLifetimeStackPin>
{
public:
    StaticLifetimeStackPin(lua_State* L, LifetimeHandle lifetime);

    inline ~StaticLifetimeStackPin()
    {
        stack_.Pop(currentLifetime_);
        currentLifetime_ = previousLifetime_;
    }

    inline LifetimeHandle GetLifetime() const
    {
        return currentLifetime_;
    }

private:
    LifetimeStack& stack_;
    LifetimeHandle& currentLifetime_;
    LifetimeHandle previousLifetime_;
};


// RAII lifetime owner guard; 
// Allocates a lifetime object on construction; deletes the lifetime on destruction
class LifetimeOwnerPin : Noncopyable<LifetimeOwnerPin>
{
public:
    inline LifetimeOwnerPin(LifetimePool& pool)
        : pool_(pool), lifetime_(pool_.Allocate())
    {}
    
    inline LifetimeOwnerPin(LifetimePool& pool, LifetimeHandle lifetime)
        : pool_(pool), lifetime_(lifetime)
    {}
    
    LifetimeOwnerPin(lua_State* L);
    LifetimeOwnerPin(lua_State* L, LifetimeHandle lifetime);

    inline ~LifetimeOwnerPin()
    {
        pool_.Release(lifetime_);
    }

    inline LifetimeHandle GetLifetime() const
    {
        return lifetime_;
    }

    inline operator LifetimeHandle() const
    {
        return lifetime_;
    }

private:
    LifetimePool& pool_;
    LifetimeHandle lifetime_;
};

END_NS()
