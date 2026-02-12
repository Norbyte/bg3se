#pragma once

BEGIN_NS(gn)

struct GenomeVariant;
struct GenomeTypeManager;

using SequenceIndex = uint16_t;
using NodeIndex = uint16_t;
using VariableIndex = uint16_t;
using EGenomeExecutionResult = uint32_t;

struct [[bg3::hidden]] GenomeVarTypeDesc : public ProtectedGameObject<GenomeVarTypeDesc>
{
    using AssignProc = void (GenomeVariant* variant, GenomeVariant const& other);
    using InitProc = void (GenomeVariant* this_, GenomeVariant const& other);
    using VisitProc = bool (ObjectVisitor* visitor, FixedString const& node, GenomeVariant* variant);
    using FromStringProc = void (GenomeVariant* variant, char const* str);
    using ToStringProc = void (GenomeVariant* variant, STDString* out);
    using DestroyProc = void (void* data);
    using ComparatorProc = void (void* this_, void* other);

    AssignProc* Assign;
    InitProc* Init;
    VisitProc* Visit;
    FromStringProc* FromString;
    ToStringProc* ToString;
    DestroyProc* Destroy;
    ComparatorProc* Comparator;

    uint32_t TypeHash;
    uint32_t TypeHash2;
    uint64_t Size;
    uint64_t Alignment;
    FixedString TypeName;
    bool Trivial;
    GenomeVariant* DefaultValue;
};

struct [[bg3::hidden]] GenomeVariant
{
public:
    void* Value{ nullptr };
    GenomeVarTypeDesc* Type{ nullptr };
#if 0
    // Only present in editor build
    // bool Assigned;
    // bool Constant;
#endif

    GenomeVariant();
    GenomeVariant(lua_State* L, int index);
    GenomeVariant(GenomeVariant&&);
    GenomeVariant(GenomeVariant const&);
    ~GenomeVariant();
    GenomeVariant& operator = (GenomeVariant const&);
    GenomeVariant& operator = (GenomeVariant&&);

    bool operator == (GenomeVariant const& o) const;

    FixedString GetTypeName() const;
    UserReturn LuaGetValue(lua_State* L) const;
    void LuaSetValue(lua_State* L, int index);
    void LuaSetValue(FixedString const& type, lua_State* L, int index);
    void LuaSetValue(GenomeVarTypeDesc* type, lua_State* L, int index);

    template <class T>
    void SetValue(T const& value)
    {
        if (sizeof(T) <= sizeof(Value)) {
            new (reinterpret_cast<T*>(&Value)) T(value);
        } else {
            Value = GameAlloc<T>(value);
        }
    }

    template <class T>
    void SetValue(lua_State* L, int index)
    {
        if (sizeof(T) <= sizeof(Value)) {
            new (reinterpret_cast<T*>(&Value)) T(get<T>(L, index));
        } else {
            auto val = get<T>(L, index);
            // Allocate after get<T> to avoid leak if get() fails
            Value = GameAlloc<T>(val);
        }
    }

    template <class T>
    T const& GetValue() const
    {
        if (sizeof(T) <= sizeof(Value)) {
            return *reinterpret_cast<T const*>(&Value);
        } else {
            return *reinterpret_cast<T const*>(Value);
        }
    }

private:
    void Release();
};

template <class T>
struct [[bg3::hidden]] TGenomeSet
{
public:
    TGenomeSet()
    {
        IsOwned = true;
        Values = GameAlloc<TrackedCompactSet<T>>();
    }

    TGenomeSet(TGenomeSet&& o)
    {
        IsOwned = o.IsOwned;
        Values = o.Values;
        o.IsOwned = false;
        o.Values = nullptr;
    }

    TGenomeSet(TGenomeSet const& o)
    {
        Release();
        IsOwned = true;
        if (o.Values) {
            Values = GameAlloc<TrackedCompactSet<T>>(*o.Values);
        }
    }

    TGenomeSet& operator = (TGenomeSet&& o)
    {
        Release();
        IsOwned = o.IsOwned;
        Values = o.Values;
        o.IsOwned = false;
        o.Values = nullptr;
        return *this;
    }

    TGenomeSet& operator = (TGenomeSet const& o)
    {
        Release();
        IsOwned = true;
        if (o.Values) {
            Values = GameAlloc<TrackedCompactSet<T>>(*o.Values);
        }
        return *this;
    }

    virtual ~TGenomeSet()
    {
        Release();
    }
    
    virtual void AssignReference(TGenomeSet& o)
    {
        Release();
        IsOwned = false;
        Values = o.Values;
    }
    
    virtual void Clone(TGenomeSet& o)
    {
        *this = o;
    }
    
    virtual void Add(gn::GenomeVariant const& o)
    {
        Values->push_back(o.GetValue<T>());
    }
    
    virtual std::size_t Size() const
    {
        return Values->size();
    }

    virtual void RemoveAt(uint32_t index, bool ordered)
    {
        if (index < Values->size()) {
            if (ordered) {
                Values->ordered_remove_at(index);
            } else {
                Values->remove_at(index);
            }
        }
    }

    virtual void Clear()
    {
        Values->clear();
    }

    virtual void SetVariantAt(gn::GenomeVariant const& o, uint32_t index, bool grow)
    {
        if (grow) {
            if (Values->size() <= index) {
                Values->resize(index + 1);
            }
            (*Values)[index] = o.GetValue<T>();
        } else if (index < Values->size()) {
            (*Values)[index] = o.GetValue<T>();
        }
    }

    virtual void GetVariantAt(gn::GenomeVariant& o, uint32_t index) const
    {
        if (index < Values->size()) {
            o.SetValue<T>((*Values)[index]);
        }
    }

    virtual uint32_t GetTypeHash() const
    {
        ERR("TGenomeSet::GetTypeHash() not supported yet!");
        return 0;
    }

    virtual void Iterate(void* unknown) const
    {
        ERR("TGenomeSet::Iterate() not supported yet!");
    }

    bool IsOwned{ false };
    TrackedCompactSet<T>* Values{ nullptr };

private:
    void Release()
    {
        if (IsOwned && Values) {
            GameDelete(Values);
        }
    }
};

END_NS()
