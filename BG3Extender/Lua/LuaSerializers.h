#pragma once

#include <Lua/LuaBinding.h>
#include <GameDefinitions/Stats/Common.h>
#include <GameDefinitions/RootTemplates.h>

namespace bg3se
{
	struct EntityWorldHandle;
	struct CRPGStats_Requirement;
	struct CRPGStats_Treasure_Table;
	struct CRPGStats_Treasure_Category;
	struct CRPGStats_Treasure_SubTable_Description;
	struct StatsFunctorSet;
}

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
				v = checked_get<T>(L, -1);
			}

			return *this;
		}

		template <class T>
		void VisitProperty(char const* key, T& val)
		{
			StackCheck _(L);
			if (IsWriting) {
				*this << val;
				lua_setfield(L, -2, key);
			} else {
				lua_getfield(L, -1, key);
				*this << val;
				lua_pop(L, 1);
			}
		}

		template <class T>
		void VisitOptionalProperty(char const* key, T& val, T const& defaultValue)
		{
			StackCheck _(L);
			if (IsWriting) {
				*this << val;
				lua_setfield(L, -2, key);
			} else {
				lua_getfield(L, -1, key);
				if (lua_isnil(L, -1)) {
					val = defaultValue;
				} else {
					*this << val;
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
		serializer << const_cast<T&>(val);
		return 1;
	}

	template <class T>
	void LuaRead(lua_State* L, T& val)
	{
		StackCheck _(L);
		LuaSerializer serializer(L, false);
		serializer << val;
	}

	inline LuaSerializer& operator << (LuaSerializer& s, bool& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, int8_t& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, uint8_t& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, int16_t& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, uint16_t& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, int32_t& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, uint32_t& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, int64_t& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, uint64_t& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, float& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, double& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, FixedString& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, STDString& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, STDWString& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, Path& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, UUID& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, NetId& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, UserId& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, ComponentHandle& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, EntityHandle& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, glm::ivec2& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, glm::vec2& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, glm::vec3& v) { return s.Visit(v); }
	inline LuaSerializer& operator << (LuaSerializer& s, glm::vec4& v) { return s.Visit(v); }
	LuaSerializer& operator << (LuaSerializer& s, TranslatedString& v);

	template <class T>
	LuaSerializer& operator << (LuaSerializer& s, OverrideableProperty<T>& v)
	{
		s << v.Value;
		if (!s.IsWriting) {
			v.IsOverridden = true;
		}
		return s;
	}

	template <class T, class Allocator, bool StoreSize>
	LuaSerializer& operator << (LuaSerializer& s, ObjectSet<T, Allocator, StoreSize>& v)
	{
		s.BeginObject();
		if (s.IsWriting) {
			int i = 1;
			for (auto& val : v) {
				StackCheck _(s.L);
				push(s.L, i++);
				s << val;
				lua_settable(s.L, -3);
			}
		} else {
			v.Clear();
			for (auto idx : iterate(s.L, -1)) {
				StackCheck _(s.L);
				T temp{};
				s << temp;
				v.Add(temp);
			}
		}
		s.EndObject();
		return s;
	}

	template <class T>
	LuaSerializer& operator << (LuaSerializer& s, Array<T>& v)
	{
		s.BeginObject();
		if (s.IsWriting) {
			int i = 1;
			for (auto& val : v) {
				StackCheck _(s.L);
				push(s.L, i++);
				s << val;
				lua_settable(s.L, -3);
			}
		} else {
			v.Clear();
			for (auto idx : iterate(s.L, -1)) {
				StackCheck _(s.L);
				T temp{};
				s << temp;
				v.Add(temp);
			}
		}
		s.EndObject();
		return s;
	}

	template <class T>
	LuaSerializer& operator << (LuaSerializer& s, MultiHashSet<T>& v)
	{
		s.BeginObject();
		if (s.IsWriting) {
			int i = 1;
			for (auto& val : v) {
				StackCheck _(s.L);
				push(s.L, i++);
				s << val;
				lua_settable(s.L, -3);
			}
		} else {
			v.Clear();
			for (auto idx : iterate(s.L, -1)) {
				StackCheck _(s.L);
				T temp{};
				s << temp;
				v.Add(temp);
			}
		}
		s.EndObject();
		return s;
	}

	template <class TKey, class TValue>
	LuaSerializer& operator << (LuaSerializer& s, MultiHashMap<TKey, TValue>& v)
	{
		s.BeginObject();
		if (s.IsWriting) {
			int i = 1;
			/*for (auto& it : v) {
				StackCheck _(s.L);
				s << it.Key() << it.Value();
				lua_settable(s.L, -3);
			}*/
			for (auto it = v.begin(); it != v.end(); ++it) {
				StackCheck _(s.L);
				s << it.Key();
				s << it.Value();
				lua_settable(s.L, -3);
			}
		} else {
			v.Clear();
			for (auto idx : iterate(s.L, -1)) {
				StackCheck _(s.L);
				TKey key{};
				TValue value{};
				s << key << value;
				v.Set(std::move(key), std::move(value));
			}
		}
		s.EndObject();
		return s;
	}

	template <class TKey, class TValue>
	LuaSerializer& operator << (LuaSerializer& s, RefMap<TKey, TValue>& v)
	{
		s.BeginObject();
		if (s.IsWriting) {
			int i = 1;
			for (auto& it : v) {
				StackCheck _(s.L);
				s << it.Key << it.Value;
				lua_settable(s.L, -3);
			}
		} else {
			v.Clear();
			for (auto idx : iterate(s.L, -1)) {
				StackCheck _(s.L);
				TKey key{};
				TValue value{};
				s << key << value;
				v.Insert(std::move(key), std::move(value));
			}
		}
		s.EndObject();
		return s;
	}

	template <class TKey, class TValue>
	LuaSerializer& operator << (LuaSerializer& s, Map<TKey, TValue>& v)
	{
		s.BeginObject();
		if (s.IsWriting) {
			int i = 1;
			for (auto& it : v) {
				StackCheck _(s.L);
				s << it.Key() << it.Value();
				lua_settable(s.L, -3);
			}
		} else {
			v.Clear();
			for (auto idx : iterate(s.L, -1)) {
				StackCheck _(s.L);
				TKey key{};
				TValue value{};
				s << key << value;
				v.Insert(std::move(key), std::move(value));
			}
		}
		s.EndObject();
		return s;
	}

	template <class T>
	LuaSerializer& operator << (LuaSerializer& s, Vector<T>& v)
	{
		s.BeginObject();
		if (s.IsWriting) {
			int i = 1;
			for (auto& val : v) {
				StackCheck _(s.L);
				push(s.L, i++);
				s << val;
				lua_settable(s.L, -3);
			}
		} else {
			v.clear();
			for (auto idx : iterate(s.L, -1)) {
				StackCheck _(s.L);
				T temp{};
				s << temp;
				v.push_back(temp);
			}
		}
		s.EndObject();
		return s;
	}

	template <class T>
	LuaSerializer& operator << (LuaSerializer& s, std::optional<T>& v)
	{
		if (s.IsWriting) {
			if (v) {
				s << *v;
			} else {
				push(s.L, nullptr);
			}
		} else {
			if (!v) {
				v = T();
			}

			s << *v;
		}

		return s;
	}


	template <class T>
	typename std::enable_if_t<std::is_pointer_v<T>, std::enable_if_t<IsAllocatable<std::remove_pointer_t<T>>::value, LuaSerializer&>> operator << (LuaSerializer& s, T& v)
	{
		if (s.IsWriting) {
			if (v == nullptr) {
				lua_pushnil(s.L);
			} else {
				s << *v;
			}
		} else {
			if (v == nullptr) {
				v = GameAlloc<std::remove_pointer_t<T>>();
			}

			s << *v;
		}

		return s;
	}


	template <class T>
	typename std::enable_if_t<std::is_enum_v<T>, LuaSerializer&> operator << (LuaSerializer& s, T& v)
	{
		if constexpr (std::is_base_of_v<EnumInfoBase<T>, EnumInfo<T>>) {
			if (s.IsWriting) {
				push(s.L, v);
			} else {
				v = checked_get<T>(s.L, -1);
			}
		} else if constexpr (std::is_base_of_v<BitmaskInfoBase<T>, EnumInfo<T>>) {
			if (s.IsWriting) {
				push_flags(s.L, v);
			} else {
				v = checked_get_flags<T>(s.L, -1);
			}
		} else {
			static_assert(false, "Cannot serialize an enumeration that has no EnumInfo!");
		}

		return s;
	}

	void LuaSerializeStatsEnum(LuaSerializer& s, char const* key, FixedString const& enumName, int& v);

	LuaSerializer& operator << (LuaSerializer& s, EntityWorldHandle& v);

/*	LuaSerializer& operator << (LuaSerializer& s, CEquipmentSet& v);
	LuaSerializer& operator << (LuaSerializer& s, CEquipmentGroup& v);
	LuaSerializer& operator << (LuaSerializer& s, CSkillSet& v);*/
	LuaSerializer& operator << (LuaSerializer& s, CRPGStats_Requirement& v);
	LuaSerializer& operator << (LuaSerializer& s, CRPGStats_Object::StatsFunctorInfo& v);
	LuaSerializer& operator << (LuaSerializer& s, CRPGStats_Treasure_Table& v);
	LuaSerializer& operator << (LuaSerializer& s, CRPGStats_Treasure_SubTable_Description& v);
	LuaSerializer& operator << (LuaSerializer& s, CRPGStats_Treasure_Category& v);
	LuaSerializer& operator << (LuaSerializer& s, StatsFunctorSet& v);
/*	LuaSerializer& operator << (LuaSerializer& s, CItemGroup& v);
	LuaSerializer& operator << (LuaSerializer& s, CLevelGroup& v);
	LuaSerializer& operator << (LuaSerializer& s, CRootGroup& v);
	LuaSerializer& operator << (LuaSerializer& s, CNameGroupLink& v);
	LuaSerializer& operator << (LuaSerializer& s, CNameGroup& v);
	LuaSerializer& operator << (LuaSerializer& s, CNameGroupName& v);*/
	LuaSerializer& operator << (LuaSerializer& s, SurfaceTemplate::VisualData& v);
	LuaSerializer& operator << (LuaSerializer& s, SurfaceTemplate::StatusData& v);
}
