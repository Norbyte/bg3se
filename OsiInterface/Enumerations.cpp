#include "stdafx.h"

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>
#include <ExtensionHelpers.h>

namespace dse
{

#define BEGIN_BITMASK_NS(NS, T, type) \
	std::vector<FixedString> BitmaskInfoBase<NS::T>::Labels; \
	Map<FixedString, NS::T> BitmaskInfoBase<NS::T>::Values;
#define BEGIN_ENUM_NS(NS, T, type) \
	std::vector<FixedString> EnumInfoBase<NS::T>::Labels; \
	Map<FixedString, NS::T> EnumInfoBase<NS::T>::Values;
#define BEGIN_BITMASK(T, type) \
	std::vector<FixedString> BitmaskInfoBase<T>::Labels; \
	Map<FixedString, T> BitmaskInfoBase<T>::Values;
#define BEGIN_ENUM(T, type) \
	std::vector<FixedString> EnumInfoBase<T>::Labels; \
	Map<FixedString, T> EnumInfoBase<T>::Values;
#define E(label)
#define EV(label, value)
#define END_ENUM_NS()
#define END_ENUM()
#include <GameDefinitions/Enumerations.inl>
#undef BEGIN_BITMASK_NS
#undef BEGIN_ENUM_NS
#undef BEGIN_BITMASK
#undef BEGIN_ENUM
#undef E
#undef EV
#undef END_ENUM_NS
#undef END_ENUM

	void InitializeEnumerations()
	{

#define BEGIN_BITMASK_NS(NS, T, type) { \
	using e = NS::T; \
	using ei = EnumInfo<e>; \
	ei::Init(61);
#define BEGIN_ENUM_NS(NS, T, type) { \
	using e = NS::T; \
	using ei = EnumInfo<e>; \
	ei::Init(61);
#define BEGIN_BITMASK(T, type) { \
	using e = T; \
	using ei = EnumInfo<e>; \
	ei::Init(61);
#define BEGIN_ENUM(T, type) { \
	using e = T; \
	using ei = EnumInfo<e>; \
	ei::Init(61);
#define E(label) ei::Add(e::label, #label);
#define EV(label, value) ei::Add(e::label, #label);
#define END_ENUM_NS() }
#define END_ENUM() }
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
}
