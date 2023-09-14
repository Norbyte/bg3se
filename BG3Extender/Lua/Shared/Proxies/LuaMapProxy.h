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
	class MultiHashMapProxyImpl : public MapProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<TKey>, "MultiHashMapProxyImpl template parameter should not be a pointer type!");
		static_assert(!std::is_pointer_v<TValue>, "MultiHashMapProxyImpl template parameter should not be a pointer type!");

		MultiHashMapProxyImpl(LifetimeHandle const& lifetime, MultiHashMap<TKey, TValue> * obj)
			: object_(obj), lifetime_(lifetime)
		{}
		
		~MultiHashMapProxyImpl() override
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
			auto key = get<TKey>(L, luaKeyIndex);
			auto value = object_->Find(key);
			if (value) {
				PushAny(L, *value, lifetime_);
				return true;
			} else {
				return false;
			}
		}

		bool SetValue(lua_State* L, int luaKeyIndex, int luaValueIndex) override
		{
			if constexpr (IsByVal<TValue>) {
				auto key = get<TKey>(L, luaKeyIndex);
				if (lua_type(L, luaValueIndex) == LUA_TNIL) {
					// FIXME - add support for remove() in hashmaps!
					// object_->Remove(key);
					return false;
				} else {
					auto value = get<TValue>(L, luaValueIndex);
					object_->Set(key, value);
				}

				return true;
			} else {
				return false;
			}
		}

		unsigned Length() override
		{
			return object_->Keys.size();
		}

		int Next(lua_State* L, int luaKeyIndex) override
		{
			if (lua_type(L, luaKeyIndex) == LUA_TNIL) {
				if (object_->Keys.size() > 0) {
					PushAny(L, &object_->Keys[0], lifetime_);
					PushAny(L, &object_->Values[0], lifetime_);
					return 2;
				}
			} else {
				auto key = get<TKey>(L, luaKeyIndex);
				auto index = object_->FindIndex(key);
				if (index != -1 && index < (int)object_->Keys.size() - 1) {
					PushAny(L, &object_->Keys[index + 1], lifetime_);
					PushAny(L, &object_->Values[index + 1], lifetime_);
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
	class RefMapProxyImpl : public MapProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<TKey>, "RefMapProxyImpl template parameter should not be a pointer type!");
		static_assert(!std::is_pointer_v<TValue>, "RefMapProxyImpl template parameter should not be a pointer type!");

		RefMapProxyImpl(LifetimeHandle const& lifetime, MapBase<TInternals> * obj)
			: object_(obj), lifetime_(lifetime)
		{}
		
		~RefMapProxyImpl() override
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
			auto key = get<TKey>(L, luaKeyIndex);
			auto value = object_->find(key);
			if (value != object_->end()) {
				PushAny(L, &value.Value(), lifetime_);
				return true;
			} else {
				return false;
			}
		}

		bool SetValue(lua_State* L, int luaKeyIndex, int luaValueIndex) override
		{
			if constexpr (IsByVal<TValue>) {
				auto key = get<TKey>(L, luaKeyIndex);
				if (lua_type(L, luaValueIndex) == LUA_TNIL) {
					// FIXME - add support for remove() in refmaps!
					// object_->Remove(key);
					return false;
				} else {
					auto value = get<TValue>(L, luaValueIndex);
					object_->insert(key, value);
				}

				return true;
			} else {
				return false;
			}
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
					PushAny(L, &it.Key(), lifetime_);
					PushAny(L, &it.Value(), lifetime_);
					return 2;
				}
			} else {
				auto key = get<TKey>(L, luaKeyIndex);
				auto it = object_->find(key);
				if (it != object_->end()) {
					it++;
					if (it != object_->end()) {
						PushAny(L, &it.Key(), lifetime_);
						PushAny(L, &it.Value(), lifetime_);
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
		inline static MultiHashMapProxyImpl<TKey, TValue>* Make(lua_State* L, MultiHashMap<TKey, TValue>* object, LifetimeHandle const& lifetime)
		{
			auto self = NewWithExtraData(L, sizeof(MultiHashMapProxyImpl<TKey, TValue>), lifetime);
			return new (self->GetImpl()) MultiHashMapProxyImpl<TKey, TValue>(lifetime, object);
		}

		template <class TKey, class TValue>
		inline static RefMapProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>>* Make(lua_State* L, RefMap<TKey, TValue>* object, LifetimeHandle const& lifetime)
		{
			auto self = NewWithExtraData(L, sizeof(RefMapProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>>), lifetime);
			return new (self->GetImpl()) RefMapProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>>(lifetime, object);
		}

		template <class TKey, class TValue>
		inline static RefMapProxyImpl<TKey, TValue, MapInternals<TKey, TValue>>* Make(lua_State* L, Map<TKey, TValue>* object, LifetimeHandle const& lifetime)
		{
			auto self = NewWithExtraData(L, sizeof(RefMapProxyImpl<TKey, TValue, MapInternals<TKey, TValue>>), lifetime);
			return new (self->GetImpl()) RefMapProxyImpl<TKey, TValue, MapInternals<TKey, TValue>>(lifetime, object);
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
		MultiHashMapProxyImpl<TKey, TValue>* GetMultiHashMap(lua_State* L)
		{
			if (!lifetime_.IsAlive(L)) {
				return nullptr;
			}
			
			return dynamic_cast<MultiHashMapProxyImpl<TKey, TValue>*>(GetImpl());
		}

		template <class TKey, class TValue>
		RefMapProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>>* GetRefMap(lua_State* L)
		{
			if (!lifetime_.IsAlive(L)) {
				return nullptr;
			}
			
			return dynamic_cast<RefMapProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>>*>(GetImpl());
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
}
