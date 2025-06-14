#pragma once

struct lua_State;

BEGIN_NS(lua)

class GenericPropertyMap;

template <class T>
class LuaDelegate;

class RegistryEntry;

END_NS();

BEGIN_SE()

template <class T>
constexpr bool IsIntegralAlias = false;

#define MARK_INTEGRAL_ALIAS(ty) template <> constexpr bool IsIntegralAlias<ty> = true;


struct TypeInformation;

struct StaticTypeInformation : Noncopyable<StaticTypeInformation>
{
    using InitializerProc = TypeInformation * ();

    TypeInformation* Type{ nullptr };
    InitializerProc* Initializer{ nullptr };

    StaticTypeInformation(TypeInformation* type, InitializerProc* initializer);
    void DeferredInitialize();
};

class TypeInformationRef
{
public:
    inline TypeInformationRef() {}
    inline TypeInformationRef(StaticTypeInformation& ref) : ref_(&ref) {}
    inline TypeInformationRef(TypeInformationRef const& ref) : ref_(ref.ref_) {}
    
    inline TypeInformationRef& operator = (TypeInformationRef const& ref)
    {
        ref_ = ref.ref_;
        return *this;
    }

    inline operator bool() const
    {
        return ref_ && ref_->Type;
    }

    inline bool operator !() const
    {
        return !ref_ || !ref_->Type;
    }

    inline bool IsBound() const
    {
        return ref_ != nullptr;
    }

    inline StaticTypeInformation* GetStatic() const
    {
        return ref_;
    }

    TypeInformation const& Get() const;

private:
    StaticTypeInformation* ref_{ nullptr };
};

struct TypeInformation
{
    FixedString TypeName;
    FixedString NativeName;
    LuaTypeId Kind{ LuaTypeId::Unknown };
    TypeInformationRef KeyType;
    TypeInformationRef ElementType;
    TypeInformationRef ParentType;
    LegacyMap<FixedString, TypeInformationRef> Members;
    LegacyMap<FixedString, TypeInformation> Methods;
    bool HasWildcardProperties{ false };
    LegacyMap<FixedString, uint64_t> EnumValues;
    Array<TypeInformationRef> ReturnValues;
    Array<TypeInformationRef> Params;
    bool VarargParams{ false };
    bool VarargsReturn{ false };
    bool IsBitfield{ false };
    bool IsBuiltin{ false };
    bool DeferredInitializationDone{ false };
    bool Validated{ false };
    FixedString ModuleRole;
    lua::GenericPropertyMap* PropertyMap{ nullptr };
    FixedString ComponentName;

    void DeferredInitialize();
    void Validate();
};

template <class T>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<T>)
{
    return nullptr;
}

template <class T>
TypeInformation* MakeDeferredOptionalType();

template <class T>
TypeInformation* MakeDeferredArrayType();

template <class T>
TypeInformation* MakeDeferredSetType();

template <class TKey, class TValue>
TypeInformation* MakeDeferredMapType();

template <class... Args>
TypeInformation* MakeDeferredVariantType();

template <class T>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<Array<T>>)
{
    return &MakeDeferredArrayType<T>;
}

template <class T>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<StaticArray<T>>)
{
    return &MakeDeferredArrayType<T>;
}

template <class T>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<LegacyArray<T>>)
{
    return &MakeDeferredArrayType<T>;
}

template <class T>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<Queue<T>>)
{
    return &MakeDeferredArrayType<T>;
}

template <class TWord, unsigned Words>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<BitArray<TWord, Words>>)
{
    return &MakeDeferredArrayType<bool>;
}

template <class T>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<Vector<T>>)
{
    return &MakeDeferredArrayType<T>;
}

template <class T>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<std::span<T>>)
{
    return &MakeDeferredArrayType<T>;
}

template <class T, class Allocator, bool StoreSize>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<ObjectSet<T, Allocator, StoreSize>>)
{
    return &MakeDeferredArrayType<T>;
}

template <class T>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<TrackedCompactSet<T>>)
{
    return &MakeDeferredArrayType<T>;
}

template <class T>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<MiniCompactSet<T>>)
{
    return &MakeDeferredArrayType<T>;
}

template <class T, unsigned N>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<Noesis::Vector<T, N>>)
{
    return &MakeDeferredArrayType<T>;
}

inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<Noesis::BaseCollection>)
{
    return &MakeDeferredArrayType<Noesis::BaseComponent*>;
}

inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<Noesis::BaseObservableCollection>)
{
    return &MakeDeferredArrayType<Noesis::BaseComponent*>;
}

inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<Noesis::UIElementCollection>)
{
    return &MakeDeferredArrayType<Noesis::UIElement*>;
}

template <class T, class Allocator, bool StoreSize>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<Set<T, Allocator, StoreSize>>)
{
    return &MakeDeferredArrayType<T>;
}

template <class T, class Allocator>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<PrimitiveSmallSet<T, Allocator>>)
{
    return &MakeDeferredArrayType<T>;
}

template <class T, int Size>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<std::array<T, Size>>)
{
    return &MakeDeferredArrayType<T>;
}

template <class T>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<HashSet<T>>)
{
    return &MakeDeferredSetType<T>;
}

template <class T>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<VirtualMultiHashSet<T>>)
{
    return &MakeDeferredSetType<T>;
}

template <class TKey, class TValue>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<LegacyMap<TKey, TValue>>)
{
    return &MakeDeferredMapType<TKey, TValue>;
}

template <class TKey, class TValue>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<LegacyRefMap<TKey, TValue>>)
{
    return &MakeDeferredMapType<TKey, TValue>;
}

template <class TKey, class TValue>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<HashMap<TKey, TValue>>)
{
    return &MakeDeferredMapType<TKey, TValue>;
}

template <class TKey, class TValue>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<VirtualHashMap<TKey, TValue>>)
{
    return &MakeDeferredMapType<TKey, TValue>;
}

template <class... Args>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<std::variant<Args...>>)
{
    return &MakeDeferredVariantType<Args...>;
}

/*template <class T, class... Trait>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<lua::Traits<T, Trait...>>)
{
    return &MakeDeferredOptionalType<T>;
}*/

template <class T>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<std::optional<T>>)
{
    return &MakeDeferredOptionalType<T>;
}

template <class T>
StaticTypeInformation& GetStaticTypeInfoInternal(Overload<T>)
{
    static StaticTypeInformation info{ nullptr, MakeDeferredTypeInitializer(Overload<T>{}) };
    return info;
}

template <class T, class Allocator, bool StoreSize>
StaticTypeInformation& GetStaticTypeInfoInternal(Overload<ObjectSet<T, Allocator, StoreSize>>)
{
    return GetStaticTypeInfoInternal(Overload<Set<T, Allocator, StoreSize>>{});
}

template <class T>
StaticTypeInformation& GetStaticTypeInfoInternal(Overload<TrackedCompactSet<T>>)
{
    return GetStaticTypeInfoInternal(Overload<Set<T, GameMemoryAllocator, true>>{});
}

template <class T>
StaticTypeInformation& GetStaticTypeInfoInternal(Overload<MiniCompactSet<T>>)
{
    return GetStaticTypeInfoInternal(Overload<Set<T, GameMemoryAllocator, true>>{});
}

template <class T>
StaticTypeInformation& GetStaticTypeInfo(Overload<T>)
{
    if constexpr (std::is_pointer_v<T>) {
        return GetStaticTypeInfo(Overload<std::remove_pointer_t<T>>{});
    } else if constexpr (std::is_reference_v<T>) {
        return GetStaticTypeInfo(Overload<std::remove_reference_t<T>>{});
    } else if constexpr (std::is_const_v<T> || std::is_volatile_v<T>) {
        return GetStaticTypeInfo(Overload<std::remove_cv_t<T>>{});
    } else if constexpr (IsIntegralAlias<T>) {
        return GetStaticTypeInfo(Overload<std::underlying_type_t<T>>{});
    } else {
        return GetStaticTypeInfoInternal(Overload<T>{});
    }
}

template <class T>
StaticTypeInformation& GetStaticTypeInfo(Overload<lua::LuaDelegate<T>>)
{
    return GetStaticTypeInfo(Overload<lua::RegistryEntry>{});
}

template <class T>
StaticTypeInformation& GetStaticTypeInfo(Overload<OverrideableProperty<T>>)
{
    return GetStaticTypeInfo(Overload<T>{});
}

template <class T>
StaticTypeInformation& GetStaticTypeInfo(Overload<RefReturn<T>>)
{
    return GetStaticTypeInfo(Overload<T>{});
}

template <class T>
StaticTypeInformation& GetStaticTypeInfo(Overload<GlobalRefReturn<T>>)
{
    return GetStaticTypeInfo(Overload<T>{});
}

template <class T>
StaticTypeInformation& GetStaticTypeInfo(Overload<CopyReturn<T>>)
{
    return GetStaticTypeInfo(Overload<T>{});
}

template <class T>
StaticTypeInformation& GetStaticTypeInfo(Overload<ByValReturn<T>>)
{
    return GetStaticTypeInfo(Overload<T>{});
}

template <class T>
StaticTypeInformation& GetStaticTypeInfo(Overload<ProxyParam<T>>)
{
    return GetStaticTypeInfo(Overload<T>{});
}

template <class T>
TypeInformation const& GetTypeInfo()
{
    auto& type = GetStaticTypeInfo(Overload<T>{});
    if (type.Type == nullptr) {
        type.DeferredInitialize();
    }
    se_assert(type.Type != nullptr);
    return *type.Type;
}

template <class T>
TypeInformationRef GetTypeInfoRef()
{
    return TypeInformationRef(GetStaticTypeInfo(Overload<T>{}));
}

class TypeInformationRepository
{
public:
    static TypeInformationRepository& GetInstance();

    TypeInformationRepository();
    void Initialize();
    TypeInformation& RegisterType(FixedString const& typeName);
    void RegisterType(TypeInformation* typeInfo);
    void RegisterInitializer(StaticTypeInformation* typeInfo);
    TypeInformation const* TryGetType(FixedString const& typeName);
    TypeInformation const& GetType(FixedString const& typeName);
    TypeInformation const& GetUndefinedType();
    LegacyMap<FixedString, TypeInformation*> const& GetAllTypes();

    template <class T>
    void DeferredRegister(Overload<Array<T>>)
    {
        RegisterArrayType("Array", GetTypeInfoRef<T>());
    }

private:
    LegacyMap<FixedString, TypeInformation*> types_;
    Array<StaticTypeInformation*> initializers_;
    TypeInformation undefinedType_;
    bool initialized_;

    void RegisterArrayType(char const* arrayClass, TypeInformationRef const& elementType);
    TypeInformation& RegisterType(char const* name, LuaTypeId typeId);

    template <class T>
    TypeInformation& RegisterStaticType(char const* name, LuaTypeId typeId)
    {
        auto& ty = RegisterType(name, typeId);
        GetStaticTypeInfo(Overload<T>{}).Type = &ty;
        return ty;
    }
};

template <class T>
TypeInformation* MakeDeferredOptionalType()
{
    auto& subType = GetStaticTypeInfo(Overload<T>{});
    subType.DeferredInitialize();
    if (subType.Type) {
        auto ty = GameAlloc<TypeInformation>();
        ty->TypeName = FixedString(STDString("Optional<") + subType.Type->TypeName.GetString() + ">");
        ty->Kind = LuaTypeId::Nullable;
        ty->ParentType = subType;
        return ty;
    } else {
        return nullptr;
    }
}

template <class T>
TypeInformation* MakeDeferredArrayType()
{
    auto ty = GameAlloc<TypeInformation>();
    ty->Kind = LuaTypeId::Array;
    ty->ElementType = GetStaticTypeInfo(Overload<T>{});
    return ty;
}

template <class T>
TypeInformation* MakeDeferredSetType()
{
    auto ty = GameAlloc<TypeInformation>();
    ty->Kind = LuaTypeId::Set;
    ty->ElementType = GetStaticTypeInfo(Overload<T>{});
    return ty;
}

template <class TKey, class TValue>
TypeInformation* MakeDeferredMapType()
{
    auto ty = GameAlloc<TypeInformation>();
    ty->Kind = LuaTypeId::Map;
    ty->KeyType = GetStaticTypeInfo(Overload<TKey>{});
    ty->ElementType = GetStaticTypeInfo(Overload<TValue>{});
    return ty;
}

template <class R, class... Args>
R GetFunctionReturnType(R (*)(Args...)) {}

template <class R, class T, class... Args>
R GetFunctionReturnType(R (T::*)(Args...)) {}

template <class R, class T, class... Args>
R GetFunctionReturnType(R (T::*)(Args...) const) {}

template <class T>
inline void AddFunctionReturnType(TypeInformation& ty, Overload<T>)
{
    ty.ReturnValues.push_back(GetTypeInfoRef<T>());
}

template <>
inline void AddFunctionReturnType(TypeInformation& ty, Overload<void>)
{
    // No return value, nothing to do
}

template <>
inline void AddFunctionReturnType(TypeInformation& ty, Overload<UserReturn>)
{
    // User defined number of return types and values
    ty.VarargsReturn = true;
}

template <class T>
inline void AddFunctionParamType(TypeInformation& ty, Overload<T>)
{
    ty.Params.push_back(GetTypeInfoRef<T>());
}

// Pointer to member function without Lua state parameter
template <class R, class T, class... Args>
void ConstructFunctionSignature(TypeInformation& sig, R (T::*)(Args...))
{
    sig.Kind = LuaTypeId::Function;
    AddFunctionReturnType(sig, Overload<R>{});
    (AddFunctionParamType(sig, Overload<Args>{}), ...);
}

// Const pointer to member function without Lua state parameter
template <class R, class T, class... Args>
void ConstructFunctionSignature(TypeInformation& sig, R (T::*)(Args...) const)
{
    sig.Kind = LuaTypeId::Function;
    AddFunctionReturnType(sig, Overload<R>{});
    (AddFunctionParamType(sig, Overload<Args>{}), ...);
}

// Pointer to member function with Lua state parameter
template <class R, class T, class... Args>
void ConstructFunctionSignature(TypeInformation& sig, R (T::*)(lua_State* L, Args...))
{
    sig.Kind = LuaTypeId::Function;
    AddFunctionReturnType(sig, Overload<R>{});
    (AddFunctionParamType(sig, Overload<Args>{}), ...);
}

// Const pointer to member function with Lua state parameter
template <class R, class T, class... Args>
void ConstructFunctionSignature(TypeInformation& sig, R (T::*)(lua_State* L, Args...) const)
{
    sig.Kind = LuaTypeId::Function;
    AddFunctionReturnType(sig, Overload<R>{});
    (AddFunctionParamType(sig, Overload<Args>{}), ...);
}

// Unbound function without Lua state parameter
template <class R, class... Args>
void ConstructFunctionSignature(TypeInformation& sig, R (*)(Args...))
{
    sig.Kind = LuaTypeId::Function;
    AddFunctionReturnType(sig, Overload<R>{});
    (AddFunctionParamType(sig, Overload<Args>{}), ...);
}

// Unbound function with Lua state parameter
template <class R, class... Args>
void ConstructFunctionSignature(TypeInformation& sig, R (*)(lua_State* L, Args...))
{
    sig.Kind = LuaTypeId::Function;
    AddFunctionReturnType(sig, Overload<R>{});
    (AddFunctionParamType(sig, Overload<Args>{}), ...);
}

template <class... Args>
TypeInformation* MakeDeferredVariantType()
{
    auto ty = GameAlloc<TypeInformation>();
    ty->Kind = LuaTypeId::Variant;
    (AddFunctionParamType(*ty, Overload<Args>{}), ...);
    return ty;
}

END_SE()
