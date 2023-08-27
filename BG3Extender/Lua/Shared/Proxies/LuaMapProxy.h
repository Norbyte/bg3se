#pragma once

#include <Lua/LuaHelpers.h>
#include <Lua/LuaSerializers.h>
#include <Lua/Shared/LuaLifetime.h>

namespace bg3se::lua
{
	LifetimeHandle GetCurrentLifetime();

	class MapProxyImplBase
	{
	public:
		inline virtual ~MapProxyImplBase() {};
		virtual char const* GetKeyTypeName() const = 0;
		virtual char const* GetValueTypeName() const = 0;
		virtual void* GetRaw() = 0;
		virtual bool GetValue(lua_State* L, int luaKeyIndex) = 0;
		virtual bool SetValue(lua_State* L, int luaKeyIndex, int luaValueIndex) = 0;
		virtual int Next(lua_State* L, int luaKeyIndex) = 0;
		virtual unsigned Length() = 0;
	};

	
	template <class TKey, class TValue>
	class MultiHashMapByRefProxyImpl : public MapProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<TKey>, "MultiHashMapByRefProxyImpl template parameter should not be a pointer type!");
		static_assert(!std::is_pointer_v<TValue>, "MultiHashMapByRefProxyImpl template parameter should not be a pointer type!");

		MultiHashMapByRefProxyImpl(LifetimeHandle const& lifetime, MultiHashMap<TKey, TValue> * obj)
			: object_(obj), lifetime_(lifetime)
		{}
		
		~MultiHashMapByRefProxyImpl() override
		{}

		void* GetRaw() override
		{
			return object_;
		}

		char const* GetKeyTypeName() const override
		{
			return GetTypeInfo<TKey>().TypeName.GetString();
		}

		char const* GetValueTypeName() const override
		{
			return GetTypeInfo<TValue>().TypeName.GetString();
		}

		bool GetValue(lua_State* L, int luaKeyIndex) override
		{
			TKey key;
			lua_pushvalue(L, luaKeyIndex);
			LuaRead(L, key);
			lua_pop(L, 1);

			auto value = object_->Find(key);
			if (value) {
				MakeObjectRef(L, *value, lifetime_);
				return true;
			} else {
				return false;
			}
		}

		bool SetValue(lua_State* L, int luaKeyIndex, int luaValueIndex) override
		{
			return false;
		}

		unsigned Length() override
		{
			return object_->Keys.Size();
		}

		int Next(lua_State* L, int luaKeyIndex) override
		{
			if (lua_type(L, luaKeyIndex) == LUA_TNIL) {
				if (object_->Keys.Size() > 0) {
					LuaWrite(L, object_->Keys[0]);
					MakeObjectRef(L, &object_->Values[0], lifetime_);
					return 2;
				}
			} else {
				TKey key;
				lua_pushvalue(L, luaKeyIndex);
				LuaRead(L, key);
				lua_pop(L, 1);

				auto index = object_->FindIndex(key);
				if (index != -1 && index < (int)object_->Keys.Size() - 1) {
					LuaWrite(L, object_->Keys[index + 1]);
					MakeObjectRef(L, &object_->Values[index + 1], lifetime_);
					return 2;
				}
			}

			return 0;
		}

	private:
		MultiHashMap<TKey, TValue>* object_;
		LifetimeHandle lifetime_;
	};

	
	template <class TKey, class TValue>
	class MultiHashMapByValProxyImpl : public MapProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<TKey>, "MultiHashMapByValProxyImpl template parameter should not be a pointer type!");
		static_assert(!std::is_pointer_v<TValue>, "MultiHashMapByValProxyImpl template parameter should not be a pointer type!");

		MultiHashMapByValProxyImpl(LifetimeHandle const& lifetime, MultiHashMap<TKey, TValue> * obj)
			: object_(obj), lifetime_(lifetime)
		{}
		
		~MultiHashMapByValProxyImpl() override
		{}

		void* GetRaw() override
		{
			return object_;
		}

		char const* GetKeyTypeName() const override
		{
			return GetTypeInfo<TKey>().TypeName.GetString();
		}

		char const* GetValueTypeName() const override
		{
			return GetTypeInfo<TValue>().TypeName.GetString();
		}

		bool GetValue(lua_State* L, int luaKeyIndex) override
		{
			TKey key;
			lua_pushvalue(L, luaKeyIndex);
			LuaRead(L, key);
			lua_pop(L, 1);

			auto value = object_->Find(key);
			if (value) {
				LuaWrite(L, **value);
				return true;
			} else {
				return false;
			}
		}

		bool SetValue(lua_State* L, int luaKeyIndex, int luaValueIndex) override
		{
			TKey key;
			lua_pushvalue(L, luaKeyIndex);
			LuaRead(L, key);
			lua_pop(L, 1);

			if (lua_type(L, luaValueIndex) == LUA_TNIL) {
				// FIXME - add support for remove() in hashmaps!
				// object_->Remove(key);
			} else {
				TValue value;
				lua_pushvalue(L, luaValueIndex);
				LuaRead(L, value);
				lua_pop(L, 1);

				object_->Set(key, value);
			}

			return true;
		}

		unsigned Length() override
		{
			return object_->Keys.Size();
		}

		int Next(lua_State* L, int luaKeyIndex) override
		{
			if (lua_type(L, luaKeyIndex) == LUA_TNIL) {
				if (object_->Keys.Size() > 0) {
					LuaWrite(L, object_->Keys[0]);
					LuaWrite(L, object_->Values[0]);
					return 2;
				}
			} else {
				TKey key;
				lua_pushvalue(L, luaKeyIndex);
				LuaRead(L, key);
				lua_pop(L, 1);

				auto index = object_->FindIndex(key);
				if (index != -1 && index < (int)object_->Keys.Size() - 1) {
					LuaWrite(L, object_->Keys[index + 1]);
					LuaWrite(L, object_->Values[index + 1]);
					return 2;
				}
			}

			return 0;
		}

	private:
		MultiHashMap<TKey, TValue>* object_;
		LifetimeHandle lifetime_;
	};

	
	template <class TKey, class TValue, class TInternals>
	class RefMapByRefProxyImpl : public MapProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<TKey>, "RefMapByRefProxyImpl template parameter should not be a pointer type!");
		static_assert(!std::is_pointer_v<TValue>, "RefMapByRefProxyImpl template parameter should not be a pointer type!");

		RefMapByRefProxyImpl(LifetimeHandle const& lifetime, MapBase<TInternals> * obj)
			: object_(obj), lifetime_(lifetime)
		{}
		
		~RefMapByRefProxyImpl() override
		{}

		void* GetRaw() override
		{
			return object_;
		}

		char const* GetKeyTypeName() const override
		{
			return GetTypeInfo<TKey>().TypeName.GetString();
		}

		char const* GetValueTypeName() const override
		{
			return GetTypeInfo<TValue>().TypeName.GetString();
		}

		bool GetValue(lua_State* L, int luaKeyIndex) override
		{
			TKey key;
			lua_pushvalue(L, luaKeyIndex);
			LuaRead(L, key);
			lua_pop(L, 1);

			auto value = object_->find(key);
			if (value != object_->end()) {
				MakeObjectRef(L, &value.Value(), lifetime_);
				return true;
			} else {
				return false;
			}
		}

		bool SetValue(lua_State* L, int luaKeyIndex, int luaValueIndex) override
		{
			return false;
		}

		unsigned Length() override
		{
			return object_->size();
		}

		int Next(lua_State* L, int luaKeyIndex) override
		{
			if (lua_type(L, luaKeyIndex) == LUA_TNIL) {
				auto it = object_->begin();
				if (it != object_->end()) {
					LuaWrite(L, it.Key());
					MakeObjectRef(L, &it.Value(), lifetime_);
					return 2;
				}
			} else {
				TKey key;
				lua_pushvalue(L, luaKeyIndex);
				LuaRead(L, key);
				lua_pop(L, 1);

				auto it = object_->find(key);
				if (it != object_->end()) {
					it++;
					if (it != object_->end()) {
						LuaWrite(L, it.Key());
						MakeObjectRef(L, &it.Value(), lifetime_);
						return 2;
					}
				}
			}

			return 0;
		}

	private:
		MapBase<TInternals>* object_;
		LifetimeHandle lifetime_;
	};

	
	template <class TKey, class TValue, class TInternals>
	class RefMapByValProxyImpl : public MapProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<TKey>, "RefMapByValProxyImpl template parameter should not be a pointer type!");
		static_assert(!std::is_pointer_v<TValue>, "RefMapByValProxyImpl template parameter should not be a pointer type!");

		RefMapByValProxyImpl(LifetimeHandle const& lifetime, MapBase<TInternals>* obj)
			: object_(obj), lifetime_(lifetime)
		{}
		
		~RefMapByValProxyImpl() override
		{}

		void* GetRaw() override
		{
			return object_;
		}

		char const* GetKeyTypeName() const override
		{
			return GetTypeInfo<TKey>().TypeName.GetString();
		}

		char const* GetValueTypeName() const override
		{
			return GetTypeInfo<TValue>().TypeName.GetString();
		}

		bool GetValue(lua_State* L, int luaKeyIndex) override
		{
			TKey key;
			lua_pushvalue(L, luaKeyIndex);
			LuaRead(L, key);
			lua_pop(L, 1);

			auto value = object_->find(key);
			if (value != object_->end()) {
				LuaWrite(L, value.Value());
				return true;
			} else {
				return false;
			}
		}

		bool SetValue(lua_State* L, int luaKeyIndex, int luaValueIndex) override
		{
			TKey key;
			lua_pushvalue(L, luaKeyIndex);
			LuaRead(L, key);
			lua_pop(L, 1);

			if (lua_type(L, luaValueIndex) == LUA_TNIL) {
				// FIXME - add support for remove() in refmaps!
				// object_->Remove(key);
			} else {
				TValue value;
				lua_pushvalue(L, luaValueIndex);
				LuaRead(L, value);
				lua_pop(L, 1);

				object_->insert(key, value);
			}

			return true;
		}

		unsigned Length() override
		{
			return object_->size();
		}

		int Next(lua_State* L, int luaKeyIndex) override
		{
			if (lua_type(L, luaKeyIndex) == LUA_TNIL) {
				auto it = object_->begin();
				if (it != object_->end()) {
					LuaWrite(L, it.Key());
					LuaWrite(L, it.Value());
					return 2;
				}
			} else {
				TKey key;
				lua_pushvalue(L, luaKeyIndex);
				LuaRead(L, key);
				lua_pop(L, 1);

				auto it = object_->find(key);
				if (it != object_->end()) {
					it++;
					if (it != object_->end()) {
						LuaWrite(L, it.Key());
						LuaWrite(L, it.Value());
						return 2;
					}
				}
			}

			return 0;
		}

	private:
		MapBase<TInternals>* object_;
		LifetimeHandle lifetime_;
	};


	class MapProxy : private Userdata<MapProxy>, public Indexable, public NewIndexable,
		public Lengthable, public Iterable, public Stringifiable, public Pushable, public GarbageCollected
	{
	public:
		static char const * const MetatableName;

		template <class TKey, class TValue>
		inline static MultiHashMapByRefProxyImpl<TKey, TValue>* MakeByRef(lua_State* L, MultiHashMap<TKey, TValue>* object, LifetimeHandle const& lifetime)
		{
			auto self = NewWithExtraData(L, sizeof(MultiHashMapByRefProxyImpl<TKey, TValue>), lifetime);
			return new (self->GetImpl()) MultiHashMapByRefProxyImpl<TKey, TValue>(lifetime, object);
		}

		template <class TKey, class TValue>
		inline static MultiHashMapByValProxyImpl<TKey, TValue>* MakeByVal(lua_State* L, MultiHashMap<TKey, TValue>* object, LifetimeHandle const& lifetime)
		{
			auto self = NewWithExtraData(L, sizeof(MultiHashMapByValProxyImpl<TKey, TValue>), lifetime);
			return new (self->GetImpl()) MultiHashMapByValProxyImpl<TKey, TValue>(lifetime, object);
		}

		template <class TKey, class TValue>
		inline static RefMapByRefProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>>* MakeByRef(lua_State* L, RefMap<TKey, TValue>* object, LifetimeHandle const& lifetime)
		{
			auto self = NewWithExtraData(L, sizeof(RefMapByRefProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>>), lifetime);
			return new (self->GetImpl()) RefMapByRefProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>>(lifetime, object);
		}

		template <class TKey, class TValue>
		inline static RefMapByValProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>>* MakeByVal(lua_State* L, RefMap<TKey, TValue>* object, LifetimeHandle const& lifetime)
		{
			auto self = NewWithExtraData(L, sizeof(RefMapByValProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>>), lifetime);
			return new (self->GetImpl()) RefMapByValProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>>(lifetime, object);
		}

		template <class TKey, class TValue>
		inline static RefMapByRefProxyImpl<TKey, TValue, MapInternals<TKey, TValue>>* MakeByRef(lua_State* L, Map<TKey, TValue>* object, LifetimeHandle const& lifetime)
		{
			auto self = NewWithExtraData(L, sizeof(RefMapByRefProxyImpl<TKey, TValue, MapInternals<TKey, TValue>>), lifetime);
			return new (self->GetImpl()) RefMapByRefProxyImpl<TKey, TValue, MapInternals<TKey, TValue>>(lifetime, object);
		}

		template <class TKey, class TValue>
		inline static RefMapByValProxyImpl<TKey, TValue, MapInternals<TKey, TValue>>* MakeByVal(lua_State* L, Map<TKey, TValue>* object, LifetimeHandle const& lifetime)
		{
			auto self = NewWithExtraData(L, sizeof(RefMapByValProxyImpl<TKey, TValue, MapInternals<TKey, TValue>>), lifetime);
			return new (self->GetImpl()) RefMapByValProxyImpl<TKey, TValue, MapInternals<TKey, TValue>>(lifetime, object);
		}

		inline MapProxyImplBase* GetImpl()
		{
			return reinterpret_cast<MapProxyImplBase*>(this + 1);
		}

		inline bool IsAlive(lua_State* L) const
		{
			return lifetime_.IsAlive(L);
		}

		template <class TKey, class TValue>
		MultiHashMapByRefProxyImpl<TKey, TValue>* GetByRefMultiHashMap(lua_State* L)
		{
			if (!lifetime_.IsAlive(L)) {
				return nullptr;
			}
			
			return dynamic_cast<MultiHashMapByRefProxyImpl<TKey, TValue>*>(GetImpl());
		}

		template <class TKey, class TValue>
		MultiHashMapByValProxyImpl<TKey, TValue>* GetByValMultiHashMap(lua_State* L)
		{
			if (!lifetime_.IsAlive(L)) {
				return nullptr;
			}
			
			return dynamic_cast<MultiHashMapByValProxyImpl<TKey, TValue>*>(GetImpl());
		}

		template <class TKey, class TValue>
		RefMapByRefProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>>* GetByRefRefMap(lua_State* L)
		{
			if (!lifetime_.IsAlive(L)) {
				return nullptr;
			}
			
			return dynamic_cast<RefMapByRefProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>>*>(GetImpl());
		}

		template <class TKey, class TValue>
		RefMapByValProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>>* GetByValRefMap(lua_State* L)
		{
			if (!lifetime_.IsAlive(L)) {
				return nullptr;
			}
			
			return dynamic_cast<RefMapByValProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>>*>(GetImpl());
		}

	private:
		LifetimeHandle lifetime_;

		MapProxy(LifetimeHandle const& lifetime)
			: lifetime_(lifetime)
		{}

		~MapProxy()
		{
			GetImpl()->~MapProxyImplBase();
		}

	protected:
		friend Userdata<MapProxy>;

		int Index(lua_State* L);
		int NewIndex(lua_State* L);
		int Length(lua_State* L);
		int Next(lua_State* L);
		int ToString(lua_State* L);
		int GC(lua_State* L);
	};

	template <class T>
	struct IsMapLike { static constexpr bool Value = false; };

	template <class TK, class TV>
	struct IsMapLike<Map<TK, TV>>
	{ 
		static constexpr bool Value = true; 
		using TKey = TK;
		using TValue = TV;
	};

	template <class TK, class TV>
	struct IsMapLike<RefMap<TK, TV>>
	{ 
		static constexpr bool Value = true;
		using TKey = TK;
		using TValue = TV;
	};

	template <class TK, class TV>
	struct IsMapLike<MultiHashMap<TK, TV>>
	{ 
		static constexpr bool Value = true;
		using TKey = TK;
		using TValue = TV;
	};

	template <class TK, class TV>
	struct IsMapLike<VirtualMultiHashMap<TK, TV>>
	{ 
		static constexpr bool Value = true;
		using TKey = TK;
		using TValue = TV;
	};

	template <class T>
	inline void push_map_proxy_by_ref(lua_State* L, LifetimeHandle const& lifetime, T* v)
	{
		MapProxy::MakeByRef<T>(L, v, lifetime);
	}

	template <class TKey, class TValue>
	inline MultiHashMapByValProxyImpl<TKey, TValue>* checked_get_map_proxy(lua_State* L, int index)
	{
		auto proxy = Userdata<MapProxy>::CheckUserData(L, index);
		auto const& keyTypeName = GetTypeInfo<TKey>().TypeName.GetString();
		auto const& valueTypeName = GetTypeInfo<TValue>().TypeName.GetString();
		if (strcmp(proxy->GetImpl()->GetKeyTypeName(), keyTypeName) == 0
			&& strcmp(proxy->GetImpl()->GetValueTypeName(), valueTypeName) == 0) {
			auto obj = proxy->GetByValMultiHashMap<TKey, TValue>();
			if (obj == nullptr) {
				luaL_error(L, "Argument %d: got Map<%s, %s> whose lifetime has expired", index, keyTypeName, valueTypeName);
				return nullptr;
			} else {
				return obj;
			}
		} else {
			luaL_error(L, "Argument %d: expected Map<%s, %s>, got Map<%s, %s>", index, 
				keyTypeName, valueTypeName, proxy->GetImpl()->GetKeyTypeName(), proxy->GetImpl()->GetValueTypeName());
			return nullptr;
		}
	}
}
