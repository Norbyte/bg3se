#include <stdafx.h>

#include <Lua/Shared/Proxies/PropertyMapDependencies.h>

BEGIN_SE()

// Type information registration

template <class UnderlyingType>
void AddBitfieldTypeInfo(TypeInformation& ty, LegacyMap<FixedString, UnderlyingType> const& values)
{
    for (auto const& label : values) {
        ty.Members.insert(std::make_pair(label.Key, GetTypeInfoRef<bool>()));
    }
}

template <class T>
void AddBitfieldTypeInfo(TypeInformation& ty)
{
    AddBitfieldTypeInfo(ty, BitfieldInfo<T>::GetStore().Values);
}

template <class Fun>
void AddFunctionSignature(TypeInformation& ty, char const* method, Fun f)
{
    TypeInformation sig;
    ConstructFunctionSignature(sig, f);
    ty.Methods.insert(std::make_pair(FixedString(method), sig));
}

__declspec(noinline) TypeInformation& RegisterClassDefn(char const* typeName, char const* nativeName, lua::GenericPropertyMap& pm, char const* component, char const* system)
{
    auto& ty = TypeInformationRepository::GetInstance().RegisterType(FixedString(typeName));
    ty.Kind = LuaTypeId::Object;
    ty.NativeName = FixedString(nativeName);
    ty.PropertyMap = &pm;
    if (component) {
        ty.ComponentName = FixedString(component);
    }
    if (system) {
        ty.SystemName = FixedString(system);
    }
    ty.PropertyMap->TypeInfo = &ty;
    se_assert(FixedString(typeName) == ty.PropertyMap->Name);
    return ty;
}

template <class T>
inline constexpr char const* GetStaticComponentName()
{
    if constexpr (IsComponentType<T>) {
        return T::ComponentName;
    } else {
        return nullptr;
    }
}

template <class T>
inline constexpr char const* GetStaticSystemName()
{
    if constexpr (std::is_base_of_v<BaseSystem, T> && !std::is_same_v<BaseSystem, T>) {
        return T::SystemName;
    } else {
        return nullptr;
    }
}

void RegisterObjectProxyTypeInformation()
{
#define GENERATING_TYPE_INFO
#define ADD_TYPE(prop, type) ty.Members.insert(std::make_pair(FixedString(prop), GetTypeInfoRef<type>()));

#define BEGIN_CLS_TN(clsName, typeName, id) ([]() { \
    using TClass = clsName;\
    auto& ty = RegisterClassDefn(#typeName, typeid(TClass).name(), lua::GetStaticPropertyMap<TClass>(), \
        GetStaticComponentName<TClass>(), GetStaticSystemName<TClass>());

#define BEGIN_CLS(clsName, id) BEGIN_CLS_TN(clsName, clsName, id)

#define END_CLS() GetStaticTypeInfo(Overload<TClass>{}).Type = &ty; })();
#define INHERIT(base) ty.ParentType = GetTypeInfoRef<base>();
#define P(prop) ty.AddMember(#prop, std::move(GetTypeInfoRef<decltype(TClass::prop)>()));
#define P_NOTIFY(prop, notification) ty.AddMember(#prop, std::move(GetTypeInfoRef<decltype(TClass::prop)>()));
#define P_RENAMED(prop, oldName) ty.AddMember(#prop, std::move(GetTypeInfoRef<decltype(TClass::prop)>())); \
    ty.AddMember(#oldName, std::move(GetTypeInfoRef<decltype(TClass::prop)>()));
#define P_RO(prop) ty.AddMember(#prop, std::move(GetTypeInfoRef<decltype(TClass::prop)>()));
#define P_BITMASK(prop) AddBitfieldTypeInfo<decltype(TClass::prop)>(ty);
#define P_BITMASK_GETTER_SETTER(prop, getter, setter) AddBitfieldTypeInfo<decltype(TClass::prop)>(ty);
#define PN(name, prop) ty.AddMember(#name, std::move(GetTypeInfoRef<decltype(TClass::prop)>()));
#define PN_RO(name, prop) ty.AddMember(#name, std::move(GetTypeInfoRef<decltype(TClass::prop)>()));
#define P_GETTER(prop, fun) ty.AddMember(#prop, std::move(GetTypeInfoRef<decltype(GetFunctionReturnType(&TClass::fun))>()));
#define P_FREE_GETTER(prop, fun) ty.AddMember(#prop, std::move(GetTypeInfoRef<decltype(GetFunctionReturnType(&fun))>()));
#define P_GETTER_SETTER(prop, getter, setter) ty.AddMember(#prop, std::move(GetTypeInfoRef<decltype(GetFunctionReturnType(&TClass::getter))>()));
#define P_FUN(prop, fun) AddFunctionSignature(ty, #prop, &fun);
#define P_FALLBACK(getter, setter, next) ty.HasWildcardProperties = true;

#include <GameDefinitions/Generated/PropertyMaps.inl>

#undef BEGIN_CLS
#undef BEGIN_CLS_TN
#undef END_CLS
#undef INHERIT
#undef P
#undef P_NOTIFY
#undef P_RENAMED
#undef P_RO
#undef P_BITMASK
#undef P_BITMASK_GETTER_SETTER
#undef PN
#undef PN_RO
#undef P_GETTER
#undef P_FREE_GETTER
#undef P_GETTER_SETTER
#undef P_FUN
#undef P_FALLBACK

    RegisterEnumerationTypeInformation();
}

END_SE()
