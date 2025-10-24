#pragma once

#include <concurrent_vector.h>
#include <concurrent_queue.h>

BEGIN_NS(lua)

// Generation ID forward declarations
uint32_t get_generation_id(lua_State* L);

class GlobalRefManager : public Noncopyable<GlobalRefManager>
{
public:
    GlobalRefManager(lua_State* L);
    ~GlobalRefManager();

    int32_t AddGlobalIndex(lua_State* L, int local);
    void IncRef(int32_t i);
    void DecRef(int32_t i);
    void Push(lua_State* L, int32_t i);
    int32_t GetGlobalIndex(int32_t i);

private:
    struct GlobalEntry
    {
        inline GlobalEntry(int32_t index)
            : Index(index), RefCount(1)
        {}
        
        inline GlobalEntry()
            : Index(-1), RefCount(0)
        {}
        
        inline GlobalEntry(GlobalEntry const& o)
            : Index(o.Index), RefCount(o.RefCount.load())
        {}

        int32_t Index;
        std::atomic<uint32_t> RefCount;
    };

    lua_State * L_;
    concurrency::concurrent_vector<GlobalEntry> ref_;
    concurrency::concurrent_queue<int32_t> freeList_;
};

class RegistryEntry
{
public:
    RegistryEntry();
    RegistryEntry(lua_State* L, int index);
    RegistryEntry(lua_State* L, Ref const& local);
    ~RegistryEntry();

    RegistryEntry(RegistryEntry const &);
    RegistryEntry(RegistryEntry &&) noexcept;

    RegistryEntry & operator = (RegistryEntry const &);
    RegistryEntry & operator = (RegistryEntry &&) noexcept;

    explicit inline operator bool() const
    {
        return global_ != -1;
    }

    inline int GetRef() const
    {
        return global_;
    }

    void Push(lua_State* L) const;
    void Reset();

private:
    GlobalRefManager* manager_;
    int global_;
};

class PersistentRegistryEntry
{
public:
    PersistentRegistryEntry();
    PersistentRegistryEntry(lua_State * L, int index);
    PersistentRegistryEntry(lua_State * L, Ref const& local);
    ~PersistentRegistryEntry();

    PersistentRegistryEntry(PersistentRegistryEntry const &);
    PersistentRegistryEntry(PersistentRegistryEntry&&) noexcept;

    PersistentRegistryEntry& operator = (PersistentRegistryEntry const &);
    PersistentRegistryEntry& operator = (PersistentRegistryEntry&&);

    bool IsValid(lua_State* L) const;
    bool TryPush(lua_State* L) const;
    Ref ToRef(lua_State* L) const;
    void Bind(lua_State* L, Ref const& ref);

    explicit inline operator bool() const
    {
        return global_ != -1;
    }

    inline int GetGlobalId() const
    {
        return global_;
    }

    inline void ResetWithoutUnbind()
    {
        manager_ = nullptr;
        global_ = -1;
    }

private:
    GlobalRefManager* manager_;
    uint32_t generationId_;
    int global_;

    void Release();
    void Release(lua_State* L);
};

enum class RefType
{
    None = 0,
    Local = 1,
    Registry = 2
};

class Ref
{
public:
    inline Ref()
        : type_(RefType::None), index_(0)
    {}

    inline Ref(lua_State* L, int index)
        : type_(RefType::Local), index_(index)
    {}

    inline Ref(lua_State* L, RefType type, int index)
        : type_(type), index_(index)
    {}

    inline Ref(RegistryEntry const& ref)
        : type_(ref ? RefType::Registry : RefType::None), index_(ref ? ref.GetRef() : 0)
    {}

    inline Ref(Ref const& ref)
        : type_(ref.type_), index_(ref.index_)
    {}

    explicit inline operator bool() const
    {
        return type_ != RefType::None;
    }

    void Push(lua_State* L) const
    {
        EnterVMCheck(L);
        switch (type_) {
        case RefType::None:
            lua_pushnil(L);
            break;

        case RefType::Local:
            lua_pushvalue(L, index_);
            break;

        case RefType::Registry:
            lua_rawgeti(L, LUA_REGISTRYINDEX, index_);
            break;
        }
    }

    inline int Index() const
    {
        return index_;
    }

    inline RefType Type() const
    {
        return type_;
    }

private:
    RefType type_;
    int index_;
};

END_NS()
