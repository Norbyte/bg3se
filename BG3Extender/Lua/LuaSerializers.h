#pragma once

#include <Lua/LuaBinding.h>
#include <GameDefinitions/Stats/Common.h>
#include <GameDefinitions/Stats/Stats.h>
#include <GameDefinitions/RootTemplates.h>


namespace bg3se::lua
{
    class LuaSerializer : Noncopyable<LuaSerializer>
    {
    public:
        lua_State* L;
        bool IsWriting;

        inline LuaSerializer(lua_State* l, bool isWriting)
            : L(l), IsWriting(isWriting)
        {}

        template <class T>
        LuaSerializer& Visit(T& v)
        {
            if (IsWriting) {
                StackCheck _(L, 1);
                push(L, v);
            } else {
                StackCheck _(L);
                v = get<T>(L, -1);
            }

            return *this;
        }

        template <class T>
        LuaSerializer& Visit(T const& v)
        {
            return this->Visit(const_cast<T&>(v));
        }

        template <class T>
        void VisitProperty(char const* key, T& val)
        {
            StackCheck _(L);
            if (IsWriting) {
                serialize(*this, val);
                lua_setfield(L, -2, key);
            } else {
                lua_getfield(L, -1, key);
                serialize(*this, val);
                lua_pop(L, 1);
            }
        }

        template <class T>
        void VisitOptionalProperty(char const* key, T& val, T const& defaultValue)
        {
            StackCheck _(L);
            if (IsWriting) {
                serialize(*this, val);
                lua_setfield(L, -2, key);
            } else {
                lua_getfield(L, -1, key);
                if (lua_isnil(L, -1)) {
                    val = defaultValue;
                } else {
                    serialize(*this, val);
                }
                lua_pop(L, 1);
            }
        }

        inline void BeginObject()
        {
            if (IsWriting) {
                lua_newtable(L);
            } else {
                luaL_checktype(L, -1, LUA_TTABLE);
            }
        }

        inline void EndObject() {}
    };

    template <class T>
    int LuaWrite(lua_State* L, T const& val)
    {
        StackCheck _(L, 1);
        LuaSerializer serializer(L, true);
        serialize(serializer, const_cast<T&>(val));
        return 1;
    }

    template <class T>
    void LuaRead(lua_State* L, T& val)
    {
        StackCheck _(L);
        LuaSerializer serializer(L, false);
        serialize(serializer, val);
    }

    inline LuaSerializer& serialize(LuaSerializer& s, bool& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, int8_t& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, uint8_t& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, int16_t& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, uint16_t& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, int32_t& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, uint32_t& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, int64_t& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, uint64_t& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, float& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, double& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, FixedString& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, STDString& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, STDWString& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, StringView& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, Noesis::Symbol& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, ImguiHandle& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, Path& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, Guid& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, NetId& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, UserId& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, ComponentHandle& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, EntityHandle& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, glm::ivec2& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, glm::ivec4& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, glm::vec2& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, glm::vec3& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, glm::vec4& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, glm::aligned_highp_vec4& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, glm::quat& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, glm::mat3& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, glm::mat3x4& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, glm::mat4x3& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, glm::mat4& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, Version& v) { return s.Visit(v); }

    inline LuaSerializer& serialize(LuaSerializer& s, Ref& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, RegistryEntry& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, PersistentRef& v) { return s.Visit(v); }
    inline LuaSerializer& serialize(LuaSerializer& s, PersistentRegistryEntry& v) { return s.Visit(v); }

    template <class T>
    inline LuaSerializer& serialize(LuaSerializer& s, LuaDelegate<T>& v) { return s.Visit(v); }
    

    LuaSerializer& serialize(LuaSerializer& s, ecs::EntityRef& v);
    LuaSerializer& serialize(LuaSerializer& s, TranslatedString& v);
    LuaSerializer& serialize(LuaSerializer& s, TypeInformationRef& v);
    LuaSerializer& serialize(LuaSerializer& s, bg3se::stats::Requirement& v);
    LuaSerializer& serialize(LuaSerializer& s, bg3se::stats::TreasureTable& v);
    LuaSerializer& serialize(LuaSerializer& s, bg3se::stats::TreasureSubTable& v);
    LuaSerializer& serialize(LuaSerializer& s, bg3se::stats::TreasureCategory& v);

    LuaSerializer& serialize(LuaSerializer& s, bg3se::stats::TreasureTable*& v);
    LuaSerializer& serialize(LuaSerializer& s, bg3se::stats::TreasureSubTable*& v);
    LuaSerializer& serialize(LuaSerializer& s, bg3se::stats::TreasureCategory*& v);

    template <class T>
    LuaSerializer& serialize(LuaSerializer& s, OverrideableProperty<T>& v)
    {
        serialize(s, v.Value);
        if (!s.IsWriting) {
            v.IsOverridden = true;
        }
        return s;
    }

    template <class T>
    LuaSerializer& serialize(LuaSerializer& s, OverrideableProperty<T> const& v)
    {
        serialize(s, v.Value);
        if (!s.IsWriting) {
            v.IsOverridden = true;
        }
        return s;
    }

    template <class T, class Allocator, bool StoreSize>
    LuaSerializer& serialize(LuaSerializer& s, ObjectSet<T, Allocator, StoreSize>& v)
    {
        s.BeginObject();
        if (s.IsWriting) {
            int i = 1;
            for (auto& val : v) {
                StackCheck _(s.L);
                push(s.L, i++);
                serialize(s, val);
                lua_rawset(s.L, -3);
            }
        } else {
            v.clear();
            for (auto idx : iterate(s.L, -1)) {
                StackCheck _(s.L);
                T temp{};
                serialize(s, temp);
                v.Add(temp);
            }
        }
        s.EndObject();
        return s;
    }

    template <class T, class Allocator, bool StoreSize>
    LuaSerializer& serialize(LuaSerializer& s, TrackedCompactSet<T>& v)
    {
        s.BeginObject();
        if (s.IsWriting) {
            int i = 1;
            for (auto& val : v) {
                StackCheck _(s.L);
                push(s.L, i++);
                serialize(s, val);
                lua_rawset(s.L, -3);
            }
        } else {
            v.clear();
            for (auto idx : iterate(s.L, -1)) {
                StackCheck _(s.L);
                T temp{};
                serialize(s, temp);
                v.Add(temp);
            }
        }
        s.EndObject();
        return s;
    }

    template <class T, class Allocator, bool StoreSize>
    LuaSerializer& serialize(LuaSerializer& s, MiniCompactSet<T>& v)
    {
        s.BeginObject();
        if (s.IsWriting) {
            int i = 1;
            for (auto& val : v) {
                StackCheck _(s.L);
                push(s.L, i++);
                serialize(s, val);
                lua_rawset(s.L, -3);
            }
        } else {
            v.clear();
            for (auto idx : iterate(s.L, -1)) {
                StackCheck _(s.L);
                T temp{};
                serialize(s, temp);
                v.Add(temp);
            }
        }
        s.EndObject();
        return s;
    }

    template <class T>
    LuaSerializer& serialize(LuaSerializer& s, Array<T>& v)
    {
        s.BeginObject();
        if (s.IsWriting) {
            int i = 1;
            for (auto& val : v) {
                StackCheck _(s.L);
                push(s.L, i++);
                serialize(s, val);
                lua_rawset(s.L, -3);
            }
        } else {
            v.clear();
            for (auto idx : iterate(s.L, -1)) {
                StackCheck _(s.L);
                T temp{};
                serialize(s, temp);
                v.Add(temp);
            }
        }
        s.EndObject();
        return s;
    }

    template <class T>
    LuaSerializer& serialize(LuaSerializer& s, Array<T> const& v)
    {
        se_assert(!s.IsWriting);

        s.BeginObject();
        int i = 1;
        for (auto const& val : v) {
            StackCheck _(s.L);
            push(s.L, i++);
            serialize(s, const_cast<T&>(val));
            lua_rawset(s.L, -3);
        }
        s.EndObject();
        return s;
    }

    template <class T, unsigned N>
    LuaSerializer& serialize(LuaSerializer& s, Noesis::Vector<T, N>& v)
    {
        s.BeginObject();
        if (s.IsWriting) {
            int i = 1;
            for (auto& val : v) {
                StackCheck _(s.L);
                push(s.L, i++);
                serialize(s, val);
                lua_rawset(s.L, -3);
            }
        } else {
            v.Clear();
            for (auto idx : iterate(s.L, -1)) {
                StackCheck _(s.L);
                T temp{};
                serialize(s, temp);
                v.PushBack(temp);
            }
        }
        s.EndObject();
        return s;
    }

    template <class T>
    LuaSerializer& serialize(LuaSerializer& s, HashSet<T>& v)
    {
        s.BeginObject();
        if (s.IsWriting) {
            int i = 1;
            for (auto& val : v) {
                StackCheck _(s.L);
                push(s.L, i++);
                serialize(s, val);
                lua_rawset(s.L, -3);
            }
        } else {
            v.Clear();
            for (auto idx : iterate(s.L, -1)) {
                StackCheck _(s.L);
                T temp{};
                serialize(s, temp);
                v.Add(temp);
            }
        }
        s.EndObject();
        return s;
    }

    template <class TKey, class TValue>
    LuaSerializer& serialize(LuaSerializer& s, HashMap<TKey, TValue>& v)
    {
        s.BeginObject();
        if (s.IsWriting) {
            int i = 1;
            for (auto it = v.begin(); it != v.end(); ++it) {
                StackCheck _(s.L);
                serialize(s, const_cast<TKey&>(it.Key()));
                serialize(s, it.Value());
                lua_rawset(s.L, -3);
            }
        } else {
            v.clear();
            for (auto idx : iterate(s.L, -1)) {
                StackCheck _(s.L);
                TKey key{};
                TValue value{};
                lua_pushvalue(s.L, -2);
                serialize(s, key);
                lua_pop(s.L, 1);
                serialize(s, value);
                v.set(std::move(key), std::move(value));
            }
        }
        s.EndObject();
        return s;
    }

    template <class TKey, class TValue>
    LuaSerializer& serialize(LuaSerializer& s, LegacyRefMap<TKey, TValue>& v)
    {
        s.BeginObject();
        if (s.IsWriting) {
            int i = 1;
            for (auto& it : v) {
                StackCheck _(s.L);
                serialize(s, it.Key);
                serialize(s, it.Value);
                lua_rawset(s.L, -3);
            }
        } else {
            v.clear();
            for (auto idx : iterate(s.L, -1)) {
                StackCheck _(s.L);
                TKey key{};
                TValue value{};
                lua_pushvalue(s.L, -2);
                serialize(s, key);
                lua_pop(s.L, 1);
                serialize(s, value);
                v.insert(std::move(key), std::move(value));
            }
        }
        s.EndObject();
        return s;
    }

    template <class TKey, class TValue>
    LuaSerializer& serialize(LuaSerializer& s, LegacyMap<TKey, TValue>& v)
    {
        s.BeginObject();
        if (s.IsWriting) {
            int i = 1;
            for (auto& it : v) {
                StackCheck _(s.L);
                serialize(s, it.Key);
                serialize(s, it.Value);
                lua_rawset(s.L, -3);
            }
        } else {
            v.clear();
            for (auto idx : iterate(s.L, -1)) {
                StackCheck _(s.L);
                TKey key{};
                TValue value{};
                lua_pushvalue(s.L, -2);
                serialize(s, key);
                lua_pop(s.L, 1);
                serialize(s, value);
                v.insert(std::move(key), std::move(value));
            }
        }
        s.EndObject();
        return s;
    }

    template <class T>
    LuaSerializer& serialize(LuaSerializer& s, Vector<T>& v)
    {
        s.BeginObject();
        if (s.IsWriting) {
            int i = 1;
            for (auto& val : v) {
                StackCheck _(s.L);
                push(s.L, i++);
                serialize(s, val);
                lua_rawset(s.L, -3);
            }
        } else {
            v.clear();
            for (auto idx : iterate(s.L, -1)) {
                StackCheck _(s.L);
                T temp{};
                serialize(s, temp);
                v.push_back(temp);
            }
        }
        s.EndObject();
        return s;
    }

    template <class T>
    LuaSerializer& serialize(LuaSerializer& s, std::optional<T>& v)
    {
        if (s.IsWriting) {
            if (v) {
                serialize(s, *v);
            } else {
                push(s.L, nullptr);
            }
        } else {
            if (!v) {
                v = T();
            }

            serialize(s, *v);
        }

        return s;
    }

    
    template <class T>
    typename std::enable_if_t<std::is_pointer_v<T>, LuaSerializer&> serialize(LuaSerializer& s, T& v)
    {
        if (s.IsWriting) {
            if (v == nullptr) {
                lua_pushnil(s.L);
            } else {
                serialize(s, *v);
            }
        } else {
            if constexpr (decltype(IsAllocatable<std::remove_pointer_t<T>>(nullptr))::value) {
                if (v == nullptr) {
                    v = GameAlloc<std::remove_pointer_t<T>>();
                }
            }

            if (v != nullptr) {
                serialize(s, *v);
            }
        }

        return s;
    }


    template <class T>
    typename std::enable_if_t<std::is_enum_v<T>, LuaSerializer&> serialize(LuaSerializer& s, T& v)
    {
        if (s.IsWriting) {
            push(s.L, v);
        } else {
            v = get<T>(s.L, -1);
        }

        return s;
    }

    template <class T>
    std::enable_if_t<!IsByVal<T>, LuaSerializer&> serialize(LuaSerializer& s, T* v)
    {
        MakeObjectRef(s.L, v);
        return s;
    }

    void LuaSerializeStatsEnum(LuaSerializer& s, char const* key, FixedString const& enumName, int& v);
}
