#pragma once

#include <Lua/Shared/Proxies/LuaStructIDs.h>

BEGIN_NS(lua)

enum class PropertyNotification
{
    None = 0,
    Deprecated = 1 << 0,
    Renamed = 1 << 1,
    TemporaryName = 1 << 2,
};

class GenericPropertyMap;

struct RawPropertyAccessors
{
    using Getter = PropertyOperationResult (lua_State* L, LifetimeHandle const& lifetime, void const* object, RawPropertyAccessors const& prop);
    using Setter = PropertyOperationResult (lua_State* L, void* object, int index, RawPropertyAccessors const& prop);
    using Serializer = PropertyOperationResult (lua_State* L, void const* object, RawPropertyAccessors const& prop);

    FixedString Name;
    std::size_t Offset;
    uint64_t Flag{ 0 };
    Getter* Get{ nullptr };
    Setter* Set{ nullptr };
    Serializer* Serialize{ nullptr };
    mutable PropertyNotification PendingNotifications{ PropertyNotification::None };
    GenericPropertyMap* PropertyMap{ nullptr };
    FixedString NewName;
    bool Iterable{ true };
};

class GenericPropertyMap : Noncopyable<GenericPropertyMap>
{
public:
    using TFallbackGetter = PropertyOperationResult (lua_State* L, LifetimeHandle lifetime, void const* object, FixedString const& prop);
    using TFallbackSetter = PropertyOperationResult (lua_State* L, void* object, FixedString const& prop, int index);
    using TFallbackNext = int (lua_State* L, LifetimeHandle lifetime, void const* object, FixedString const& prop);
    using TConstructor = void (void*);
    using TDestructor = void (void*);
    using TSerializer = void (lua_State* L, void const*);
    using TUnserializer = void (lua_State* L, int index, void*);
    using TAssigner = void (void* object, void* rhs);

    struct RawPropertyValidators
    {
        using Validator = bool (void const* object, std::size_t offset, uint64_t flag);

        FixedString Name;
        Validator* Validate;
        std::size_t Offset;
        uint64_t Flag;
    };

    enum class ValidationState
    {
        Unknown,
        Valid,
        Invalid
    };

    void Init();
    void Finish();
    bool HasProperty(FixedString const& prop) const;
    PropertyOperationResult GetRawProperty(lua_State* L, LifetimeHandle lifetime, void const* object, FixedStringId const& prop) const;
    PropertyOperationResult GetRawProperty(lua_State* L, LifetimeHandle lifetime, void const* object, RawPropertyAccessors const& prop) const;
    PropertyOperationResult SetRawProperty(lua_State* L, void* object, FixedStringId const& prop, int index) const;
    void AddRawProperty(char const* prop, typename RawPropertyAccessors::Getter* getter, typename RawPropertyAccessors::Setter* setter,
        typename RawPropertyAccessors::Serializer* serialize, std::size_t offset, uint64_t flag, 
        PropertyNotification notification, char const* newName = nullptr, bool iterable = true);
    void AddRawValidator(char const* prop, typename RawPropertyValidators::Validator* validate, std::size_t offset, uint64_t flag);
    void AddRawProperty(char const* prop, typename RawPropertyAccessors::Getter* getter,
        typename RawPropertyAccessors::Setter* setter, typename RawPropertyValidators::Validator* validate, 
        typename RawPropertyAccessors::Serializer* serialize, std::size_t offset, uint64_t flag, 
        PropertyNotification notification, char const* newName = nullptr, bool iterable = true);
    bool IsA(int typeRegistryIndex) const;
    bool ValidatePropertyMap(void const* object);
    bool ValidateObject(void const* object);

    FixedString Name;
    HashMap<FixedString, RawPropertyAccessors> Properties;
    HashMap<FixedString, uint32_t> IterableProperties;
    Array<RawPropertyValidators> Validators;
    Array<FixedString> Parents;
    Array<int> ParentRegistryIndices;
    TFallbackGetter* FallbackGetter{ nullptr };
    TFallbackSetter* FallbackSetter{ nullptr };
    TFallbackNext* FallbackNext{ nullptr };
    TConstructor* Construct{ nullptr };
    TDestructor* Destroy{ nullptr };
    TAssigner* Assign{ nullptr };
    TSerializer* Serialize{ nullptr };
    TUnserializer* Unserialize{ nullptr };
    GenericPropertyMap const* Parent{ nullptr };
    std::size_t Size{ 0 };
    bool IsInitializing{ false };
    bool Initialized{ false };
    bool InheritanceUpdated{ false };
    ValidationState Validated{ ValidationState::Unknown };
    StructTypeId RegistryIndex{ -1 };
    std::optional<ExtComponentType> ComponentType;
    TypeInformation* TypeInfo{ nullptr };
};

inline PropertyOperationResult GenericSetNonWriteableProperty(lua_State* L,  void* obj, int index, RawPropertyAccessors const&)
{
    return PropertyOperationResult::UnsupportedType;
}

inline PropertyOperationResult GenericSetReadOnlyProperty(lua_State* L, void* obj, int index, RawPropertyAccessors const&)
{
    return PropertyOperationResult::ReadOnly;
}

inline PropertyOperationResult GenericNullSerializeProperty(lua_State* L, void const* object, RawPropertyAccessors const&)
{
    return PropertyOperationResult::UnsupportedType;
}

inline bool GenericValidateNoopProperty(void const* obj, std::size_t offset, uint64_t)
{
    return true;
}

template <class T>
void DefaultConstruct(void* ptr)
{
    new (reinterpret_cast<T*>(ptr)) T();
}

template <class T>
void DefaultDestroy(void* ptr)
{
    (reinterpret_cast<T*>(ptr))->~T();
}

template <class T>
void DefaultSerialize(lua_State* L, void const* ptr)
{
    Serialize(L, reinterpret_cast<T const*>(ptr));
}

template <class T>
void DefaultUnserialize(lua_State* L, int index, void* ptr)
{
    auto result = Unserialize(L, index, reinterpret_cast<T*>(ptr));
    if (result != PropertyOperationResult::Success) {
        luaL_error(L, "Failed to unserialize value");
    }
}

template <class T>
void DefaultAssign(void* object, void* rhs)
{
    *reinterpret_cast<T*>(object) = *reinterpret_cast<T*>(rhs);
}


struct StructRegistry
{
    Array<GenericPropertyMap*> StructsById;

    void Register(GenericPropertyMap* ei, StructTypeId id);

    inline GenericPropertyMap* Get(StructTypeId id) const
    {
        assert(id < (int)StructsById.size());
        return StructsById[id];
    }
};

extern StructRegistry gStructRegistry;


template <class T>
inline GenericPropertyMap& GetStaticPropertyMap()
{
    return *gStructRegistry.Get(StructID<std::remove_cv_t<T>>::ID);
}

END_NS()
