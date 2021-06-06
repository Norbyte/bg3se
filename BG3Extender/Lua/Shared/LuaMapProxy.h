#pragma once

#include <Lua/LuaHelpers.h>
#include <Lua/Shared/LuaLifetime.h>
#include <Lua/Shared/LuaPropertyMapHelpers.h>

namespace bg3se::lua
{
	LifetimeHolder GetCurrentLifetime();

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

		MultiHashMapByRefProxyImpl(LifetimeHolder const& lifetime, MultiHashMap<TKey, TValue> * obj)
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
			return TypeInfo<TKey>::TypeName;
		}

		char const* GetValueTypeName() const override
		{
			return TypeInfo<TValue>::TypeName;
		}

		bool GetValue(lua_State* L, int luaKeyIndex) override
		{
			TKey key;
			lua_pushvalue(L, luaKeyIndex);
			LuaRead(L, key);
			lua_pop(L, 1);

			auto value = object_->Find(key);
			if (value) {
				MakeObjectRef(L, lifetime_, *value);
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
					MakeObjectRef(L, lifetime_, &object_->Values[0]);
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
					MakeObjectRef(L, lifetime_, &object_->Values[index + 1]);
					return 2;
				}
			}

			return 0;
		}

	private:
		MultiHashMap<TKey, TValue>* object_;
		LifetimeHolder lifetime_;
	};

	
	template <class TKey, class TValue>
	class MultiHashMapByValProxyImpl : public MapProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<TKey>, "MultiHashMapByValProxyImpl template parameter should not be a pointer type!");
		static_assert(!std::is_pointer_v<TValue>, "MultiHashMapByValProxyImpl template parameter should not be a pointer type!");

		MultiHashMapByValProxyImpl(LifetimeHolder const& lifetime, MultiHashMap<TKey, TValue> * obj)
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
			return TypeInfo<TKey>::TypeName;
		}

		char const* GetValueTypeName() const override
		{
			return TypeInfo<TValue>::TypeName;
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
		LifetimeHolder lifetime_;
	};

	
	template <class TKey, class TValue>
	class RefMapByRefProxyImpl : public MapProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<TKey>, "RefMapByRefProxyImpl template parameter should not be a pointer type!");
		static_assert(!std::is_pointer_v<TValue>, "RefMapByRefProxyImpl template parameter should not be a pointer type!");

		RefMapByRefProxyImpl(LifetimeHolder const& lifetime, RefMap<TKey, TValue> * obj)
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
			return TypeInfo<TKey>::TypeName;
		}

		char const* GetValueTypeName() const override
		{
			return TypeInfo<TValue>::TypeName;
		}

		bool GetValue(lua_State* L, int luaKeyIndex) override
		{
			TKey key;
			lua_pushvalue(L, luaKeyIndex);
			LuaRead(L, key);
			lua_pop(L, 1);

			auto value = object_->Find(key);
			if (value) {
				MakeObjectRef(L, lifetime_, value);
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
			return object_->Count();
		}

		int Next(lua_State* L, int luaKeyIndex) override
		{
			if (lua_type(L, luaKeyIndex) == LUA_TNIL) {
				auto it = object_->begin();
				if (it != object_->end()) {
					LuaWrite(L, it.Key());
					MakeObjectRef(L, lifetime_, &it.Value());
					return 2;
				}
			} else {
				TKey key;
				lua_pushvalue(L, luaKeyIndex);
				LuaRead(L, key);
				lua_pop(L, 1);

				auto it = object_->FindIterator(key);
				if (it != object_->end()) {
					it++;
					if (it != object_->end()) {
						LuaWrite(L, it.Key());
						MakeObjectRef(L, lifetime_, &it.Value());
						return 2;
					}
				}
			}

			return 0;
		}

	private:
		RefMap<TKey, TValue>* object_;
		LifetimeHolder lifetime_;
	};

	
	template <class TKey, class TValue>
	class RefMapByValProxyImpl : public MapProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<TKey>, "RefMapByValProxyImpl template parameter should not be a pointer type!");
		static_assert(!std::is_pointer_v<TValue>, "RefMapByValProxyImpl template parameter should not be a pointer type!");

		RefMapByValProxyImpl(LifetimeHolder const& lifetime, RefMap<TKey, TValue> * obj)
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
			return TypeInfo<TKey>::TypeName;
		}

		char const* GetValueTypeName() const override
		{
			return TypeInfo<TValue>::TypeName;
		}

		bool GetValue(lua_State* L, int luaKeyIndex) override
		{
			TKey key;
			lua_pushvalue(L, luaKeyIndex);
			LuaRead(L, key);
			lua_pop(L, 1);

			auto value = object_->Find(key);
			if (value) {
				LuaWrite(L, *value);
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

				*object_->Insert(key) = value;
			}

			return true;
		}

		unsigned Length() override
		{
			return object_->Count();
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

				auto it = object_->FindIterator(key);
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
		RefMap<TKey, TValue>* object_;
		LifetimeHolder lifetime_;
	};


	class MapProxy : private Userdata<MapProxy>, public Indexable, public NewIndexable,
		public Iterable, public Stringifiable, public Pushable, public GarbageCollected
	{
	public:
		static char const * const MetatableName;

		template <class TKey, class TValue>
		inline static MultiHashMapByRefProxyImpl<TKey, TValue>* MakeByRef(lua_State* L, MultiHashMap<TKey, TValue>* object, LifetimeHolder const& lifetime)
		{
			static_assert(sizeof(MultiHashMapByRefProxyImpl<TKey, TValue>) <= sizeof(impl_), "MultiHashMapByRefProxyImpl implementation object too large!");
			auto self = New(L, lifetime);
			return new (self->impl_) MultiHashMapByRefProxyImpl<TKey, TValue>(lifetime, object);
		}

		template <class TKey, class TValue>
		inline static MultiHashMapByValProxyImpl<TKey, TValue>* MakeByVal(lua_State* L, MultiHashMap<TKey, TValue>* object, LifetimeHolder const& lifetime)
		{
			static_assert(sizeof(MultiHashMapByValProxyImpl<TKey, TValue>) <= sizeof(impl_), "MultiHashMapByValProxyImpl implementation object too large!");
			auto self = New(L, lifetime);
			return new (self->impl_) MultiHashMapByValProxyImpl<TKey, TValue>(lifetime, object);
		}

		template <class TKey, class TValue>
		inline static RefMapByRefProxyImpl<TKey, TValue>* MakeByRef(lua_State* L, RefMap<TKey, TValue>* object, LifetimeHolder const& lifetime)
		{
			static_assert(sizeof(RefMapByRefProxyImpl<TKey, TValue>) <= sizeof(impl_), "RefMapByRefProxyImpl implementation object too large!");
			auto self = New(L, lifetime);
			return new (self->impl_) RefMapByRefProxyImpl<TKey, TValue>(lifetime, object);
		}

		template <class TKey, class TValue>
		inline static RefMapByValProxyImpl<TKey, TValue>* MakeByVal(lua_State* L, RefMap<TKey, TValue>* object, LifetimeHolder const& lifetime)
		{
			static_assert(sizeof(RefMapByValProxyImpl<TKey, TValue>) <= sizeof(impl_), "RefMapByValProxyImpl implementation object too large!");
			auto self = New(L, lifetime);
			return new (self->impl_) RefMapByValProxyImpl<TKey, TValue>(lifetime, object);
		}

		inline MapProxyImplBase* GetImpl()
		{
			return reinterpret_cast<MapProxyImplBase*>(impl_);
		}

		inline bool IsAlive() const
		{
			return lifetime_.IsAlive();
		}

		template <class TKey, class TValue>
		MultiHashMapByRefProxyImpl<TKey, TValue>* GetByRefMultiHashMap()
		{
			if (!lifetime_.IsAlive()) {
				return nullptr;
			}
			
			return dynamic_cast<MultiHashMapByRefProxyImpl<TKey, TValue>*>(GetImpl());
		}

		template <class TKey, class TValue>
		MultiHashMapByValProxyImpl<TKey, TValue>* GetByValMultiHashMap()
		{
			if (!lifetime_.IsAlive()) {
				return nullptr;
			}
			
			return dynamic_cast<MultiHashMapByValProxyImpl<TKey, TValue>*>(GetImpl());
		}

		template <class TKey, class TValue>
		RefMapByRefProxyImpl<TKey, TValue>* GetByRefRefMap()
		{
			if (!lifetime_.IsAlive()) {
				return nullptr;
			}
			
			return dynamic_cast<RefMapByRefProxyImpl<TKey, TValue>*>(GetImpl());
		}

		template <class TKey, class TValue>
		RefMapByValProxyImpl<TKey, TValue>* GetByValRefMap()
		{
			if (!lifetime_.IsAlive()) {
				return nullptr;
			}
			
			return dynamic_cast<RefMapByValProxyImpl<TKey, TValue>*>(GetImpl());
		}

	private:
		LifetimeReference lifetime_;
		uint8_t impl_[40];

		MapProxy(LifetimeHolder const& lifetime)
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
		int Next(lua_State* L);
		int ToString(lua_State* L);
		int GC(lua_State* L);
	};

	template <class T>
	inline void push_map_proxy_by_ref(lua_State* L, LifetimeHolder const& lifetime, T* v)
	{
		MapProxy::MakeByRef<T>(L, v, lifetime);
	}

	template <class TKey, class TValue>
	inline MultiHashMapByValProxyImpl<TKey, TValue>* checked_get_map_proxy(lua_State* L, int index)
	{
		auto proxy = Userdata<MapProxy>::CheckUserData(L, index);
		auto const& keyTypeName = TypeInfo<TKey>::TypeName;
		auto const& valueTypeName = TypeInfo<TValue>::TypeName;
		if (strcmp(GetImpl()->GetKeyTypeName(), keyTypeName) == 0
			&& strcmp(GetImpl()->GetValueTypeName(), valueTypeName) == 0) {
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
