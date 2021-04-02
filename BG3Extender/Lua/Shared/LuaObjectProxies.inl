// Shared container for all property lists

#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>
#include <Lua/Shared/LuaPropertyMapHelpers.h>
#include <GameDefinitions/Resources.h>


namespace bg3se::lua
{

// Lua property map and object proxy template specialization declarations

#define BEGIN_CLS(name) \
	char const* const ObjectProxy2<name>::MetatableName = #name; \
	LuaPropertyMap<name> StaticLuaPropertyMap<name>::PropertyMap;

#define END_CLS()
#define INHERIT(base)
#define P(prop)
#define P_RO(prop)
#define P_REF(prop)
#define PN(prop, name)

#include <GameDefinitions/PropertyMaps/AllPropertyMaps.inl>

#undef BEGIN_CLS
#undef END_CLS
#undef INHERIT
#undef P
#undef P_RO
#undef P_REF
#undef PN


// Runtime Lua metatable registrations

	void RegisterObjectProxies(lua_State* L)
	{

#define BEGIN_CLS(name) ObjectProxy2<name>::RegisterMetatable(L);
#define END_CLS()
#define INHERIT(base)
#define P(prop)
#define P_RO(prop)
#define P_REF(prop)
#define PN(prop, name)

#include <GameDefinitions/PropertyMaps/AllPropertyMaps.inl>

#undef BEGIN_CLS
#undef END_CLS
#undef INHERIT
#undef P
#undef P_RO
#undef P_REF
#undef PN
	}


// Property registrations

	void InitObjectProxyPropertyMaps(lua_State* L)
	{

#define BEGIN_CLS(cls) { \
	using PM = StaticLuaPropertyMap<cls>; \
	auto& pm = StaticLuaPropertyMap<cls>::PropertyMap;

#define END_CLS() }

#define INHERIT(base) { \
		auto& basePm = StaticLuaPropertyMap<base>::PropertyMap; \
		CopyProperties(basePm, pm); \
	}

#define P(prop) \
	pm.AddProperty(#prop, \
		[](lua_State* L, PM::ObjectType* obj) { \
			return GenericGetProperty(L, obj->prop); \
		}, \
		[](lua_State* L, PM::ObjectType* obj, int index) { \
			return GenericSetProperty(L, obj->prop, index); \
		} \
	);

#define P_RO(prop) \
	pm.AddProperty(#prop, \
		[](lua_State* L, PM::ObjectType* obj) { \
			return GenericGetProperty(L, obj->prop); \
		}, \
		[](lua_State* L, PM::ObjectType* obj, int index) { \
			return false; \
		} \
	);

#define P_REF(prop) \
	pm.AddProperty(#prop, \
		[](lua_State* L, PM::ObjectType* obj) { \
			ObjectProxy2<decltype(obj->prop)>::New(L, &obj->prop); \
			return true; \
		}, \
		[](lua_State* L, PM::ObjectType* obj, int index) { \
			return false; \
		} \
	);

#define PN(name, prop) \
	pm.AddProperty(#name, \
		[](lua_State* L, PM::ObjectType* obj) { \
			return GenericGetProperty(L, obj->prop); \
		}, \
		[](lua_State* L, PM::ObjectType* obj, int index) { \
			return GenericSetProperty(L, obj->prop, index); \
		} \
	);

#include <GameDefinitions/PropertyMaps/AllPropertyMaps.inl>

#undef BEGIN_CLS
#undef END_CLS
#undef INHERIT
#undef P
#undef P_RO
#undef PN
	}
}
