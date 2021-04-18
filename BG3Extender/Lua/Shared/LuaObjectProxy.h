#pragma once

#include <Lua/LuaHelpers.h>
#include <Lua/Shared/LuaLifetime.h>

namespace bg3se::lua
{
	LifetimeHolder GetCurrentLifetime();

	template <class T>
	class LuaPropertyMap
	{
	public:
		struct PropertyAccessors
		{
			using Getter = bool (lua_State* L, LifetimeHolder const& lifetime, T* object);
			using Setter = bool (lua_State* L, LifetimeHolder const& lifetime, T* object, int index);

			Getter* Get;
			Setter* Set;
		};

		bool GetProperty(lua_State* L, LifetimeHolder const& lifetime, T* object, STDString const& prop) const
		{
			auto it = Properties.find(prop);
			if (it == Properties.end()) {
				return 0;
			}

			return it->second.Get(L, lifetime, object);
		}

		bool SetProperty(lua_State* L, LifetimeHolder const& lifetime, T* object, STDString const& prop, int index) const
		{
			auto it = Properties.find(prop);
			if (it == Properties.end()) {
				return 0;
			}

			return it->second.Set(L, lifetime, object, index);
		}

		void AddProperty(STDString const& prop, typename PropertyAccessors::Getter* getter, typename PropertyAccessors::Setter* setter)
		{
			Properties.insert(std::make_pair(prop, PropertyAccessors{ getter, setter }));
		}

		std::unordered_map<STDString, PropertyAccessors> Properties;
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
		virtual char const* GetTypeName() const = 0;
		virtual bool GetProperty(lua_State* L, char const* prop) = 0;
		virtual bool SetProperty(lua_State* L, char const* prop, int index) = 0;
		virtual int Next(lua_State* L, char const* key) = 0;
	};

	template <class T>
	class ObjectProxyImpl : public ObjectProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<T>, "ObjectProxyImpl template parameter should not be a pointer type!");

		static char const* const TypeName;

		ObjectProxyImpl(LifetimeHolder const& lifetime, T * obj)
			: object_(obj), lifetime_(lifetime)
		{}
		
		~ObjectProxyImpl() override
		{}

		T* Get() const
		{
			return object_;
		}

		char const* GetTypeName() const override
		{
			return TypeName;
		}

		bool GetProperty(lua_State* L, char const* prop) override
		{
			auto const& map = StaticLuaPropertyMap<T>::PropertyMap;
			auto fetched = map.GetProperty(L, lifetime_, object_, prop);
			if (!fetched) {
				luaL_error(L, "Object of type '%s' has no property named '%s'", TypeName, prop);
				return false;
			}

			return true;
		}

		bool SetProperty(lua_State* L, char const* prop, int index) override
		{
			auto const& map = StaticLuaPropertyMap<T>::PropertyMap;
			auto ok = map.SetProperty(L, lifetime_, object_, prop, index);
			if (!ok) {
				luaL_error(L, "Object of type '%s' has no property named '%s'", TypeName, prop);
				return false;
			}

			return true;
		}

		int Next(lua_State* L, char const* key) override
		{
			auto const& map = StaticLuaPropertyMap<T>::PropertyMap;
			if (key == nullptr) {
				if (!map.Properties.empty()) {
					StackCheck _(L, 2);
					auto it = map.Properties.begin();
					push(L, it->first);
					if (!it->second.Get(L, lifetime_, object_)) {
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
						if (!it->second.Get(L, lifetime_, object_)) {
							push(L, nullptr);
						}

						return 2;
					}
				}
			}

			return 0;
		}

	private:
		T* object_;
		LifetimeHolder lifetime_;
	};

	class ObjectProxy : private Userdata<ObjectProxy>, public Indexable, public NewIndexable, 
		public Iterable, public Pushable, public GarbageCollected
	{
	public:
		static char const * const MetatableName;

		template <class T>
		inline static ObjectProxyImpl<T>* Make(lua_State* L, T* object, LifetimeHolder const& lifetime)
		{
			return MakeImpl<ObjectProxyImpl<T>, T*>(L, lifetime, object);
		}

		inline ObjectProxyImplBase* GetImpl()
		{
			return reinterpret_cast<ObjectProxyImplBase*>(impl_);
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

			auto type = GetImpl()->GetTypeName();
			if (type == ObjectProxyImpl<T>::TypeName) {
				return static_cast<ObjectProxyImpl<T>*>(GetImpl())->Get();
			} else {
				return nullptr;
			}
		}

	private:
		LifetimeReference lifetime_;
		uint8_t impl_[32];

		ObjectProxy(LifetimeHolder const& lifetime)
			: lifetime_(lifetime)
		{}

		~ObjectProxy()
		{
			GetImpl()->~ObjectProxyImplBase();
		}

		template <class TImpl, class... Args>
		static TImpl* MakeImpl(lua_State* L, LifetimeHolder const& lifetime, Args... args)
		{
			static_assert(sizeof(TImpl) <= sizeof(impl_), "ObjectProxy implementation object too large!");
			auto self = New(L, lifetime);
			return new (self->impl_) TImpl(lifetime, args...);
		}

	protected:
		friend Userdata<ObjectProxy>;

		int Index(lua_State* L)
		{
			StackCheck _(L, 1);
			auto impl = GetImpl();
			if (!lifetime_.IsAlive()) {
				luaL_error(L, "Attempted to read dead object of type '%s'", impl->GetTypeName());
				push(L, nullptr);
				return 1;
			}

			auto prop = luaL_checkstring(L, 2);
			if (!impl->GetProperty(L, prop)) {
				push(L, nullptr);
			}

			return 1;
		}

		int NewIndex(lua_State* L)
		{
			StackCheck _(L, 0);
			auto impl = GetImpl();
			if (!lifetime_.IsAlive()) {
				luaL_error(L, "Attempted to write dead object of type '%s'", impl->GetTypeName());
				return 0;
			}

			auto prop = luaL_checkstring(L, 2);
			impl->SetProperty(L, prop, 3);
			return 0;
		}

		int Next(lua_State* L)
		{
			auto impl = GetImpl();
			if (!lifetime_.IsAlive()) {
				luaL_error(L, "Attempted to iterate dead object of type '%s'", impl->GetTypeName());
				return 0;
			}

			if (lua_type(L, 2) == LUA_TNIL) {
				return impl->Next(L, nullptr);
			} else {
				auto key = checked_get<char const*>(L, 2);
				return impl->Next(L, key);
			}
		}

		int GC(lua_State* L)
		{
			this->~ObjectProxy();
			return 0;
		}
	};

	template <class T>
	inline void push_proxy(lua_State* L, LifetimeHolder const& lifetime, T const& v)
	{
		if constexpr (std::is_pointer_v<T> 
			&& (std::is_base_of_v<HasObjectProxy, std::remove_pointer_t<T>>
				|| HasObjectProxyTag<std::remove_pointer_t<T>>::HasProxy)) {
			if (v) {
				ObjectProxy::Make<std::remove_pointer_t<T>>(L, v, lifetime);
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
		auto type = proxy->GetImpl()->GetTypeName();
		if (type == ObjectProxyImpl<T>::TypeName) {
			auto obj = proxy->Get<T>();
			if (obj == nullptr) {
				luaL_error(L, "Argument %d: got object of type '%s' whose lifetime has expired", index, type);
				return nullptr;
			} else {
				return obj;
			}
		} else {
			luaL_error(L, "Argument %d: expected an object of type '%s', got '%s'", index, ObjectProxyImpl<T>::TypeName, type);
			return nullptr;
		}
	}
}
