#pragma once

namespace bg3se::lua
{
	LifetimeHandle GetCurrentLifetime();

	class MapProxyImplBase
	{
	public:
		inline virtual ~MapProxyImplBase() {};
		virtual unsigned GetContainerClass() = 0;
		virtual char const* GetKeyTypeName() const = 0;
		virtual char const* GetValueTypeName() const = 0;
		virtual TypeInformation const& GetContainerType() const = 0;
		virtual TypeInformation const& GetKeyType() const = 0;
		virtual TypeInformation const& GetValueType() const = 0;
		virtual void* GetRaw() = 0;
		virtual bool GetValue(lua_State* L, int luaKeyIndex) = 0;
		virtual bool SetValue(lua_State* L, int luaKeyIndex, int luaValueIndex) = 0;
		virtual int Next(lua_State* L, int luaKeyIndex) = 0;
		virtual unsigned Length() = 0;
		virtual bool Unserialize(lua_State* L, int index) = 0;
		virtual void Serialize(lua_State* L) = 0;
	};

	
	template <class TKey, class TValue>
	class MultiHashMapProxyImpl : public MapProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<TKey>, "MultiHashMapProxyImpl template parameter should not be a pointer type!");

		using ContainerType = MultiHashMap<TKey, TValue>;
		static constexpr unsigned ContainerClassId = 1;

		MultiHashMapProxyImpl(LifetimeHandle const& lifetime, ContainerType* obj)
			: object_(obj), lifetime_(lifetime)
		{}
		
		~MultiHashMapProxyImpl() override
		{}

		unsigned GetContainerClass() override
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

		ContainerType* Get() const
		{
			return object_;
		}

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
				push(L, *value, lifetime_);
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
					push(L, &object_->Keys[0], lifetime_);
					push(L, &object_->Values[0], lifetime_);
					return 2;
				}
			} else {
				auto key = get<TKey>(L, luaKeyIndex);
				auto index = object_->FindIndex(key);
				if (index != -1 && index < (int)object_->Keys.size() - 1) {
					push(L, &object_->Keys[index + 1], lifetime_);
					push(L, &object_->Values[index + 1], lifetime_);
					return 2;
				}
			}

			return 0;
		}

		bool Unserialize(lua_State* L, int index) override
		{
			if constexpr (std::is_default_constructible_v<TValue>) {
				lua::Unserialize(L, index, object_);
				return true;
			} else {
				return false;
			}
		}

		void Serialize(lua_State* L) override
		{
			lua::Serialize(L, object_);
		}

	private:
		ContainerType* object_;
		LifetimeHandle lifetime_;
	};

	
	template <class TKey, class TValue, class TInternals, unsigned TContainerClassId>
	class RefMapProxyImpl : public MapProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<TKey>, "RefMapProxyImpl template parameter should not be a pointer type!");

		using ContainerType = MapBase<TInternals>;
		static constexpr unsigned ContainerClassId = TContainerClassId;

		RefMapProxyImpl(LifetimeHandle const& lifetime, ContainerType* obj)
			: object_(obj), lifetime_(lifetime)
		{}
		
		~RefMapProxyImpl() override
		{}

		unsigned GetContainerClass() override
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

		ContainerType* Get() const
		{
			return object_;
		}

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
				push(L, &value.Value(), lifetime_);
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
					push(L, &it.Key(), lifetime_);
					push(L, &it.Value(), lifetime_);
					return 2;
				}
			} else {
				auto key = get<TKey>(L, luaKeyIndex);
				auto it = object_->find(key);
				if (it != object_->end()) {
					it++;
					if (it != object_->end()) {
						push(L, &it.Key(), lifetime_);
						push(L, &it.Value(), lifetime_);
						return 2;
					}
				}
			}

			return 0;
		}

		bool Unserialize(lua_State* L, int index) override
		{
			if constexpr (std::is_default_constructible_v<TValue>) {
				lua::Unserialize(L, index, object_);
				return true;
			} else {
				return false;
			}
		}

		void Serialize(lua_State* L) override
		{
			lua::Serialize(L, object_);
		}

	private:
		ContainerType* object_;
		LifetimeHandle lifetime_;
	};


	class MapProxy : private Userdata<MapProxy>, public Indexable, public NewIndexable,
		public Lengthable, public Iterable, public Stringifiable
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
		inline static RefMapProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>, 2>* Make(lua_State* L, RefMap<TKey, TValue>* object, LifetimeHandle const& lifetime)
		{
			auto self = NewWithExtraData(L, sizeof(RefMapProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>, 2>), lifetime);
			return new (self->GetImpl()) RefMapProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>, 2>(lifetime, object);
		}

		template <class TKey, class TValue>
		inline static RefMapProxyImpl<TKey, TValue, MapInternals<TKey, TValue>, 3>* Make(lua_State* L, Map<TKey, TValue>* object, LifetimeHandle const& lifetime)
		{
			auto self = NewWithExtraData(L, sizeof(RefMapProxyImpl<TKey, TValue, MapInternals<TKey, TValue>, 3>), lifetime);
			return new (self->GetImpl()) RefMapProxyImpl<TKey, TValue, MapInternals<TKey, TValue>, 3>(lifetime, object);
		}

		inline MapProxyImplBase* GetImpl()
		{
			return reinterpret_cast<MapProxyImplBase*>(this + 1);
		}

		inline bool IsAlive(lua_State* L) const
		{
			return lifetime_.IsAlive(L);
		}

		template <class T>
		static T* CheckedGetImpl(lua_State* L, int index)
		{
			auto self = CheckUserData(L, index);
			auto impl = self->GetImpl();

			if (!self->lifetime_.IsAlive(L)) {
				luaL_error(L, "Attempted to reference dead Map<%s, %s>", impl->GetKeyTypeName(), impl->GetValueTypeName());
				return nullptr;
			}

			if (impl->GetContainerClass() != T::ContainerClassId) {
				luaL_error(L, "Attempted to reference Map<%s, %s> with incorrect internal container type", impl->GetKeyTypeName(), impl->GetValueTypeName());
				return nullptr;
			}

			return static_cast<T*>(impl);
		}

		template <class TKey, class TValue>
		static inline auto CheckedGetMultiHashMap(lua_State* L, int index)
		{
			return CheckedGetImpl<MultiHashMapProxyImpl<TKey, TValue>>(L, index);
		}

		template <class TKey, class TValue>
		static inline auto CheckedGetRefMap(lua_State* L, int index)
		{
			return CheckedGetImpl<RefMapProxyImpl<TKey, TValue, RefMapInternals<TKey, TValue>, 2>>(L, index);
		}

		template <class TKey, class TValue>
		static inline auto CheckedGetMap(lua_State* L, int index)
		{
			return CheckedGetImpl<RefMapProxyImpl<TKey, TValue, MapInternals<TKey, TValue>, 3>>(L, index);
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
	};
}
