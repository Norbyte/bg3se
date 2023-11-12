#pragma once

namespace bg3se::lua
{
	LifetimeHandle GetCurrentLifetime();

	class ObjectProxyImplBase
	{
	public:
		inline virtual ~ObjectProxyImplBase() {};
		virtual FixedString const& GetTypeName() const = 0;
		virtual void* GetRaw(lua_State* L) = 0;
		virtual bool GetProperty(lua_State* L, FixedString const& prop) = 0;
		virtual bool SetProperty(lua_State* L, FixedString const& prop, int index) = 0;
		virtual int Next(lua_State* L, FixedString const& key) = 0;
		virtual bool IsA(FixedString const& typeName) = 0;
		virtual GenericPropertyMap& GetPropertyMap() = 0;
	};

	template <class T>
	struct ObjectProxyHelpers
	{
		static bool GetProperty(lua_State* L, T* object, LifetimeHandle const& lifetime, FixedString const& prop)
		{
			auto const& map = StaticLuaPropertyMap<T>::PropertyMap;
			auto result = map.GetProperty(L, lifetime, object, prop);
			switch (result) {
			case PropertyOperationResult::Success:
				break;

			case PropertyOperationResult::NoSuchProperty:
				luaL_error(L, "Cannot get property: %s.%s - property does not exist", GetTypeInfo<T>().TypeName.GetString(), prop.GetString());
				push(L, nullptr);
				break;

			case PropertyOperationResult::Unknown:
			default:
				luaL_error(L, "Cannot get property %s.%s - unknown error", GetTypeInfo<T>().TypeName.GetString(), prop.GetString());
				push(L, nullptr);
				break;
			}

			return true;
		}

		static bool SetProperty(lua_State* L, T* object, FixedString const& prop, int index)
		{
			auto const& map = StaticLuaPropertyMap<T>::PropertyMap;
			auto result = map.SetProperty(L, object, prop, index);
			switch (result) {
			case PropertyOperationResult::Success:
				return true;

			case PropertyOperationResult::NoSuchProperty:
				luaL_error(L, "Cannot set property %s.%s - property does not exist", GetTypeInfo<T>().TypeName.GetString(), prop.GetString());
				return false;

			case PropertyOperationResult::ReadOnly:
				luaL_error(L, "Cannot set property %s.%s - property is read-only", GetTypeInfo<T>().TypeName.GetString(), prop.GetString());
				return false;

			case PropertyOperationResult::UnsupportedType:
				luaL_error(L, "Cannot set property %s.%s - cannot write properties of this type", GetTypeInfo<T>().TypeName.GetString(), prop.GetString());
				return false;

			case PropertyOperationResult::Unknown:
			default:
				luaL_error(L, "Cannot set property %s.%s - unknown error", GetTypeInfo<T>().TypeName.GetString(), prop.GetString());
				return false;
			}
		}

		static int Next(lua_State* L, T* object, LifetimeHandle const& lifetime, FixedString const& key)
		{
			auto const& map = StaticLuaPropertyMap<T>::PropertyMap;
			if (!key) {
				if (!map.Properties.empty()) {
					StackCheck _(L, 2);
					auto it = map.Properties.begin();
					push(L, it->first);
					if (map.GetProperty(L, lifetime, object, it->second) != PropertyOperationResult::Success) {
						push(L, nullptr);
					}

					return 2;
				}
			} else {
				auto it = map.Properties.find(key);
				if (it != map.Properties.end()) {
					++it;
					if (it != map.Properties.end()) {
						StackCheck _(L, 2);
						push(L, it->first);
						if (map.GetProperty(L, lifetime, object, it->second) != PropertyOperationResult::Success) {
							push(L, nullptr);
						}

						return 2;
					}
				}
			}

			return 0;
		}

		static bool IsA(FixedString const& typeName)
		{
			auto const& map = StaticLuaPropertyMap<T>::PropertyMap;
			if (map.Name == typeName) {
				return true;
			}

			for (auto const& parent : map.Parents) {
				if (parent == typeName) {
					return true;
				}
			}

			return false;
		}
	};

	template <class T>
	class ObjectProxyRefImpl : public ObjectProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<T>, "ObjectProxyImpl template parameter should not be a pointer type!");

		ObjectProxyRefImpl(LifetimeHandle const& lifetime, T * obj)
			: object_(obj), lifetime_(lifetime)
		{}
		
		~ObjectProxyRefImpl() override
		{}

		T* Get() const
		{
			return object_;
		}

		void* GetRaw(lua_State* L) override
		{
			return object_;
		}

		FixedString const& GetTypeName() const override
		{
			return StaticLuaPropertyMap<T>::PropertyMap.Name;
		}

		bool GetProperty(lua_State* L, FixedString const& prop) override
		{
			return ObjectProxyHelpers<T>::GetProperty(L, object_, lifetime_, prop);
		}

		bool SetProperty(lua_State* L, FixedString const& prop, int index) override
		{
			return ObjectProxyHelpers<T>::SetProperty(L, object_, prop, index);
		}

		int Next(lua_State* L, FixedString const& key) override
		{
			return ObjectProxyHelpers<T>::Next(L, object_, lifetime_, key);
		}

		bool IsA(FixedString const& typeName) override
		{
			return ObjectProxyHelpers<T>::IsA(typeName);
		}

		GenericPropertyMap& GetPropertyMap() override
		{
			return StaticLuaPropertyMap<T>::PropertyMap;
		}

	private:
		T* object_;
		LifetimeHandle lifetime_;
	};


	// Object proxy that owns the contained object and deletes the object on GC
	template <class T>
	class ObjectProxyOwnerImpl : public ObjectProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<T>, "ObjectProxyImpl template parameter should not be a pointer type!");

		ObjectProxyOwnerImpl(lua_State* L, LifetimeHandle const& lifetime, T* obj)
			: lifetime_(L, lifetime), 
			object_(obj, &GameDelete<T>)
		{}

		~ObjectProxyOwnerImpl() override
		{}

		T* Get() const
		{
			return object_.get();
		}

		void* GetRaw(lua_State* L) override
		{
			return object_.get();
		}

		FixedString const& GetTypeName() const override
		{
			return StaticLuaPropertyMap<T>::PropertyMap.Name;
		}

		bool GetProperty(lua_State* L, FixedString const& prop) override
		{
			return ObjectProxyHelpers<T>::GetProperty(L, object_.get(), lifetime_, prop);
		}

		bool SetProperty(lua_State* L, FixedString const& prop, int index) override
		{
			return ObjectProxyHelpers<T>::SetProperty(L, object_.get(), prop, index);
		}

		int Next(lua_State* L, FixedString const& key) override
		{
			return ObjectProxyHelpers<T>::Next(L, object_.get(), lifetime_, key);
		}

		bool IsA(FixedString const& typeName) override
		{
			return ObjectProxyHelpers<T>::IsA(typeName);
		}

		GenericPropertyMap& GetPropertyMap() override
		{
			return StaticLuaPropertyMap<T>::PropertyMap;
		}

	private:
		GameUniquePtr<T> object_;
		LifetimeOwnerPin lifetime_;
	};


	class LegacyObjectProxy : private Userdata<LegacyObjectProxy>, public Indexable, public NewIndexable,
		public Iterable, public Stringifiable
	{
	public:
		static char const * const MetatableName;

		template <class T>
		inline static ObjectProxyRefImpl<T>* MakeRef(lua_State* L, T* object, LifetimeHandle const& lifetime)
		{
			if (!StaticLuaPropertyMap<T>::PropertyMap.ValidatePropertyMap(object)) {
				push(L, nullptr);
				return nullptr;
			} else {
				auto self = NewWithExtraData(L, sizeof(ObjectProxyRefImpl<T>), lifetime);
				return new (self->GetImpl()) ObjectProxyRefImpl<T>(lifetime, object);
			}
		}

		template <class T>
		inline static ObjectProxyOwnerImpl<T>* MakeOwner(lua_State* L, LifetimePool& pool, T* obj)
		{
			auto lifetime = pool.Allocate();
			auto self = NewWithExtraData(L, sizeof(ObjectProxyOwnerImpl<T>), lifetime);
			return new (self->GetImpl()) ObjectProxyOwnerImpl<T>(L, lifetime, obj);
		}

		template <class T, class... Args>
		inline static ObjectProxyOwnerImpl<T>* MakeOwner(lua_State* L, LifetimePool& pool, Args... args)
		{
			auto lifetime = pool.Allocate();
			auto self = NewWithExtraData(L, sizeof(ObjectProxyOwnerImpl<T>), lifetime);
			auto obj = GameAlloc<T, Args...>(std::forward(args)...);
			return new (self->GetImpl()) ObjectProxyOwnerImpl<T>(L, lifetime, obj);
		}

		inline ObjectProxyImplBase* GetImpl()
		{
			return reinterpret_cast<ObjectProxyImplBase*>(this + 1);
		}

		inline bool IsAlive(lua_State* L) const
		{
			return lifetime_.IsAlive(L);
		}

		inline void* GetRaw(lua_State* L)
		{
			if (!lifetime_.IsAlive(L)) {
				return nullptr;
			}

			return GetImpl()->GetRaw(L);
		}

		static void* GetRaw(lua_State* L, int index, FixedString const& typeName);
		static void* TryGetRaw(lua_State* L, int index, FixedString const& typeName);

		template <class T>
		T* Get(lua_State* L)
		{
			if (!lifetime_.IsAlive(L)) {
				return nullptr;
			}

			if (GetImpl()->IsA(StaticLuaPropertyMap<T>::PropertyMap.Name)) {
				return reinterpret_cast<T*>(GetImpl()->GetRaw(L));
			} else {
				return nullptr;
			}
		}

		template <class T>
		inline static T* Get(lua_State* L, int index)
		{
			auto const& typeName = StaticLuaPropertyMap<T>::PropertyMap.Name;
			auto obj = GetRaw(L, index, typeName);
			return reinterpret_cast<T*>(obj);
		}

		template <class T>
		inline static T* TryGet(lua_State* L, int index)
		{
			auto const& typeName = StaticLuaPropertyMap<T>::PropertyMap.Name;
			auto obj = TryGetRaw(L, index, typeName);
			return reinterpret_cast<T*>(obj);
		}

	private:
		LifetimeHandle lifetime_;

		LegacyObjectProxy(LifetimeHandle const& lifetime)
			: lifetime_(lifetime)
		{}

		~LegacyObjectProxy()
		{
			GetImpl()->~ObjectProxyImplBase();
		}

	protected:
		friend Userdata<LegacyObjectProxy>;

		int Index(lua_State* L);
		int NewIndex(lua_State* L);
		int Next(lua_State* L);
		int ToString(lua_State* L);
	};

	template <class T>
	inline T* checked_get_proxy(lua_State* L, int index)
	{
		auto proxy = Userdata<LegacyObjectProxy>::CheckUserData(L, index);
		auto const& typeName = StaticLuaPropertyMap<T>::PropertyMap.Name;
		if (proxy->GetImpl()->IsA(typeName)) {
			auto obj = proxy->Get<T>(L);
			if (obj == nullptr) {
				luaL_error(L, "Argument %d: got object of type '%s' whose lifetime has expired", index, typeName.GetString());
				return nullptr;
			} else {
				return obj;
			}
		} else {
			luaL_error(L, "Argument %d: expected an object of type '%s', got '%s'", index, typeName.GetString(), proxy->GetImpl()->GetTypeName());
			return nullptr;
		}
	}
}
