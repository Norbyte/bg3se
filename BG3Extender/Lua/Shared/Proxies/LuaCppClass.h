#pragma once

BEGIN_NS(lua)

struct CppObjectMetadata
{
    union
    {
        void* Ptr;
        uint64_t Value;
    };
    MetatableTag MetatableTag;
    uint16_t PropertyMapTag;
    LifetimeHandle Lifetime;
};

struct CppObjectOpaque : public ProtectedGameObject<CppObjectOpaque> {};
struct CppValueOpaque : public ProtectedGameObject<CppValueOpaque> {};

class CppMetatableManager
{
public:
    CppMetatableManager();
    void RegisterMetatable(MetatableTag tag, CMetatable* mt);
    CMetatable* GetMetatable(MetatableTag tag);

    static CppMetatableManager& FromLua(lua_State* L);

private:
    Array<CMetatable *> metatables_;
};

// NOTE: Must match TMS enumeration in Lua
enum class MetamethodName : int
{
    Index,
    NewIndex,
    GC,
    Mode,
    Len,
    Eq,
    Add,
    Sub,
    Mul,
    Mod,
    Pow,
    Div,
    IDiv,
    BAnd,
    BOr,
    BXor,
    Shl,
    Shr,
    Unm,
    BNot,
    Lt,
    Le,
    Concat,
    Call,
    Pairs,
    ToString,
    Name
};

END_NS()
