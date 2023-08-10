#pragma once

#include <Lua/LuaHelpers.h>
#include <Lua/LuaUserdata.h>
#include <Lua/Shared/LuaLifetime.h>

namespace bg3se::lua
{
	LifetimeHolder GetCurrentLifetime();

	class GenericPropertyMap
	{
	public:
		struct RawPropertyAccessors
		{
			using Getter = bool (lua_State* L, LifetimeHolder const& lifetime, void* object, std::size_t offset);
			using Setter = bool (lua_State* L, LifetimeHolder const& lifetime, void* object, int index, std::size_t offset);

			FixedString Name;
			Getter* Get;
			Setter* Set;
			std::size_t Offset;
		};

		bool GetRawProperty(lua_State* L, LifetimeHolder const& lifetime, void* object, FixedString const& prop) const;
		bool SetRawProperty(lua_State* L, LifetimeHolder const& lifetime, void* object, FixedString const& prop, int index) const;
		void AddRawProperty(char const* prop, typename RawPropertyAccessors::Getter* getter,
			typename RawPropertyAccessors::Setter* setter, std::size_t offset);

		FixedString Name;
		std::unordered_map<FixedString, RawPropertyAccessors> Properties;
		std::vector<FixedString> Parents;
	};

	template <class T>
	class LuaPropertyMap : public GenericPropertyMap
	{
	public:
		struct PropertyAccessors
		{
			using Getter = bool (lua_State* L, LifetimeHolder const& lifetime, T* object, std::size_t offset);
			using Setter = bool (lua_State* L, LifetimeHolder const& lifetime, T* object, int index, std::size_t offset);
		};

		inline bool GetProperty(lua_State* L, LifetimeHolder const& lifetime, T* object, FixedString const& prop) const
		{
#if defined(_DEBUG)
			__try {
				return GetRawProperty(L, lifetime, (void*)object, prop);
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				ERR("Exception while reading property %s.%s", Name.GetString(), prop.GetString());
				return false;
			}
#else
			return GetRawProperty(L, lifetime, (void*)object, prop);
#endif
		}

		inline bool SetProperty(lua_State* L, LifetimeHolder const& lifetime, T* object, FixedString const& prop, int index) const
		{
#if defined(_DEBUG)
			__try {
				return SetRawProperty(L, lifetime, (void*)object, prop, index);
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				ERR("Exception while writing property %s.%s", Name.GetString(), prop.GetString());
				return false;
			}
#else
			return SetRawProperty(L, lifetime, (void*)object, prop, index);
#endif
		}

		inline bool GetProperty(lua_State* L, LifetimeHolder const& lifetime, T* object, RawPropertyAccessors const& prop) const
		{
			auto getter = (typename PropertyAccessors::Getter*)prop.Get;

#if defined(_DEBUG)
			__try {
				return getter(L, lifetime, object, prop.Offset);
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				ERR("Exception while reading property %s.%s", Name.GetString(), prop.Name.GetString());
				return false;
			}
#else
			return getter(L, lifetime, object, prop.Offset);
#endif
		}

		inline bool SetProperty(lua_State* L, LifetimeHolder const& lifetime, T* object, RawPropertyAccessors const& prop, int index) const
		{
			auto setter = (typename PropertyAccessors::Setter*)prop.Set;

#if defined(_DEBUG)
			__try {
				return setter(L, lifetime, object, index, prop.Offset);
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				ERR("Exception while writing property %s.%s", Name.GetString(), prop.Name.GetString());
				return false;
			}
#else
			return setter(L, lifetime, object, index, prop.Offset);
#endif
		}

		inline void AddProperty(char const* prop, typename PropertyAccessors::Getter* getter,
			typename PropertyAccessors::Setter* setter, std::size_t offset)
		{
			AddRawProperty(prop, (RawPropertyAccessors::Getter*)getter, (RawPropertyAccessors::Setter*)setter, offset);
		}
	};

	template <class T>
	struct StaticLuaPropertyMap
	{
		using ObjectType = T;

		static LuaPropertyMap<T> PropertyMap;
	};

	class ObjectProxyImplBase
	{
	public:
		inline virtual ~ObjectProxyImplBase() {};
		virtual FixedString const& GetTypeName() const = 0;
		virtual void* GetRaw() = 0;
		virtual bool GetProperty(lua_State* L, FixedString const& prop) = 0;
		virtual bool SetProperty(lua_State* L, FixedString const& prop, int index) = 0;
		virtual int Next(lua_State* L, FixedString const& key) = 0;
		virtual bool IsA(FixedString const& typeName) = 0;
	};

	template <class T>
	struct ObjectProxyHelpers
	{
		static bool GetProperty(lua_State* L, T* object, LifetimeHolder const& lifetime, FixedString const& prop)
		{
			auto const& map = StaticLuaPropertyMap<T>::PropertyMap;
			auto fetched = map.GetProperty(L, lifetime, object, prop);
			if (!fetched) {
				luaL_error(L, "Object of type '%s' has no property named '%s'", TypeInfo<T>::TypeName, prop.GetString());
				return false;
			}

			return true;
		}

		static bool SetProperty(lua_State* L, T* object, LifetimeHolder const& lifetime, FixedString const& prop, int index)
		{
			auto const& map = StaticLuaPropertyMap<T>::PropertyMap;
			auto ok = map.SetProperty(L, lifetime, object, prop, index);
			if (!ok) {
				luaL_error(L, "Object of type '%s' has no property named '%s'", TypeInfo<T>::TypeName, prop.GetString());
				return false;
			}

			return true;
		}

		static int Next(lua_State* L, T* object, LifetimeHolder const& lifetime, FixedString const& key)
		{
			auto const& map = StaticLuaPropertyMap<T>::PropertyMap;
			if (!key) {
				if (!map.Properties.empty()) {
					StackCheck _(L, 2);
					auto it = map.Properties.begin();
					push(L, it->first);
					if (!map.GetProperty(L, lifetime, object, it->second)) {
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
						if (!map.GetProperty(L, lifetime, object, it->second)) {
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

		ObjectProxyRefImpl(LifetimeHolder const& lifetime, T * obj)
			: object_(obj), lifetime_(lifetime)
		{}
		
		~ObjectProxyRefImpl() override
		{}

		T* Get() const
		{
			return object_;
		}

		void* GetRaw() override
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
			return ObjectProxyHelpers<T>::SetProperty(L, object_, lifetime_, prop, index);
		}

		int Next(lua_State* L, FixedString const& key) override
		{
			return ObjectProxyHelpers<T>::Next(L, object_, lifetime_, key);
		}

		bool IsA(FixedString const& typeName) override
		{
			return ObjectProxyHelpers<T>::IsA(typeName);
		}

	private:
		T* object_;
		LifetimeHolder lifetime_;
	};


	// Object proxy that owns the contained object and deletes the object on GC
	template <class T>
	class ObjectProxyOwnerImpl : public ObjectProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<T>, "ObjectProxyImpl template parameter should not be a pointer type!");

		ObjectProxyOwnerImpl(LifetimePool& pool, Lifetime* lifetime, T* obj)
			: lifetime_(pool, lifetime), 
			object_(obj, &GameDelete<T>)
		{}

		~ObjectProxyOwnerImpl() override
		{
			lifetime_.GetLifetime()->Kill();
		}

		T* Get() const
		{
			return object_.get();
		}

		void* GetRaw() override
		{
			return object_.get();
		}

		FixedString const& GetTypeName() const override
		{
			return StaticLuaPropertyMap<T>::PropertyMap.Name;
		}

		bool GetProperty(lua_State* L, FixedString const& prop) override
		{
			return ObjectProxyHelpers<T>::GetProperty(L, object_.get(), lifetime_.Get(), prop);
		}

		bool SetProperty(lua_State* L, FixedString const& prop, int index) override
		{
			return ObjectProxyHelpers<T>::SetProperty(L, object_.get(), lifetime_.Get(), prop, index);
		}

		int Next(lua_State* L, FixedString const& key) override
		{
			return ObjectProxyHelpers<T>::Next(L, object_.get(), lifetime_.Get(), key);
		}

		bool IsA(FixedString const& typeName) override
		{
			return ObjectProxyHelpers<T>::IsA(typeName);
		}

	private:
		GameUniquePtr<T> object_;
		LifetimeReference lifetime_;
	};


	class ObjectProxy : private Userdata<ObjectProxy>, public Indexable, public NewIndexable, 
		public Iterable, public Stringifiable, public Pushable, public GarbageCollected
	{
	public:
		static char const * const MetatableName;

		template <class T>
		inline static ObjectProxyRefImpl<T>* MakeRef(lua_State* L, T* object, LifetimeHolder const& lifetime)
		{
			auto self = NewWithExtraData(L, sizeof(ObjectProxyRefImpl<T>), lifetime);
			return new (self->GetImpl()) ObjectProxyRefImpl<T>(lifetime, object);
		}

		template <class T>
		inline static ObjectProxyOwnerImpl<T>* MakeOwner(lua_State* L, LifetimePool& pool, T* obj)
		{
			auto lifetime = pool.Allocate();
			auto self = NewWithExtraData(L, sizeof(ObjectProxyOwnerImpl<T>), LifetimeHolder(pool, lifetime));
			return new (self->GetImpl()) ObjectProxyOwnerImpl<T>(pool, lifetime, obj);
		}

		template <class T, class... Args>
		inline static ObjectProxyOwnerImpl<T>* MakeOwner(lua_State* L, LifetimePool& pool, Args... args)
		{
			auto lifetime = pool.Allocate();
			auto self = NewWithExtraData(L, sizeof(ObjectProxyOwnerImpl<T>), LifetimeHolder(pool, lifetime));
			auto obj = GameAlloc<T, Args...>(std::forward(args)...);
			return new (self->GetImpl()) ObjectProxyOwnerImpl<T>(pool, lifetime, obj);
		}

		inline ObjectProxyImplBase* GetImpl()
		{
			return reinterpret_cast<ObjectProxyImplBase*>(this + 1);
		}

		inline bool IsAlive() const
		{
			return lifetime_.IsAlive();
		}

		template <class T>
		T* Get()
		{
			if (!lifetime_.IsAlive()) {
				return nullptr;
			}

			if (GetImpl()->IsA(StaticLuaPropertyMap<T>::PropertyMap.Name)) {
				return reinterpret_cast<T*>(GetImpl()->GetRaw());
			} else {
				return nullptr;
			}
		}

	private:
		LifetimeReference lifetime_;

		ObjectProxy(LifetimeHolder const& lifetime)
			: lifetime_(lifetime)
		{}

		~ObjectProxy()
		{
			GetImpl()->~ObjectProxyImplBase();
		}

	protected:
		friend Userdata<ObjectProxy>;

		int Index(lua_State* L);
		int NewIndex(lua_State* L);
		int Next(lua_State* L);
		int ToString(lua_State* L);
		int GC(lua_State* L);
	};

	template <class T>
	inline void push_proxy(lua_State* L, LifetimeHolder const& lifetime, T const& v)
	{
		if constexpr (std::is_pointer_v<T> 
			&& (std::is_base_of_v<HasObjectProxy, std::remove_pointer_t<T>>
				|| HasObjectProxyTag<std::remove_pointer_t<T>>::HasProxy)) {
			if (v) {
				ObjectProxy::MakeRef<std::remove_pointer_t<T>>(L, v, lifetime);
			} else {
				lua_pushnil(L);
			}
		} else {
			push(L, v);
		}
	}

	template <class T>
	inline T* checked_get_proxy(lua_State* L, int index)
	{
		auto proxy = Userdata<ObjectProxy>::CheckUserData(L, index);
		auto const& typeName = StaticLuaPropertyMap<T>::PropertyMap.Name;
		if (proxy->GetImpl()->IsA(typeName)) {
			auto obj = proxy->Get<T>();
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
