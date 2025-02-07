#pragma once

#include <Lua/Shared/Proxies/LuaPropertyMap.h>
#include <Lua/Shared/Proxies/LuaCppClass.h>

BEGIN_NS(lua)

template <class TSubclass>
class LightCppObjectMetatable
{
public:
    static std::optional<CppObjectMetadata> TryGetMetadata(lua_State* L, int index)
    {
        CppObjectMetadata meta;
        if (lua_try_get_lightcppobject(L, index, TSubclass::MetaTag, meta)) {
            return meta;
        } else {
            return {};
        }
    }

    static int CallProxy(lua_State* L)
    {
        if constexpr (std::is_base_of_v<Callable, TSubclass>) {
            auto self = lua_get_lightcppobject(L, 1, TSubclass::MetaTag);
            return TSubclass::Call(L, self);
        } else {
            return luaL_error(L, "Not callable!");
        }
    }

    static int IndexProxy(lua_State* L)
    {
        if constexpr (std::is_base_of_v<OpaqueIndexable, TSubclass>) {
            StackCheck _(L, 1);
            auto self = lua_get_opaque_lightcppobject(L, 1, TSubclass::MetaTag);
            return TSubclass::Index(L, self);

        } else if constexpr (std::is_base_of_v<Indexable, TSubclass>) {
            StackCheck _(L, 1);
            auto self = lua_get_lightcppobject(L, 1, TSubclass::MetaTag);
            return TSubclass::Index(L, self);

        } else {
            return luaL_error(L, "Not indexable!");
        }
    }

    static int NewIndexProxy(lua_State* L)
    {
        if constexpr (std::is_base_of_v<NewIndexable, TSubclass>) {
            StackCheck _(L, 0);
            auto self = lua_get_lightcppobject(L, 1, TSubclass::MetaTag);
            return TSubclass::NewIndex(L, self);
        } else {
            return luaL_error(L, "Not newindexable!");
        }
    }

    static int LengthProxy(lua_State* L)
    {
        if constexpr (std::is_base_of_v<Lengthable, TSubclass>) {
            StackCheck _(L, 1);
            auto self = lua_get_lightcppobject(L, 1, TSubclass::MetaTag);
            return TSubclass::Length(L, self);
        } else {
            return luaL_error(L, "Not lengthable!");
        }
    }

    static int PairsProxy(lua_State* L)
    {
        if constexpr (std::is_base_of_v<Iterable, TSubclass>) {
            auto self = lua_get_lightcppobject(L, 1, TSubclass::MetaTag);
            return TSubclass::Pairs(L, self);
        } else {
            return luaL_error(L, "Not iterable!");
        }
    }

    static int ToStringProxy(lua_State* L)
    {
        if constexpr (std::is_base_of_v<Stringifiable, TSubclass>) {
            StackCheck _(L, 1);
            auto self = lua_get_unchecked_lightcppobject(L, 1, TSubclass::MetaTag);
            return TSubclass::ToString(L, self);
        } else {
            return luaL_error(L, "Not stringifiable!");
        }
    }

    static int GCProxy(lua_State* L)
    {
        if constexpr (std::is_base_of_v<GarbageCollected, TSubclass>) {
            auto self = lua_get_lightcppobject(L, 1, TSubclass::MetaTag);
            return TSubclass::GC(L, self);
        } else {
            return luaL_error(L, "Not garbage collected!");
        }
    }

    static int EqualProxy(lua_State* L)
    {
        if constexpr (std::is_base_of_v<EqualityComparable, TSubclass>) {
            StackCheck _(L, 1);
            auto self = lua_get_lightcppobject(L, 1, TSubclass::MetaTag);
            CppObjectMetadata other;

            bool equal;
            if (lua_try_get_lightcppobject(L, 2, TSubclass::MetaTag, other)) {
                equal = TSubclass::IsEqual(L, self, other);
            } else {
                equal = false;
            }

            push(L, equal);
            return 1;
        } else {
            return luaL_error(L, "Not comparable!");
        }
    }

    static int LessThanProxy(lua_State* L)
    {
        if constexpr (std::is_base_of_v<LessThanComparable, TSubclass>) {
            StackCheck _(L, 1);
            auto self = lua_get_lightcppobject(L, 1, TSubclass::MetaTag);
            CppObjectMetadata other;

            bool lt;
            if (lua_try_get_lightcppobject(L, 2, TSubclass::MetaTag, other)) {
                lt = TSubclass::IsLessThan(L, self, other);
            } else {
                lt = false;
            }

            push(L, lt);
            return 1;
        } else {
            return luaL_error(L, "Not comparable!");
        }
    }

    // Default __pairs implementation
    static int Pairs(lua_State* L, CppObjectMetadata const& self)
    {
        StackCheck _(L, 3);
        lua_pushcfunction(L, &NextProxy);
        lua_pushvalue(L, 1);
        push(L, nullptr);

        return 3;
    }

    static int NextProxy(lua_State* L)
    {
        if constexpr (std::is_base_of_v<Iterable, TSubclass>) {
            auto self = lua_get_lightcppobject(L, 1, TSubclass::MetaTag);
            return TSubclass::Next(L, self);
        } else {
            return luaL_error(L, "Not iterable!");
        }
    }

    static int NameProxy(lua_State* L)
    {
        StackCheck _(L, 1);
        auto self = lua_get_lightcppobject(L, 1, TSubclass::MetaTag);
        push(L, TSubclass::GetTypeName(L, self));
        return 1;
    }

    static void PopulateMetatable(lua_State* L, CMetatable* mt)
    {
        // Add custom metatable items by overriding this in subclasses
    }

    static void RegisterMetatable(lua_State* L)
    {
        auto mt = lua_alloc_cmetatable(L);

        if constexpr (std::is_base_of_v<Callable, TSubclass>) {
            lua_cmetatable_set(L, mt, (int)MetamethodName::Call, &CallProxy);
        }

        if constexpr (std::is_base_of_v<Indexable, TSubclass>) {
            lua_cmetatable_set(L, mt, (int)MetamethodName::Index, &IndexProxy);
        }

        if constexpr (std::is_base_of_v<NewIndexable, TSubclass>) {
            lua_cmetatable_set(L, mt, (int)MetamethodName::NewIndex, &NewIndexProxy);
        }

        if constexpr (std::is_base_of_v<Lengthable, TSubclass>) {
            lua_cmetatable_set(L, mt, (int)MetamethodName::Len, &LengthProxy);
        }

        if constexpr (std::is_base_of_v<Iterable, TSubclass>) {
            lua_cmetatable_set(L, mt, (int)MetamethodName::Pairs, &PairsProxy);
        }

        if constexpr (std::is_base_of_v<Stringifiable, TSubclass>) {
            lua_cmetatable_set(L, mt, (int)MetamethodName::ToString, &ToStringProxy);
        }

        if constexpr (std::is_base_of_v<GarbageCollected, TSubclass>) {
            lua_cmetatable_set(L, mt, (int)MetamethodName::GC, &GCProxy);
        }

        if constexpr (std::is_base_of_v<EqualityComparable, TSubclass>) {
            lua_cmetatable_set(L, mt, (int)MetamethodName::Eq, &EqualProxy);
        }

        if constexpr (std::is_base_of_v<LessThanComparable, TSubclass>) {
            lua_cmetatable_set(L, mt, (int)MetamethodName::Lt, &LessThanProxy);
        }

        lua_cmetatable_set(L, mt, (int)MetamethodName::Name, &NameProxy);

        TSubclass::PopulateMetatable(L, mt);
        CppMetatableManager::FromLua(L).RegisterMetatable(TSubclass::MetaTag, mt);
    }
};

END_NS()
