#include <stdafx.h>

#include <GameDefinitions/Base/Base.h>
#include <Extender/Shared/ExtensionHelpers.h>

namespace bg3se
{

#define BEGIN_BITMASK_NS(NS, T, luaName, type) \
	BitmaskInfoStore<uint64_t>* BitmaskInfoBase<NS::T>::Store;
#define BEGIN_ENUM_NS(NS, T, luaName, type) \
	EnumInfoStore<uint64_t>* EnumInfoBase<NS::T>::Store;
#define BEGIN_BITMASK(T, type) \
	BitmaskInfoStore<uint64_t>* BitmaskInfoBase<T>::Store;
#define BEGIN_ENUM(T, type) \
	EnumInfoStore<uint64_t>* EnumInfoBase<T>::Store;
#define E(label)
#define EV(label, value)
#define ER(label, value)
#define END_ENUM_NS()
#define END_ENUM()
#include <GameDefinitions/Enumerations.inl>
#include <GameDefinitions/ExternalEnumerations.inl>
#undef BEGIN_BITMASK_NS
#undef BEGIN_ENUM_NS
#undef BEGIN_BITMASK
#undef BEGIN_ENUM
#undef E
#undef EV
#undef ER
#undef END_ENUM_NS
#undef END_ENUM

	void InitializeEnumerations()
	{

#define BEGIN_BITMASK_NS(NS, T, luaName, type) { \
	using e = NS::T; \
	using ei = EnumInfo<e>; \
	ei::Init(61, #NS "::" #T, #luaName); \
	BitmaskRegistry::Get().Register(ei::Store);
#define BEGIN_ENUM_NS(NS, T, luaName, type) { \
	using e = NS::T; \
	using ei = EnumInfo<e>; \
	ei::Init(61, #NS "::" #T, #luaName); \
	EnumRegistry::Get().Register(ei::Store);
#define BEGIN_BITMASK(T, type) { \
	using e = T; \
	using ei = EnumInfo<e>; \
	ei::Init(61, #T, #T); \
	BitmaskRegistry::Get().Register(ei::Store);
#define BEGIN_ENUM(T, type) { \
	using e = T; \
	using ei = EnumInfo<e>; \
	ei::Init(61, #T, #T); \
	EnumRegistry::Get().Register(ei::Store);
#define E(label) ei::Add(e::label, #label);
#define EV(label, value) ei::Add(e::label, #label);
#define ER(label, value) ei::Add(value, #label);
#define END_ENUM_NS() }
#define END_ENUM() }
#include <GameDefinitions/Enumerations.inl>
#include <GameDefinitions/ExternalEnumerations.inl>
#undef BEGIN_BITMASK_NS
#undef BEGIN_ENUM_NS
#undef BEGIN_BITMASK
#undef BEGIN_ENUM
#undef E
#undef EV
#undef ER
#undef END_ENUM_NS
#undef END_ENUM

	}
}
