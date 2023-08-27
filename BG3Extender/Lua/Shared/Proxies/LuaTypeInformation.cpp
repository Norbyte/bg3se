#include <stdafx.h>

#include <Lua/Shared/Proxies/PropertyMapDependencies.h>

BEGIN_SE()

// Type information registration

template <class UnderlyingType>
void AddBitmaskTypeInfo(TypeInformation& ty, Map<FixedString, UnderlyingType> const& values)
{
	for (auto const& label : values) {
		ty.Members.insert(std::make_pair(label.Key, GetTypeInfoRef<bool>()));
	}
}

template <class T>
void AddBitmaskTypeInfo(TypeInformation& ty)
{
	AddBitmaskTypeInfo(ty, EnumInfo<T>::Store->Values);
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

#define BEGIN_CLS(clsName) ([]() { \
	using TClass = clsName;\
	auto& ty = TypeInformationRepository::GetInstance().RegisterType(FixedString(#clsName)); \
	ty.Kind = LuaTypeId::Object; \
	ty.NativeName = FixedString(typeid(TClass).name());


#define BEGIN_CLS_TN(clsName, typeName) ([]() { \
	using TClass = clsName;\
	auto& ty = TypeInformationRepository::GetInstance().RegisterType(FixedString(#typeName)); \
	ty.Kind = LuaTypeId::Object; \
	ty.NativeName = FixedString(typeid(TClass).name());

#define END_CLS() GetStaticTypeInfo(Overload<TClass>{}).Type = &ty; })();
#define INHERIT(base) ty.ParentType = GetTypeInfoRef<base>();
#define P(prop) ty.Members.insert(std::make_pair(FixedString(#prop), GetTypeInfoRef<decltype(TClass::prop)>()));
#define P_RO(prop) ty.Members.insert(std::make_pair(FixedString(#prop), GetTypeInfoRef<decltype(TClass::prop)>()));
#define P_REF(prop) ty.Members.insert(std::make_pair(FixedString(#prop), GetTypeInfoRef<decltype(TClass::prop)>()));
#define P_REF_TY(prop, propty) ty.Members.insert(std::make_pair(FixedString(#prop), GetTypeInfoRef<propty>()));
#define P_BITMASK(prop) AddBitmaskTypeInfo<decltype(TClass::prop)>(ty);
#define P_BITMASK_GETTER_SETTER(prop, getter, setter) AddBitmaskTypeInfo<decltype(TClass::prop)>(ty);
#define PN(name, prop) ty.Members.insert(std::make_pair(FixedString(#name), GetTypeInfoRef<decltype(TClass::prop)>()));
#define PN_RO(name, prop) ty.Members.insert(std::make_pair(FixedString(#name), GetTypeInfoRef<decltype(TClass::prop)>()));
#define PN_REF(name, prop) ty.Members.insert(std::make_pair(FixedString(#name), GetTypeInfoRef<decltype(TClass::prop)>()));
#define P_GETTER(prop, fun) ty.Members.insert(std::make_pair(FixedString(#prop), GetTypeInfoRef<decltype(GetFunctionReturnType(&TClass::fun))>()));
#define P_GETTER_SETTER(prop, getter, setter) ty.Members.insert(std::make_pair(FixedString(#prop), GetTypeInfoRef<decltype(GetFunctionReturnType(&TClass::getter))>()));
#define P_FUN(prop, fun) AddFunctionSignature(ty, #prop, &TClass::fun);
#define P_FALLBACK(getter, setter) ty.HasWildcardProperties = true;

#include <GameDefinitions/PropertyMaps/AllPropertyMaps.inl>

#undef BEGIN_CLS
#undef BEGIN_CLS_TN
#undef END_CLS
#undef INHERIT
#undef P
#undef P_RO
#undef P_REF
#undef P_REF_TY
#undef P_BITMASK
#undef P_BITMASK_GETTER_SETTER
#undef PN
#undef PN_RO
#undef PN_REF
#undef P_GETTER
#undef P_GETTER_SETTER
#undef P_FUN
#undef P_FALLBACK


#define BEGIN_BITMASK_NS(NS, T, luaName, type) ([]() { \
	using TEnum = NS::T; \
	auto& ty = TypeInformationRepository::GetInstance().RegisterType(FixedString(#luaName)); \
	ty.Kind = LuaTypeId::Enumeration;

#define BEGIN_ENUM_NS(NS, T, luaName, type) ([]() { \
	using TEnum = NS::T; \
	auto& ty = TypeInformationRepository::GetInstance().RegisterType(FixedString(#luaName)); \
	ty.Kind = LuaTypeId::Enumeration;

#define BEGIN_BITMASK(T, type) ([]() { \
	using TEnum = T; \
	auto& ty = TypeInformationRepository::GetInstance().RegisterType(FixedString(#T)); \
	ty.Kind = LuaTypeId::Enumeration;

#define BEGIN_ENUM(T, type) ([]() { \
	using TEnum = T; \
	auto& ty = TypeInformationRepository::GetInstance().RegisterType(FixedString(#T)); \
	ty.Kind = LuaTypeId::Enumeration;

#define E(label) ty.EnumValues.insert(std::make_pair(FixedString(#label), (uint64_t)TEnum::label));
#define EV(label, value) ty.EnumValues.insert(std::make_pair(FixedString(#label), (uint64_t)TEnum::label));
#define END_ENUM_NS() GetStaticTypeInfo(Overload<TEnum>{}).Type = &ty; })();
#define END_ENUM() GetStaticTypeInfo(Overload<TEnum>{}).Type = &ty; })(); 

#include <GameDefinitions/Enumerations.inl>

#undef BEGIN_BITMASK_NS
#undef BEGIN_ENUM_NS
#undef BEGIN_BITMASK
#undef BEGIN_ENUM
#undef E
#undef EV
#undef END_ENUM_NS
#undef END_ENUM
}

END_SE()
