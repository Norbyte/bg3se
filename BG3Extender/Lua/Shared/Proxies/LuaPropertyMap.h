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
struct RawPropertyAccessors;
struct RawPropertyAccessorsHotData;

inline PropertyOperationResult GenericHotDataPlaceholderGetter(lua_State* L, LifetimeHandle lifetime, void const* object, RawPropertyAccessorsHotData const& prop)
{
    return PropertyOperationResult::NoSuchProperty;
}

inline PropertyOperationResult GenericHotDataPlaceholderSetter(lua_State* L, void* object, int index, RawPropertyAccessors const& prop)
{
    return PropertyOperationResult::NoSuchProperty;
}

struct RawPropertyAccessors
{
    using Getter = PropertyOperationResult (lua_State* L, LifetimeHandle lifetime, void const* object, RawPropertyAccessorsHotData const& prop);
    using Setter = PropertyOperationResult (lua_State* L, void* object, int index, RawPropertyAccessorsHotData const& prop);
    using Serializer = PropertyOperationResult (lua_State* L, void const* object, RawPropertyAccessors const& prop);

    FixedString Name;
    std::size_t Offset{ 0 };
    uint64_t Flag{ 0 };
    Getter* Get{ nullptr };
    Setter* Set{ nullptr };
    Serializer* Serialize{ nullptr };
    mutable PropertyNotification PendingNotifications{ PropertyNotification::None };
    GenericPropertyMap* PropertyMap{ nullptr };
    FixedString NewName;
    bool Iterable{ true };

    inline uint64_t FlagValue() const
    {
        return (Flag & 0x3ff) << (Flag >> 10);
    }
};

void ProcessPropertyNotifications(RawPropertyAccessors const& prop, bool isWriting);

struct RawPropertyAccessorsHotData
{
    // Position of PendingNotifications flag in get_
    static constexpr unsigned NotificationFlagOffset = 48;
    // Position of field offset value in get_
    static constexpr unsigned FieldPositionOffset = 49;
    // Position of flag value in set_
    static constexpr unsigned FlagValueOffset = 48;

    inline RawPropertyAccessorsHotData()
        : get_((uint64_t)&GenericHotDataPlaceholderGetter),
        set_((uint64_t)&GenericHotDataPlaceholderSetter),
        cold_(nullptr)
    {}
    
    inline RawPropertyAccessorsHotData(RawPropertyAccessorsHotData const& props)
        : get_(props.get_.load()),
        set_(props.set_),
        cold_(props.cold_)
    {}
    
    inline RawPropertyAccessorsHotData(RawPropertyAccessors const& props)
        : get_((uint64_t)props.Get
            | (((props.PendingNotifications != PropertyNotification::None) ? 1ull : 0ull) << NotificationFlagOffset)
            | ((uint64_t)props.Offset << FieldPositionOffset)),
        set_((uint64_t)props.Set
            | ((uint64_t)props.Flag << FlagValueOffset)),
        cold_(&props)
    {
        se_assert(props.Offset <= 0x7fff);
        se_assert(props.Flag <= 0xffff);
    }

    inline RawPropertyAccessorsHotData& operator =(RawPropertyAccessorsHotData const& props)
    {
        get_ = props.get_.load();
        set_ = props.set_;
        cold_ = props.cold_;
        return *this;
    }

    inline RawPropertyAccessors::Getter* Getter() const
    {
        return reinterpret_cast<RawPropertyAccessors::Getter*>(get_ & 0x0000ffffffffffffull);
    }

    inline RawPropertyAccessors::Setter* Setter() const
    {
        return reinterpret_cast<RawPropertyAccessors::Setter*>(set_ & 0x0000ffffffffffffull);
    }

    inline RawPropertyAccessors const* GetCold() const
    {
        return cold_;
    }

    inline bool HasNotifications() const
    {
        return (get_ >> NotificationFlagOffset) & 1;
    }

    inline uint64_t Offset() const
    {
        return (get_ >> FieldPositionOffset);
    }

    inline uint64_t Flag() const
    {
        auto flag = (set_ >> FlagValueOffset);
        return (flag & 0x3ff) << (flag >> 10);
    }

    inline void MarkNotificationsProcessed() const
    {
        get_ &= ~(1ull << NotificationFlagOffset);
    }

    inline PropertyOperationResult Get(lua_State* L, LifetimeHandle lifetime, void const* object) const
    {
        if (HasNotifications()) [[unlikely]] {
            MarkNotificationsProcessed();
            ProcessPropertyNotifications(*cold_, false);
        }

        auto getter = Getter();
        auto offset = Offset();
        auto data = static_cast<uint8_t const*>(object) + offset;
        return getter(L, lifetime, data, *this);
    }

    inline PropertyOperationResult Set(lua_State* L, int index, void* object) const
    {
        if (HasNotifications()) [[unlikely]] {
            MarkNotificationsProcessed();
            ProcessPropertyNotifications(*cold_, false);
        }

        auto setter = Setter();
        auto offset = Offset();
        auto data = static_cast<uint8_t*>(object) + offset;
        return setter(L, data, index, *this);
    }

private:
    // Marked as mutable for notification flag updates
    mutable std::atomic<uint64_t> get_{ 0 };
    uint64_t set_{ 0 };
    RawPropertyAccessors const* cold_{ nullptr };
};

class GenericPropertyMap : Noncopyable<GenericPropertyMap>
{
public:
    using TFallbackGetter = PropertyOperationResult (lua_State* L, LifetimeHandle lifetime, void const* object, FixedStringId const& prop);
    using TFallbackSetter = PropertyOperationResult (lua_State* L, void* object, FixedStringId const& prop, int index);
    using TFallbackNext = int (lua_State* L, LifetimeHandle lifetime, void const* object, FixedStringId const& prop);
    using TConstructor = void (void*);
    using TDestructor = void (void*);
    using TProxyDestructor = void (void**);
    using TSerializer = void (lua_State* L, void const*);
    using TUnserializer = void (lua_State* L, int index, void*);
    using TAssigner = void (void* object, void* rhs);

    struct RawPropertyValidators
    {
        using Validator = bool (void const* value, uint64_t flag);

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
    void BuildHotPropertyMap();
    bool HasProperty(FixedStringId const& prop) const;
    PropertyOperationResult GetRawProperty(lua_State* L, LifetimeHandle lifetime, void const* object, FixedStringId const& prop) const;
    PropertyOperationResult GetRawProperty(lua_State* L, LifetimeHandle lifetime, void const* object, RawPropertyAccessors const& prop) const;
    PropertyOperationResult SetRawProperty(lua_State* L, void* object, FixedStringId const& prop, int index) const;
    PropertyOperationResult SetRawProperty(lua_State* L, void* object, RawPropertyAccessors const& prop, int index) const;
    void AddRawProperty(char const* prop, typename RawPropertyAccessors::Getter* getter, typename RawPropertyAccessors::Setter* setter,
        typename RawPropertyAccessors::Serializer* serialize, std::size_t offset, uint64_t flag, 
        PropertyNotification notification, char const* newName = nullptr, bool iterable = true);
    void AddRawValidator(char const* prop, typename RawPropertyValidators::Validator* validate, std::size_t offset, uint64_t flag);
    void AddRawProperty(char const* prop, typename RawPropertyAccessors::Getter* getter,
        typename RawPropertyAccessors::Setter* setter, typename RawPropertyValidators::Validator* validate, 
        typename RawPropertyAccessors::Serializer* serialize, std::size_t offset, uint64_t flag, 
        PropertyNotification notification, char const* newName = nullptr, bool iterable = true);
    bool IsA(StructTypeId typeRegistryIndex) const;
    bool ValidatePropertyMap(void const* object);
    bool ValidateObject(void const* object);

    // Hot data
    StaticHashMap<FixedStringUnhashed, RawPropertyAccessorsHotData> PropertiesHot;
    FixedString Name;
    ValidationState Validated{ ValidationState::Unknown };

    // Cold data
    HashMap<FixedStringUnhashed, RawPropertyAccessors> Properties;
    HashMap<FixedStringUnhashed, uint32_t> IterableProperties;
    Array<RawPropertyValidators> Validators;
    Array<FixedString> Parents;
    Array<StructTypeId> ParentRegistryIndices;
    TFallbackGetter* FallbackGetter{ nullptr };
    TFallbackSetter* FallbackSetter{ nullptr };
    TFallbackNext* FallbackNext{ nullptr };
    TConstructor* Construct{ nullptr };
    TDestructor* Destroy{ nullptr };
    TProxyDestructor* ProxyDestroy{ nullptr };
    TAssigner* Assign{ nullptr };
    TSerializer* Serialize{ nullptr };
    TUnserializer* Unserialize{ nullptr };
    GenericPropertyMap const* Parent{ nullptr };
    std::size_t Size{ 0 };
    bool IsInitializing{ false };
    bool Initialized{ false };
    bool InheritanceUpdated{ false };
    StructTypeId RegistryIndex{ -1 };
    std::optional<ExtComponentType> ComponentType;
    TypeInformation* TypeInfo{ nullptr };

    inline bool ValidateIfNecessary(void const* object)
    {
#if defined(_DEBUG)
        // Need to do a full validation because RuntimeCheckLevel can change
        return ValidatePropertyMap(object);
#else
        // RuntimeCheckLevel is always 'Once'
        if (Validated != ValidationState::Valid) [[unlikely]] {
            return ValidatePropertyMap(object);
        } else {
            return true;
        }
#endif
    }
};

class CustomPropertyManager
{
public:
    CustomPropertyManager();

    bool RegisterProperty(lua_State* L, GenericPropertyMap const& pm, FixedStringUnhashed const& name, Ref value);
    bool RegisterProperty(lua_State* L, GenericPropertyMap const& pm, FixedStringUnhashed const& name, Ref getter, Ref setter);

    PropertyOperationResult GetProperty(lua_State* L, GenericPropertyMap const& pm, LifetimeHandle lifetime, void const* object, FixedStringId const& prop) const;
    PropertyOperationResult SetProperty(lua_State* L, GenericPropertyMap const& pm, void* object, FixedStringId const& prop, int index) const;

private:
    struct CustomProperty
    {
        RegistryEntry Value;
        RegistryEntry Getter;
        RegistryEntry Setter;
    };
    
    struct CustomPropertyMap
    {
        HashMap<FixedStringUnhashed, CustomProperty> Properties;
    };

    Array<std::unique_ptr<CustomPropertyMap>> PropertyMaps;

    bool PropertyCheck(GenericPropertyMap const& pm, FixedStringUnhashed const& name);
};

inline PropertyOperationResult GenericSetNonWriteableProperty(lua_State* L,  void* obj, int index, RawPropertyAccessorsHotData const&)
{
    return PropertyOperationResult::UnsupportedType;
}

inline PropertyOperationResult GenericSetReadOnlyProperty(lua_State* L, void* obj, int index, RawPropertyAccessorsHotData const&)
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
    new (static_cast<T*>(ptr)) T();
}

template <class T>
void DefaultDestroy(void* ptr)
{
    (static_cast<T*>(ptr))->~T();
}

template <class T>
void DefaultProxyDestroy(void** ptr)
{
    if (*ptr) {
        (static_cast<T*>(*ptr))->~T();
        GameFree(*ptr);
        *ptr = nullptr;
    }
}

template <class T>
void DefaultSerialize(lua_State* L, void const* ptr)
{
    Serialize(L, static_cast<T const*>(ptr));
}

template <class T>
void DefaultUnserialize(lua_State* L, int index, void* ptr)
{
    auto result = Unserialize(L, index, static_cast<T*>(ptr));
    if (result != PropertyOperationResult::Success) {
        luaL_error(L, "Failed to unserialize value");
    }
}

template <class T>
void DefaultAssign(void* object, void* rhs)
{
    *static_cast<T*>(object) = *static_cast<T*>(rhs);
}


struct StructRegistry
{
    Array<GenericPropertyMap*> StructsById;
    StaticBitSet<> Validated;

    inline bool ValidateIfNecessary(StructTypeId id, void const* object) const
    {
        if (Validated[(int32_t)id]) [[likely]] {
            return true;
        } else {
            return Get(id)->ValidateIfNecessary(object);
        }
    }

    void Initialize(int32_t size);
    void Register(GenericPropertyMap* ei, StructTypeId id);

    inline GenericPropertyMap* Get(StructTypeId id) const
    {
        assert((int)id >= 0 && (int)id < (int)StructsById.size());
        return *(StructsById.data() + (int32_t)id);
    }
};

extern StructRegistry gStructRegistry;


template <class T>
inline GenericPropertyMap& GetStaticPropertyMap()
{
    static_assert(StructID<std::remove_cv_t<T>> >= StructTypeId(0), "Type does not have a Lua structure definition!");
    return *gStructRegistry.Get(StructID<std::remove_cv_t<T>>);
}

END_NS()
