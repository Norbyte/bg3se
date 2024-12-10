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
struct CNamedElementManager : public Noncopyable<CNamedElementManager<T>>
{
    void* VMT{ nullptr };
    Array<T *> Primitives;
    HashMap<FixedString, uint32_t> NameHashMap;
    uint32_t NextHandle{ 0 };
    uint32_t NumSomeItems{ 0 };

    void Add(FixedString const& name, T* elem)
    {
        NameHashMap.set(name, Primitives.Size());
        Primitives.Add(elem);
        NextHandle++;
    }

    std::optional<int> FindIndex(char const * str) const
    {
        FixedString fs{ str };
        if (fs) {
            return FindIndex(fs);
        } else {
            return {};
        }
    }

    std::optional<int> FindIndex(FixedString const& str) const
    {
        auto ptr = NameHashMap.try_get(str);
        if (ptr) {
            return (int)*ptr;
        } else {
            return {};
        }
    }

    T * Find(int index) const
    {
        if (index < 0 || index >= (int)Primitives.Size()) {
            return nullptr;
        } else {
            return Primitives[index];
        }
    }

    T * Find(char const * str) const
    {
        FixedString fs(str);
        if (fs) {
            return Find(fs);
        } else {
            return nullptr;
        }
    }

    T * Find(FixedString const& str) const
    {
        auto ptr = NameHashMap.try_get(str);
        if (ptr) {
            return Primitives[*ptr];
        } else {
            return nullptr;
        }
    }
};

struct ConditionId
{
    int32_t Id{ -1 };

    STDString* Get() const;
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
    Array<FixedString> ComboProperties;
    Array<FixedString> ComboCategories;
    int32_t Using{ -1 };
    uint32_t ModifierListIndex{ 0 };
    uint32_t Level{ 0 };

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
    void ToProtobuf(class MsgS2CSyncStat* msg) const;
    void FromProtobuf(MsgS2CSyncStat const& msg);
    void BroadcastSyncMessage(bool syncDuringLoading) const;
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
