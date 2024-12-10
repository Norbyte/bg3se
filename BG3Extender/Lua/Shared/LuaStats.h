#pragma once

#include <Lua/LuaHelpers.h>
#include <lua/LuaBinding.h>
#include <GameDefinitions/Stats/Functors.h>

#include <mutex>
#include <unordered_set>
#include <optional>

BEGIN_NS(lua::stats)

using namespace bg3se::stats;

struct ObjectHelpers
{
    static FixedString GetModifierList(Object const* obj);
    static FixedString GetModId(Object const* obj);
    static FixedString GetOriginalModId(Object const* obj);
    static FixedString GetUsing(Object const* obj);

    static void Sync(Object* obj, std::optional<bool> persist);
    static void SetPersistence(Object* obj, bool persist);
    static bool SetRawAttribute(Object* obj, FixedString key, char const* value);
    static bool CopyFrom(Object* obj, FixedString parent);

    static PropertyOperationResult FallbackGet(lua_State* L, lua::LifetimeHandle const& lifetime, Object const* object, bg3se::FixedString const& prop);
    static PropertyOperationResult FallbackSet(lua_State* L, Object* object, bg3se::FixedString const& prop, int index);
    static int FallbackNext(lua_State* L, LifetimeHandle const& lifetime, Object const* object, FixedString const& prop);
};


END_NS()

namespace bg3se::lua
{
    void RegisterSharedMetatables(lua_State* L);
}
