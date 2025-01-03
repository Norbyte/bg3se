#pragma once

#include <Lua/Shared/Proxies/LuaLightCppObjectMeta.h>

BEGIN_NS(lua)

struct CppObjectProxyHelpers
{
    static int Next(lua_State* L, GenericPropertyMap const& pm, void* object, LifetimeHandle lifetime, FixedString const& key);
};


class LightObjectProxyMetatable : public LightCppObjectMetatable<LightObjectProxyMetatable>,
    public Indexable, public NewIndexable, public Iterable, public Stringifiable, public EqualityComparable
{
public:
    static constexpr MetatableTag MetaTag = MetatableTag::ObjectRef;
    static constexpr bool HasLifetime = true;

    inline static void Make(lua_State* L, GenericPropertyMap& pm, void* object, LifetimeHandle lifetime)
    {
        lua_push_lightcppobject(L, MetaTag, pm.RegistryIndex, object, lifetime);
    }

    inline static void Make(lua_State* L, GenericPropertyMap& pm, void const* object, LifetimeHandle lifetime)
    {
        // TODO - add RO tag
        lua_push_lightcppobject(L, MetaTag, pm.RegistryIndex, object, lifetime);
    }

    template <class T>
    inline static void Make(lua_State* L, T* object, LifetimeHandle lifetime)
    {
        lua_push_lightcppobject(L, MetaTag, StructID<T>::ID, object, lifetime);
    }

    template <class T>
    inline static void Make(lua_State* L, T const* object, LifetimeHandle lifetime)
    {
        // TODO - add RO tag
        auto const& pm = GetStaticPropertyMap<T>();
        lua_push_lightcppobject(L, MetaTag, pm.RegistryIndex, object, lifetime);
    }

    template <class T>
    inline static T* Copy(lua_State* L, T&& object)
    {
        auto const& pm = GetStaticPropertyMap<T>();
        void* p = reinterpret_cast<T*>(lua_push_newcppobject(L, MetaTag, pm.RegistryIndex, sizeof(T)));
        *p = std::move(object);
        return p;
    }

    static GenericPropertyMap& GetPropertyMap(CppObjectMetadata const& meta);
    static void* TryGetGeneric(lua_State* L, int index, int propertyMapIndex);
    static void* GetGeneric(lua_State* L, int index, int propertyMapIndex);

    template <class T>
    static T* TryGet(lua_State* L, int index)
    {
        auto const& pm = GetStaticPropertyMap<T>();
        return reinterpret_cast<T*>(TryGetGeneric(L, index, pm.PropertyMapTag));
    }

    template <class T>
    static T* Get(lua_State* L, int index)
    {
        auto ptr = GetGeneric(L, index, StructID<T>::ID);
        return reinterpret_cast<T*>(ptr);
    }

    static int Index(lua_State* L, CppObjectMetadata& self);
    static int NewIndex(lua_State* L, CppObjectMetadata& self);
    static int ToString(lua_State* L, CppObjectMetadata& self);
    static bool IsEqual(lua_State* L, CppObjectMetadata& self, CppObjectMetadata& other);
    static int Next(lua_State* L, CppObjectMetadata& self);
    static char const* GetTypeName(lua_State* L, CppObjectMetadata& self);
};


class ObjectProxy
{
public:
    inline static void MakeRef(lua_State* L, GenericPropertyMap& pm, void* object, LifetimeHandle lifetime)
    {
        if (!pm.ValidatePropertyMap(object)) {
            push(L, nullptr);
        } else {
            LightObjectProxyMetatable::Make(L, pm, object, lifetime);
        }
    }
    
    template <class T>
    inline static void MakeRef(lua_State* L, T* object, LifetimeHandle lifetime)
    {
        if (!GetStaticPropertyMap<T>().ValidatePropertyMap(object)) {
            push(L, nullptr);
        } else {
            LightObjectProxyMetatable::Make(L, object, lifetime);
        }
    }
    
    template <class T>
    inline static T* MakeCopy(lua_State* L, T&& object)
    {
        return LightObjectProxyMetatable::Copy(L, std::forward(object));
    }

    static void* GetRaw(lua_State* L, int index, GenericPropertyMap const& pm);

    template <class T>
    inline static T* Get(lua_State* L, int index)
    {
        return reinterpret_cast<T*>(GetRaw(L, index, GetStaticPropertyMap<T>()));
    }
};

template <class T>
inline T* GetObjectRef(lua_State* L, int index)
{
    return ObjectProxy::Get<T>(L, index);
}

END_NS()
