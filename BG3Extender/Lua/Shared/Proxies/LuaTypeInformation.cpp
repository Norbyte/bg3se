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

void RegisterObjectProxyTypeInformation()
{
#define GENERATING_TYPE_INFO
#define ADD_TYPE(prop, type) ty.Members.insert(std::make_pair(FixedString(prop), GetTypeInfoRef<type>()));

#define BEGIN_CLS_TN(clsName, typeName, id) ([]() { \
    using TClass = clsName;\
    auto& ty = TypeInformationRepository::GetInstance().RegisterType(FixedString(#typeName)); \
    ty.Kind = LuaTypeId::Object; \
    ty.NativeName = FixedString(typeid(TClass).name()); \
    ty.PropertyMap = &lua::GetStaticPropertyMap<TClass>(); \
    if constexpr (std::is_base_of_v<BaseComponent, TClass> && !std::is_same_v<BaseComponent, TClass> && !std::is_same_v<BaseProxyComponent, TClass>) { \
        ty.ComponentName = FixedString(TClass::ComponentName); \
    } \
    ty.PropertyMap->TypeInfo = &ty; \
    se_assert(FixedString(#typeName) == ty.PropertyMap->Name);

#define BEGIN_CLS(clsName, id) BEGIN_CLS_TN(clsName, clsName, id)

#define END_CLS() GetStaticTypeInfo(Overload<TClass>{}).Type = &ty; })();
#define INHERIT(base) ty.ParentType = GetTypeInfoRef<base>();
#define P(prop) ty.Members.insert(std::make_pair(FixedString(#prop), GetTypeInfoRef<decltype(TClass::prop)>()));
#define P_NOTIFY(prop, notification) ty.Members.insert(std::make_pair(FixedString(#prop), GetTypeInfoRef<decltype(TClass::prop)>()));
#define P_RENAMED(prop, oldName) ty.Members.insert(std::make_pair(FixedString(#prop), GetTypeInfoRef<decltype(TClass::prop)>())); \
    ty.Members.insert(std::make_pair(FixedString(#oldName), GetTypeInfoRef<decltype(TClass::prop)>()));
#define P_RO(prop) ty.Members.insert(std::make_pair(FixedString(#prop), GetTypeInfoRef<decltype(TClass::prop)>()));
#define P_BITMASK(prop) AddBitfieldTypeInfo<decltype(TClass::prop)>(ty);
#define P_BITMASK_GETTER_SETTER(prop, getter, setter) AddBitfieldTypeInfo<decltype(TClass::prop)>(ty);
#define PN(name, prop) ty.Members.insert(std::make_pair(FixedString(#name), GetTypeInfoRef<decltype(TClass::prop)>()));
#define PN_RO(name, prop) ty.Members.insert(std::make_pair(FixedString(#name), GetTypeInfoRef<decltype(TClass::prop)>()));
#define P_GETTER(prop, fun) ty.Members.insert(std::make_pair(FixedString(#prop), GetTypeInfoRef<decltype(GetFunctionReturnType(&TClass::fun))>()));
#define P_FREE_GETTER(prop, fun) ty.Members.insert(std::make_pair(FixedString(#prop), GetTypeInfoRef<decltype(GetFunctionReturnType(&fun))>()));
#define P_GETTER_SETTER(prop, getter, setter) ty.Members.insert(std::make_pair(FixedString(#prop), GetTypeInfoRef<decltype(GetFunctionReturnType(&TClass::getter))>()));
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


#define BEGIN_BITMASK_NS(NS, T, luaName, type, id) ([]() { \
    using TEnum = NS::T; \
    auto& ty = TypeInformationRepository::GetInstance().RegisterType(FixedString(#luaName)); \
    ty.Kind = LuaTypeId::Enumeration; \
    ty.IsBitfield = true;

#define BEGIN_ENUM_NS(NS, T, luaName, type, id) ([]() { \
    using TEnum = NS::T; \
    auto& ty = TypeInformationRepository::GetInstance().RegisterType(FixedString(#luaName)); \
    ty.Kind = LuaTypeId::Enumeration;

#define BEGIN_BITMASK(T, type, id) ([]() { \
    using TEnum = T; \
    auto& ty = TypeInformationRepository::GetInstance().RegisterType(FixedString(#T)); \
    ty.Kind = LuaTypeId::Enumeration; \
    ty.IsBitfield = true;

#define BEGIN_ENUM(T, type, id) ([]() { \
    using TEnum = T; \
    auto& ty = TypeInformationRepository::GetInstance().RegisterType(FixedString(#T)); \
    ty.Kind = LuaTypeId::Enumeration;

#define EV(label, value) ty.EnumValues.insert(std::make_pair(FixedString(#label), (uint64_t)TEnum::label));
#define END_ENUM_NS() GetStaticTypeInfo(Overload<TEnum>{}).Type = &ty; })();
#define END_ENUM() GetStaticTypeInfo(Overload<TEnum>{}).Type = &ty; })(); 

#include <GameDefinitions/Generated/Enumerations.inl>
#include <GameDefinitions/Generated/ExternalEnumerations.inl>

#undef BEGIN_BITMASK_NS
#undef BEGIN_ENUM_NS
#undef BEGIN_BITMASK
#undef BEGIN_ENUM
#undef EV
#undef END_ENUM_NS
#undef END_ENUM
}

END_SE()
