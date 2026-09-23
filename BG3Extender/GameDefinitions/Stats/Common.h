#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_NS(stats)

struct ActionResourceCost
{
    Array<Guid> Resources;
    double Amount;
    int SubResourceId;
    Guid ResourceGroup;
};

template <class T>
class CNamedElementManager : public Noncopyable<CNamedElementManager<T>>
{
public:
    CNamedElementManager() noexcept
    {}

    CNamedElementManager(CNamedElementManager const& o) noexcept
        : Values(o.Values),
        NameToHandle(o.NameToHandle),
        NextHandle(o.NextHandle)
    {
        for (auto& e : Values) {
            e = e->Clone();
        }
    }

    CNamedElementManager(CNamedElementManager&& o) noexcept
        : Values(std::move(o.Values)),
        NameToHandle(std::move(o.NameToHandle)),
        NextHandle(o.NextHandle)
    {}

    CNamedElementManager& operator = (CNamedElementManager const& o)
    {
        Values = o.Values;
        NameToHandle = o.NameToHandle;
        NextHandle = o.NextHandle;

        for (auto& e : Values) {
            e = e->Clone();
        }

        return *this;
    }

    CNamedElementManager& operator = (CNamedElementManager&& o) noexcept
    {
        Values = std::move(o.Values);
        NameToHandle = std::move(o.NameToHandle);
        NextHandle = o.NextHandle;

        return *this;
    }

    virtual ~CNamedElementManager()
    {
        Destroy();
    }

    virtual void Destroy()
    {
        for (auto ele : Values) {
            GameDelete(ele);
        }

        Values.clear();
        NameToHandle.clear();
        NextHandle = 0;
    }

    virtual int Insert(T* value)
    {
        auto const& name = value->GetElementName();
        auto handle = GetHandleByName(name);
        if (handle == -1) {
            handle = NextHandle++;
            Values.push_back(value);
            NameToHandle.set(name, handle);
        } else {
            auto cur = Values[handle];
            if (cur) {
                GameDelete(cur);
            }
            Values[handle] = value;
        }

        return handle;
    }

    virtual T* GetByHandle(int handle) const
    {
        if (handle >= 0 && handle < NextHandle) {
            return Values[handle];
        } else {
            return nullptr;
        }
    }

    virtual int GetHandleByName(FixedString const& name) const
    {
        return NameToHandle.get_or_default(name, -1);
    }

    virtual T* GetByName(FixedString const& name) const
    {
        auto handle = NameToHandle.try_get(name);
        if (handle) {
            return Values[*handle];
        } else {
            return nullptr;
        }
    }

    virtual uint64_t GetAmountOfEntries() const
    {
        return Values.size();
    }

    virtual T const* GetEntryConst(uint64_t index) const
    {
        if (index < Values.size()) {
            return Values[(uint32_t)index];
        } else {
            return nullptr;
        }
    }

    virtual T* GetEntry(uint64_t index)
    {
        if (index < Values.size()) {
            return Values[(uint32_t)index];
        } else {
            return nullptr;
        }
    }

    virtual void UpdateHandles()
    {
        for (uint32_t i = 0; i < Values.size(); i++) {
            NameToHandle.set(Values[i]->GetElementName(), i);
        }
    }

    bool Remove(T* f)
    {
        auto it = Values.find(f);
        if (it == Values.end()) {
            return false;
        }

        auto idx = it - Values.begin();
        Values.erase(it);

        for (auto& h : NameToHandle.values()) {
            if (h >= idx) {
                h--;
            }
        }

        NextHandle--;
        return true;
    }

    [[bg3::hidden]] Array<T*> Values;
    [[bg3::hidden]] HashMap<FixedString, int32_t> NameToHandle;
    [[bg3::hidden]] int32_t NextHandle{ 0 };
};

struct Requirement
{
    RequirementType RequirementId;
    int IntParam;
    Guid TagParam;
    bool Not;
};

struct FunctorGroup
{
    FixedString TextKey;
    Functors* Functors;

    Array<Functor*> GetFunctors() const;
};

struct StatModifierCache;

struct Object : public Noncopyable<Object>
{
    struct RollCondition
    {
        FixedString Name;
        ConditionId Conditions;
    };

    void* VMT{ nullptr };
    Vector<int32_t> IndexedProperties;
    FixedString Name;
    HashMap<FixedString, Array<FunctorGroup>> Functors;
    HashMap<FixedString, Array<RollCondition>> RollConditions;
    FixedString AIFlags;
    Array<Requirement> Requirements;
    TrackedCompactSet<FixedString> ComboProperties;
    TrackedCompactSet<FixedString> ComboCategories;
    int32_t Using{ -1 };
    uint32_t ModifierListIndex{ 0 };
    uint32_t Level{ 0 };

    inline FixedString const& GetElementName() const
    {
        return Name;
    }

    std::optional<FixedString> GetFixedString(FixedString const& attributeName) const;
    FixedString const* GetFixedStringRef(StatModifierCache const& attribute) const;
    std::optional<int> GetInt(StatModifierCache const& attribute) const;
    std::optional<float> GetFloat(StatModifierCache const& attribute) const;
    std::optional<int64_t> GetInt64(StatModifierCache const& attribute) const;
    std::optional<Guid> GetGuid(StatModifierCache const& attribute) const;
    std::optional<TranslatedString> GetTranslatedString(StatModifierCache const& attribute) const;
    std::optional<Array<FixedString>> GetFlags(StatModifierCache const& attribute) const;
    std::optional<Array<FunctorGroup> const*> GetFunctors(StatModifierCache const& attribute) const;
    std::optional<Array<FunctorGroup>*> GetFunctors(StatModifierCache const& attribute);
    std::optional<Array<RollCondition> const*> GetRollConditions(StatModifierCache const& attribute) const;
    std::optional<Array<RollCondition>*> GetRollConditions(StatModifierCache const& attribute);
    std::optional<StringView> GetConditions(StatModifierCache const& attribute) const;

    bool SetString(StatModifierCache const& attribute, const char* value);
    bool SetFixedString(StatModifierCache const& attribute, FixedString const& value);
    bool SetInt(StatModifierCache const& attribute, int64_t value);
    bool SetFloat(StatModifierCache const& attribute, std::optional<float> value);
    bool SetInt64(StatModifierCache const& attribute, int64_t value);
    bool SetGuid(StatModifierCache const& attribute, std::optional<Guid> value);
    bool SetTranslatedString(StatModifierCache const& attribute, std::optional<TranslatedString> value);
    bool SetFlags(StatModifierCache const& attribute, Array<FixedString> const& value);
    bool SetFunctors(StatModifierCache const& attribute, std::optional<Array<FunctorGroup>> const& value);
    bool SetRollConditions(StatModifierCache const& attribute, std::optional<Array<RollCondition>> const& value);

    void TryPushValue(lua_State* L, StatModifierCache const& attribute) const;
    bool TrySetValue(lua_State* L, StatModifierCache const& attribute, int index);

    bool CopyFrom(Object* source);

private:
    void SetString(int attributeIndex, FixedString const& value);
    void SetFloat(int attributeIndex, std::optional<float> value);
    void SetInt64Flags(int attributeIndex, int64_t value);
    void SetGuid(int attributeIndex, std::optional<Guid> value);
    void SetTranslatedString(int attributeIndex, std::optional<TranslatedString> value);
};

struct ObjectInstance : public Object
{
    uint32_t InstanceId;
};

END_NS()

BEGIN_NS(lua)

LUA_INFINITE_LIFETIME(stats::Object)

END_NS()

BEGIN_SE()

MARK_BY_VALUE_TYPE(stats::ConditionId);

END_SE()
