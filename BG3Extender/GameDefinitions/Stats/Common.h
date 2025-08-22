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

struct Object : public Noncopyable<Object>
{
    using SetPropertyStringProc = void(Object* self, FixedString const& propertyName, char const* value);

    struct RollCondition
    {
        FixedString Name;
        stats::ConditionId Conditions;
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

    RPGEnumeration* GetAttributeInfo(FixedString const& attributeName, int& attributeIndex) const;
    std::optional<STDString> GetString(FixedString const& attributeName) const;
    std::optional<FixedString> GetFixedString(FixedString const& attributeName) const;
    std::optional<int> GetInt(FixedString const& attributeName) const;
    std::optional<float> GetFloat(FixedString const& attributeName) const;
    std::optional<int64_t> GetInt64(FixedString const& attributeName) const;
    std::optional<Guid> GetGuid(FixedString const& attributeName) const;
    std::optional<TranslatedString> GetTranslatedString(FixedString const& attributeName) const;
    std::optional<Array<FixedString>> GetFlags(FixedString const& attributeName) const;
    std::optional<Array<FunctorGroup> const*> GetFunctors(FixedString const& attributeName) const;
    std::optional<Array<FunctorGroup>*> GetFunctors(FixedString const& attributeName);
    std::optional<Array<RollCondition> const*> GetRollConditions(FixedString const& attributeName) const;
    std::optional<Array<RollCondition>*> GetRollConditions(FixedString const& attributeName);
    bool SetString(FixedString const& attributeName, const char* value);
    bool SetInt(FixedString const& attributeName, int32_t value);
    bool SetFloat(FixedString const& attributeName, std::optional<float> value);
    bool SetInt64(FixedString const& attributeName, int64_t value);
    bool SetGuid(FixedString const& attributeName, std::optional<Guid> value);
    bool SetTranslatedString(FixedString const& attributeName, std::optional<TranslatedString> value);
    bool SetFlags(FixedString const& attributeName, Array<STDString> const& value);
    bool SetFunctors(FixedString const& attributeName, std::optional<Array<FunctorGroup>> const& value);
    bool SetRollConditions(FixedString const& attributeName, std::optional<Array<RollCondition>> const& value);

    bool CopyFrom(Object* source);
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
BY_VAL(stats::ConditionId);
END_SE()
