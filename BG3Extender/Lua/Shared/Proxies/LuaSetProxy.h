#pragma once

namespace bg3se::lua
{
	LifetimeHandle GetCurrentLifetime();

	class SetProxyImplBase
	{
	public:
		inline virtual ~SetProxyImplBase() {};
		virtual char const* GetTypeName() const = 0;
		virtual TypeInformation const& GetContainerType() const = 0;
		virtual TypeInformation const& GetElementType() const = 0;
		virtual void* GetRaw() = 0;
		virtual bool GetElementAt(lua_State* L, unsigned int arrayIndex) = 0;
		virtual bool HasElement(lua_State* L, int luaIndex) = 0;
		virtual bool AddElement(lua_State* L, int luaIndex) = 0;
		virtual bool RemoveElement(lua_State* L, int luaIndex) = 0;
		virtual int Next(lua_State* L, int index) = 0;
		virtual unsigned Length() = 0;
		virtual bool Unserialize(lua_State* L, int index) = 0;
		virtual void Serialize(lua_State* L) = 0;
	};

	
	template <class T>
	class MultiHashSetProxyImpl : public SetProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<T>, "MultiHashSetProxyImpl template parameter should not be a pointer type!");

		using ElementType = T;
		using ContainerType = MultiHashSet<T>;

		MultiHashSetProxyImpl(LifetimeHandle const& lifetime, MultiHashSet<T> * obj)
			: object_(obj), lifetime_(lifetime)
		{}
		
		~MultiHashSetProxyImpl() override
		{}

		TypeInformation const& GetContainerType() const override
		{
			return GetTypeInfo<ContainerType>();
		}

		TypeInformation const& GetElementType() const override
		{
			return GetTypeInfo<T>();
		}

		MultiHashSet<T>* Get() const
		{
			return object_;
		}

		void* GetRaw() override
		{
			return object_;
		}

		char const* GetTypeName() const override
		{
			return GetTypeInfo<T>().TypeName.GetString();
		}

		bool GetElementAt(lua_State* L, unsigned int arrayIndex) override
		{
			if (arrayIndex > 0 && arrayIndex <= object_->size()) {
				push(L, &object_->Keys[arrayIndex - 1], lifetime_);
				return true;
			} else {
				return false;
			}
		}

		bool HasElement(lua_State* L, int luaIndex) override
		{
			auto element = get<T>(L, luaIndex);
			return object_->FindIndex(element) != -1;
		}

		bool AddElement(lua_State* L, int luaIndex) override
		{
			auto element = get<T>(L, luaIndex);
			object_->Add(element);
			return true;
		}

		bool RemoveElement(lua_State* L, int luaIndex) override
		{
			auto element = get<T>(L, luaIndex);
			auto index = object_->FindIndex(element);
			// FIXME - add support for remove() in hashsets!
			// object_->RemoveIndex(index);
			return true;
		}

		unsigned Length() override
		{
			return object_->Keys.Size();
		}

		int Next(lua_State* L, int key) override
		{
			if (key >= -1 && key < (int)object_->Keys.Size() - 1) {
				push(L, ++key);
				push(L, &object_->Keys[key], lifetime_);
				return 2;
			} else {
				return 0;
			}
		}

		bool Unserialize(lua_State* L, int index) override
		{
			if constexpr (std::is_default_constructible_v<T>) {
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
		MultiHashSet<T>* object_;
		LifetimeHandle lifetime_;
	};


	class SetProxy : private Userdata<SetProxy>, public Indexable, public NewIndexable,
		public Lengthable, public Iterable, public Stringifiable
	{
	public:
		static char const * const MetatableName;

		template <class T>
		inline static MultiHashSetProxyImpl<T>* Make(lua_State* L, MultiHashSet<T>* object, LifetimeHandle const& lifetime)
		{
			auto self = NewWithExtraData(L, sizeof(MultiHashSetProxyImpl<T>), lifetime);
			return new (self->GetImpl()) MultiHashSetProxyImpl<T>(lifetime, object);
		}

		inline SetProxyImplBase* GetImpl()
		{
			return reinterpret_cast<SetProxyImplBase*>(this + 1);
		}

		inline bool IsAlive(lua_State* L) const
		{
			return lifetime_.IsAlive(L);
		}

		template <class T>
		T* Get(lua_State* L)
		{
			auto impl = GetImpl();
			if (!lifetime_.IsAlive(L)) {
				luaL_error(L, "Attempted to access dead Set<%s>", impl->GetTypeName());
				return nullptr;
			}
			
			if (strcmp(impl->GetTypeName(), GetTypeInfo<typename T::ElementType>().TypeName.GetString()) == 0) {
				return reinterpret_cast<T*>(impl->GetRaw());
			} else {
				luaL_error(L, "Attempted to access Set<%s>, got Set<%s>", GetTypeInfo<typename T::ElementType>().TypeName.GetString(), impl->GetTypeName());
				return nullptr;
			}
		}

		template <class T>
		static T* CheckedGet(lua_State* L, int index)
		{
			auto self = CheckUserData(L, index);
			return self->Get<T>(L);
		}

	private:
		LifetimeHandle lifetime_;

		SetProxy(LifetimeHandle const& lifetime)
			: lifetime_(lifetime)
		{}

		~SetProxy()
		{
			GetImpl()->~SetProxyImplBase();
		}

	protected:
		friend Userdata<SetProxy>;

		int Index(lua_State* L);
		int NewIndex(lua_State* L);
		int Length(lua_State* L);
		int Next(lua_State* L);
		int ToString(lua_State* L);
	};
}
