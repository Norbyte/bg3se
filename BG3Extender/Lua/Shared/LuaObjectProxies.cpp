// Shared container for all property lists

#include <stdafx.h>

#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>
#include <Lua/Shared/LuaPropertyMapHelpers.h>
#include <GameDefinitions/Resources.h>
#include <GameDefinitions/Surface.h>
#include <GameDefinitions/Character.h>
#include <GameDefinitions/Item.h>
#include <GameDefinitions/GuidResources.h>
#include <GameDefinitions/Status.h>
#include <GameDefinitions/Stats/Functors.h>
#include <GameDefinitions/Components/Components.h>
#include <GameDefinitions/Components/Boosts.h>
#include <GameDefinitions/Components/Combat.h>
#include <GameDefinitions/Components/Passives.h>
#include <GameDefinitions/Components/Projectile.h>
#include <GameDefinitions/Components/Stats.h>

#include <Lua/Shared/LuaShared.inl>

namespace bg3se::lua
{

int CharacterGetInventoryItems(lua_State* L, esv::Character* self);
int CharacterGetNearbyCharacters(lua_State* L, esv::Character* self);
int CharacterGetSummons(lua_State* L, esv::Character* self);
int CharacterGetSkills(lua_State* L, esv::Character* self);
int CharacterGetSkillInfo(lua_State* L, esv::Character* self);

int ItemGetInventoryItems(lua_State* L, esv::Item* self);
int ItemGetNearbyCharacters(lua_State* L, esv::Item* self);
int ItemGetGeneratedBoosts(lua_State* L, esv::Item* self);

int StatusGetEngineType(lua_State* L, esv::Status* self);



#define BEGIN_BITMASK_NS(NS, T, type) char const* const ArrayProxyHelpers<NS::T>::TypeName = #T;
#define BEGIN_ENUM_NS(NS, T, type) char const* const ArrayProxyHelpers<NS::T>::TypeName = #T;
#define BEGIN_BITMASK(T, type) char const* const ArrayProxyHelpers<T>::TypeName = #T;
#define BEGIN_ENUM(T, type) char const* const ArrayProxyHelpers<T>::TypeName = #T;
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

void CopyRawProperties(GenericPropertyMap const& base, GenericPropertyMap& child, STDString const& baseClsName)
{
	for (auto const& prop : base.Properties) {
		child.AddRawProperty(prop.first, prop.second.Get, prop.second.Set, prop.second.Offset);
	}

	for (auto const& parent : base.Parents) {
		child.Parents.push_back(parent);
	}

	child.Parents.push_back(baseClsName);
}

// Lua property map and object proxy template specialization declarations

#define BEGIN_CLS(name) \
	char const* const ObjectProxyHelpers<name>::TypeName = #name; \
	LuaPropertyMap<name> StaticLuaPropertyMap<name>::PropertyMap;

#define END_CLS()
#define INHERIT(base)
#define P(prop)
#define P_RO(prop)
#define P_REF(prop)
#define P_REF_PTR(prop)
#define PN(prop, name)
#define P_FUN(prop, fun)

#include <GameDefinitions/PropertyMaps/AllPropertyMaps.inl>

#undef BEGIN_CLS
#undef END_CLS
#undef INHERIT
#undef P
#undef P_RO
#undef P_REF
#undef P_REF_PTR
#undef PN
#undef P_FUN


// Property registrations

	void InitObjectProxyPropertyMaps()
	{
		static bool initialized{ false };
		if (initialized) return;

#define BEGIN_CLS(cls) { \
	using PM = StaticLuaPropertyMap<cls>; \
	auto& pm = StaticLuaPropertyMap<cls>::PropertyMap;

#define END_CLS() }

#define INHERIT(base) { \
		auto& basePm = StaticLuaPropertyMap<base>::PropertyMap; \
		CopyProperties(basePm, pm, #base); \
	}

#define P(prop) \
	pm.AddRawProperty(#prop, \
		&(GenericGetOffsetProperty<decltype(PM::ObjectType::prop)>), \
		&(GenericSetOffsetProperty<decltype(PM::ObjectType::prop)>), \
		offsetof(PM::ObjectType, prop) \
	);

#define P_RO(prop) \
	pm.AddRawProperty(#prop, \
		&(GenericGetOffsetProperty<decltype(PM::ObjectType::prop)>), \
		&SetPropertyWriteProtected, \
		offsetof(PM::ObjectType, prop) \
	);

#define P_REF(prop) \
	pm.AddRawProperty(#prop, \
		&(GenericGetOffsetRefProperty<decltype(PM::ObjectType::prop)>), \
		&GenericSetOffsetRefProperty, \
		offsetof(PM::ObjectType, prop) \
	);

#define P_REF_PTR(prop) \
	pm.AddRawProperty(#prop, \
		&(GenericGetOffsetPtrProperty<decltype(PM::ObjectType::prop)>), \
		&GenericSetOffsetRefProperty, \
		offsetof(PM::ObjectType, prop) \
	);

#define PN(name, prop) \
	pm.AddRawProperty(#name, \
		&(GenericGetOffsetProperty<decltype(PM::ObjectType::prop)>), \
		&(GenericSetOffsetProperty<decltype(PM::ObjectType::prop)>), \
		offsetof(PM::ObjectType, prop) \
	);

#define P_FUN(name, fun) \
	pm.AddProperty(#name, \
		[](lua_State* L, LifetimeHolder const& lifetime, PM::ObjectType* obj, std::size_t offset) { \
			lua_pushcfunction(L, [](lua_State* L) -> int { \
				auto self = checked_get_proxy<PM::ObjectType>(L, 1); \
				return fun(L, self); \
			}); \
			return true; \
		}, \
		[](lua_State* L, LifetimeHolder const& lifetime, PM::ObjectType* obj, int index, std::size_t offset) { \
			return false; \
		}, 0 \
	);

#include <GameDefinitions/PropertyMaps/AllPropertyMaps.inl>

#undef BEGIN_CLS
#undef END_CLS
#undef INHERIT
#undef P
#undef P_RO
#undef P_REF
#undef P_REF_PTR
#undef PN
#undef P_FUN

		initialized = true;
	}
}
