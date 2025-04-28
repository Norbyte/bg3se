#pragma once

BEGIN_NS(lua)

class MapProxyImplBase
{
public:
    MapProxyImplBase();
    void Register();
    virtual int GetRegistryIndex() const;
    virtual unsigned GetContainerClass() const = 0;
    virtual TypeInformation const& GetContainerType() const = 0;
    virtual TypeInformation const& GetKeyType() const = 0;
    virtual TypeInformation const& GetValueType() const = 0;
    virtual bool GetValue(lua_State* L, CppObjectMetadata& self, int luaKeyIndex) = 0;
    virtual bool SetValue(lua_State* L, CppObjectMetadata& self, int luaKeyIndex, int luaValueIndex) = 0;
    virtual int Next(lua_State* L, CppObjectMetadata& self, int luaKeyIndex) = 0;
    virtual unsigned Length(CppObjectMetadata& self) = 0;
    virtual bool Unserialize(lua_State* L, CppObjectMetadata& self, int index) = 0;
    virtual void Serialize(lua_State* L, CppObjectMetadata& self) = 0;

private:
    int registryIndex_{ -1 };
};

    
template <class TKey, class TValue>
class MultiHashMapProxyImpl : public MapProxyImplBase
{
public:
    static_assert(!std::is_pointer_v<TKey>, "MultiHashMapProxyImpl template parameter should not be a pointer type!");

    using ContainerType = HashMap<TKey, TValue>;
    static constexpr unsigned ContainerClassId = 1;

    unsigned GetContainerClass() const override
    {
        return ContainerClassId;
    }

    TypeInformation const& GetContainerType() const override
    {
        return GetTypeInfo<ContainerType>();
    }

    TypeInformation const& GetKeyType() const override
    {
        return GetTypeInfo<TKey>();
    }

    TypeInformation const& GetValueType() const override
    {
        return GetTypeInfo<TValue>();
    }

    bool GetValue(lua_State* L, CppObjectMetadata& self, int luaKeyIndex) override
    {
        auto obj = reinterpret_cast<ContainerType*>(self.Ptr);
        auto key = get<TKey>(L, luaKeyIndex);
        auto value = obj->try_get(key);
        if (value) {
            push(L, value, self.Lifetime);
            return true;
        } else {
            return false;
        }
    }

    bool SetValue(lua_State* L, CppObjectMetadata& self, int luaKeyIndex, int luaValueIndex) override
    {
        auto obj = reinterpret_cast<ContainerType*>(self.Ptr);
        if constexpr (std::is_default_constructible_v<TValue>) {
            auto key = get<TKey>(L, luaKeyIndex);
            if (lua_type(L, luaValueIndex) == LUA_TNIL) {
                obj->remove(key);
            } else {
                auto value = get<TValue>(L, luaValueIndex);
                obj->set(key, value);
            }

            return true;
        } else {
            return false;
        }
    }

    unsigned Length(CppObjectMetadata& self) override
    {
        auto obj = reinterpret_cast<ContainerType*>(self.Ptr);
        return obj->keys().size();
    }

    int Next(lua_State* L, CppObjectMetadata& self, int luaKeyIndex) override
    {
        auto obj = reinterpret_cast<ContainerType*>(self.Ptr);
        if (lua_type(L, luaKeyIndex) == LUA_TNIL) {
            if (!obj->empty()) {
                // TODO - jank, but const proxies are not supported yet
                push(L, const_cast<TKey*>(&obj->keys()[0]), self.Lifetime);
                push(L, &obj->values()[0], self.Lifetime);
                return 2;
            }
        } else {
            auto key = get<TKey>(L, luaKeyIndex);
            auto index = obj->find_index(key);
            if (index != -1 && index < (int)obj->keys().size() - 1) {
                // TODO - jank, but const proxies are not supported yet
                push(L, const_cast<TKey*>(&obj->keys()[index + 1]), self.Lifetime);
                push(L, &obj->values()[index + 1], self.Lifetime);
                return 2;
            }
        }

        return 0;
    }

    bool Unserialize(lua_State* L, CppObjectMetadata& self, int index) override
    {
        auto obj = reinterpret_cast<ContainerType*>(self.Ptr);
        if constexpr (std::is_default_constructible_v<TValue>) {
            lua::Unserialize(L, index, obj);
            return true;
        } else {
            return false;
        }
    }

    void Serialize(lua_State* L, CppObjectMetadata& self) override
    {
        auto obj = reinterpret_cast<ContainerType*>(self.Ptr);
        lua::Serialize(L, obj);
    }
};

    
template <class TKey, class TValue, class TInternals, unsigned TContainerClassId>
class RefMapProxyImpl : public MapProxyImplBase
{
public:
    static_assert(!std::is_pointer_v<TKey>, "RefMapProxyImpl template parameter should not be a pointer type!");

    using ContainerType = LegacyMapBase<TInternals>;
    static constexpr unsigned ContainerClassId = TContainerClassId;

    unsigned GetContainerClass() const override
    {
        return ContainerClassId;
    }

    TypeInformation const& GetContainerType() const override
    {
        return GetTypeInfo<ContainerType>();
    }

    TypeInformation const& GetKeyType() const override
    {
        return GetTypeInfo<TKey>();
    }

    TypeInformation const& GetValueType() const override
    {
        return GetTypeInfo<TValue>();
    }

    bool GetValue(lua_State* L, CppObjectMetadata& self, int luaKeyIndex) override
    {
        auto obj = reinterpret_cast<ContainerType*>(self.Ptr);
        auto key = get<TKey>(L, luaKeyIndex);
        auto value = obj->find(key);
        if (value != obj->end()) {
            push(L, &value.Value(), self.Lifetime);
            return true;
        } else {
            return false;
        }
    }

    bool SetValue(lua_State* L, CppObjectMetadata& self, int luaKeyIndex, int luaValueIndex) override
    {
        auto obj = reinterpret_cast<ContainerType*>(self.Ptr);
        if constexpr (std::is_default_constructible_v<TValue>) {
            auto key = get<TKey>(L, luaKeyIndex);
            if (lua_type(L, luaValueIndex) == LUA_TNIL) {
                // FIXME - add support for remove() in refmaps!
                // obj->Remove(key);
                return false;
            } else {
                auto value = get<TValue>(L, luaValueIndex);
                obj->insert(key, value);
            }

            return true;
        } else {
            return false;
        }
    }

    unsigned Length(CppObjectMetadata& self) override
    {
        auto obj = reinterpret_cast<ContainerType*>(self.Ptr);
        return obj->size();
    }

    int Next(lua_State* L, CppObjectMetadata& self, int luaKeyIndex) override
    {
        auto obj = reinterpret_cast<ContainerType*>(self.Ptr);
        if (lua_type(L, luaKeyIndex) == LUA_TNIL) {
            auto it = obj->begin();
            if (it != obj->end()) {
                push(L, &it.Key(), self.Lifetime);
                push(L, &it.Value(), self.Lifetime);
                return 2;
            }
        } else {
            auto key = get<TKey>(L, luaKeyIndex);
            auto it = obj->find(key);
            if (it != obj->end()) {
                it++;
                if (it != obj->end()) {
                    push(L, &it.Key(), self.Lifetime);
                    push(L, &it.Value(), self.Lifetime);
                    return 2;
                }
            }
        }

        return 0;
    }

    bool Unserialize(lua_State* L, CppObjectMetadata& self, int index) override
    {
        auto obj = reinterpret_cast<ContainerType*>(self.Ptr);
        if constexpr (std::is_default_constructible_v<TValue>) {
            lua::Unserialize(L, index, obj);
            return true;
        } else {
            return false;
        }
    }

    void Serialize(lua_State* L, CppObjectMetadata& self) override
    {
        auto obj = reinterpret_cast<ContainerType*>(self.Ptr);
        lua::Serialize(L, obj);
    }
};


class MapProxyMetatable : public LightCppObjectMetatable<MapProxyMetatable>, public Indexable, public NewIndexable,
    public Lengthable, public Iterable, public Stringifiable, public EqualityComparable
{
public:
    static constexpr MetatableTag MetaTag = MetatableTag::Map;
    static constexpr bool HasLifetime = true;

    template <class TImpl>
    static MapProxyImplBase* GetImplementation()
    {
        static MapProxyImplBase* impl = new TImpl();
        return impl;
    }

    inline static void MakeImpl(lua_State* L, void* object, LifetimeHandle lifetime, MapProxyImplBase* impl)
    {
        lua_push_lightcppobject(L, MetatableTag::Map, impl->GetRegistryIndex(), object, lifetime);
    }

    inline static void MakeImpl(lua_State* L, void const* object, LifetimeHandle lifetime, MapProxyImplBase* impl)
    {
        lua_push_lightcppobject(L, MetatableTag::Map, impl->GetRegistryIndex(), object, lifetime);
    }

    template <class TKey, class TValue>
    inline static void Make(lua_State* L, HashMap<TKey, TValue>* object, LifetimeHandle lifetime)
    {
        MakeImpl(L, object, lifetime, GetImplementation<MultiHashMapProxyImpl<TKey, TValue>>());
    }

    template <class TKey, class TValue>
    inline static void Make(lua_State* L, HashMap<TKey, TValue> const* object, LifetimeHandle lifetime)
    {
        MakeImpl(L, object, lifetime, GetImplementation<MultiHashMapProxyImpl<TKey, TValue>>());
    }

    template <class TKey, class TValue>
    inline static void Make(lua_State* L, LegacyRefMap<TKey, TValue>* object, LifetimeHandle lifetime)
    {
        MakeImpl(L, object, lifetime, GetImplementation<RefMapProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>, 2>>());
    }

    template <class TKey, class TValue>
    inline static void Make(lua_State* L, LegacyRefMap<TKey, TValue> const* object, LifetimeHandle lifetime)
    {
        MakeImpl(L, object, lifetime, GetImplementation<RefMapProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>, 2>>());
    }

    template <class TKey, class TValue>
    inline static void Make(lua_State* L, LegacyMap<TKey, TValue>* object, LifetimeHandle lifetime)
    {
        MakeImpl(L, object, lifetime, GetImplementation<RefMapProxyImpl<TKey, TValue, MapInternals<TKey, TValue>, 3>>());
    }

    template <class TKey, class TValue>
    inline static void Make(lua_State* L, LegacyMap<TKey, TValue> const* object, LifetimeHandle lifetime)
    {
        MakeImpl(L, object, lifetime, GetImplementation<RefMapProxyImpl<TKey, TValue, MapInternals<TKey, TValue>, 3>>());
    }

    template <class T>
    inline static typename T::ContainerType* Get(lua_State* L, int index)
    {
        auto ptr = GetRaw(L, index, GetImplementation<T>()->GetRegistryIndex());
        return reinterpret_cast<T::ContainerType*>(ptr);
    }

    inline static MapProxyImplBase* GetImpl(CppObjectMetadata const& meta)
    {
        se_assert(meta.MetatableTag == MetatableTag::Map);
        return GetImpl(meta.PropertyMapTag);
    }

    static int Index(lua_State* L, CppObjectMetadata& self);
    static int NewIndex(lua_State* L, CppObjectMetadata& self);
    static int Length(lua_State* L, CppObjectMetadata& self);
    static int Next(lua_State* L, CppObjectMetadata& self);
    static int ToString(lua_State* L, CppObjectMetadata& self);
    static bool IsEqual(lua_State* L, CppObjectMetadata& self, CppObjectMetadata& other);
    static char const* GetTypeName(lua_State* L, CppObjectMetadata& self);

private:
    static void* GetRaw(lua_State* L, int index, int propertyMapIndex);
    static MapProxyImplBase* GetImpl(int propertyMapIndex);
};

END_NS()
