#pragma once

#include <Lua/LuaHelpers.h>
#include <Lua/Shared/LuaLifetime.h>
#include <Lua/Shared/LuaPropertyMapHelpers.h>

namespace bg3se
{
	BY_VAL_ARRAY(uint8_t);
	BY_VAL_ARRAY(int16_t);
	BY_VAL_ARRAY(uint16_t);
	BY_VAL_ARRAY(int32_t);
	BY_VAL_ARRAY(uint32_t);
	BY_VAL_ARRAY(int64_t);
	BY_VAL_ARRAY(uint64_t);
	BY_VAL_ARRAY(float);
	BY_VAL_ARRAY(double);
	BY_VAL_ARRAY(bool);
	BY_VAL_ARRAY(ComponentHandle);
	BY_VAL_ARRAY(EntityHandle);
	BY_VAL_ARRAY(EntityWorldHandle);
	BY_VAL_ARRAY(FixedString);
	BY_VAL_ARRAY(STDString);
	BY_VAL_ARRAY(STDWString);
	BY_VAL_ARRAY(Path);
	BY_VAL_ARRAY(UUID);
	BY_VAL_ARRAY(NetId);
	BY_VAL_ARRAY(UserId);
	BY_VAL_ARRAY(glm::ivec2);
	BY_VAL_ARRAY(glm::vec2);
	BY_VAL_ARRAY(glm::vec3);
	BY_VAL_ARRAY(glm::vec4);
	BY_VAL_ARRAY(glm::mat3);
}

namespace bg3se::lua
{
	LifetimeHolder GetCurrentLifetime();

	class ArrayProxyImplBase
	{
	public:
		inline virtual ~ArrayProxyImplBase() {};
		virtual char const* GetTypeName() const = 0;
		virtual void* GetRaw() = 0;
		virtual bool GetElement(lua_State* L, unsigned arrayIndex) = 0;
		virtual bool SetElement(lua_State* L, unsigned arrayIndex, int luaIndex) = 0;
		virtual int Next(lua_State* L, int key) = 0;
		virtual unsigned Length() = 0;
	};


	template <class T>
	class ArrayProxyByRefImpl : public ArrayProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<T>, "ArrayProxyByRefImpl template parameter should not be a pointer type!");

		ArrayProxyByRefImpl(LifetimeHolder const& lifetime, Array<T> * obj)
			: object_(obj), lifetime_(lifetime)
		{}
		
		~ArrayProxyByRefImpl() override
		{}

		T* Get() const
		{
			return object_;
		}

		void* GetRaw() override
		{
			return object_;
		}

		char const* GetTypeName() const override
		{
			return TypeInfo<T>::TypeName;
		}

		bool GetElement(lua_State* L, unsigned arrayIndex) override
		{
			if (arrayIndex > 0 && arrayIndex <= (int)object_->Size()) {
				MakeObjectRef(L, lifetime_, &(*object_)[arrayIndex - 1]);
				return true;
			} else {
				return false;
			}
		}

		bool SetElement(lua_State* L, unsigned arrayIndex, int luaIndex) override
		{
			// Appending/swapping elements to by-ref arrays not supported for now
			return false;
		}

		unsigned Length() override
		{
			return object_->Size();
		}

		int Next(lua_State* L, int key) override
		{
			if (key >= 0 && key < (int)object_->Size()) {
				push(L, ++key);
				MakeObjectRef(L, lifetime_, &(*object_)[key - 1]);
				return 2;
			} else {
				return 0;
			}
		}

	private:
		Array<T>* object_;
		LifetimeHolder lifetime_;
	};


	template <class T>
	class ArrayProxyByValImpl : public ArrayProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<T>, "ArrayProxyByValImpl template parameter should not be a pointer type!");

		ArrayProxyByValImpl(LifetimeHolder const& lifetime, Array<T> * obj)
			: object_(obj), lifetime_(lifetime)
		{}
		
		~ArrayProxyByValImpl() override
		{}

		T* Get() const
		{
			return object_;
		}

		void* GetRaw() override
		{
			return object_;
		}

		char const* GetTypeName() const override
		{
			return TypeInfo<T>::TypeName;
		}

		bool GetElement(lua_State* L, unsigned arrayIndex) override
		{
			if (arrayIndex > 0 && arrayIndex <= object_->Size()) {
				LuaWrite(L, (*object_)[arrayIndex - 1]);
				return true;
			} else {
				return false;
			}
		}

		bool SetElement(lua_State* L, unsigned arrayIndex, int luaIndex) override
		{
			if (arrayIndex > 0 && arrayIndex <= object_->Size()) {
				lua_pushvalue(L, luaIndex);
				LuaRead(L, (*object_)[arrayIndex - 1]);
				lua_pop(L, 1);
				return true;
			} else if (arrayIndex == object_->Size() + 1) {
				T val;
				lua_pushvalue(L, luaIndex);
				LuaRead(L, val);
				lua_pop(L, 1);

				object_->Add(val);
				return true;
			} else {
				return false;
			}
		}

		unsigned Length() override
		{
			return object_->Size();
		}

		int Next(lua_State* L, int key) override
		{
			if (key >= 0 && key < (int)object_->Size()) {
				push(L, ++key);
				LuaWrite(L, (*object_)[key - 1]);
				return 2;
			} else {
				return 0;
			}
		}

	private:
		Array<T>* object_;
		LifetimeHolder lifetime_;
	};


	template <class T>
	class ObjectSetProxyByRefImpl : public ArrayProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<T>, "ObjectSetProxyByRefImpl template parameter should not be a pointer type!");

		ObjectSetProxyByRefImpl(LifetimeHolder const& lifetime, ObjectSet<T> * obj)
			: object_(obj), lifetime_(lifetime)
		{}
		
		~ObjectSetProxyByRefImpl() override
		{}

		T* Get() const
		{
			return object_;
		}

		void* GetRaw() override
		{
			return object_;
		}

		char const* GetTypeName() const override
		{
			return TypeInfo<T>::TypeName;
		}

		bool GetElement(lua_State* L, unsigned arrayIndex) override
		{
			if (arrayIndex > 0 && arrayIndex <= object_->Size) {
				MakeObjectRef(L, lifetime_, &(*object_)[arrayIndex - 1]);
				return true;
			} else {
				return false;
			}
		}

		bool SetElement(lua_State* L, unsigned arrayIndex, int luaIndex) override
		{
			// Appending/swapping elements to by-ref arrays not supported for now
			return false;
		}

		unsigned Length() override
		{
			return object_->Size;
		}

		int Next(lua_State* L, int key) override
		{
			if (key >= 0 && key < (int)object_->Size) {
				push(L, ++key);
				MakeObjectRef(L, lifetime_, &(*object_)[key - 1]);
				return 2;
			} else {
				return 0;
			}
		}

	private:
		ObjectSet<T>* object_;
		LifetimeHolder lifetime_;
	};


	template <class T>
	class ObjectSetProxyByValImpl : public ArrayProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<T>, "ObjectSetProxyByValImpl template parameter should not be a pointer type!");

		ObjectSetProxyByValImpl(LifetimeHolder const& lifetime, ObjectSet<T> * obj)
			: object_(obj), lifetime_(lifetime)
		{}
		
		~ObjectSetProxyByValImpl() override
		{}

		T* Get() const
		{
			return object_;
		}

		void* GetRaw() override
		{
			return object_;
		}

		char const* GetTypeName() const override
		{
			return TypeInfo<T>::TypeName;
		}

		bool GetElement(lua_State* L, unsigned arrayIndex) override
		{
			if (arrayIndex > 0 && arrayIndex <= object_->Size) {
				LuaWrite(L, (*object_)[arrayIndex - 1]);
				return true;
			} else {
				return false;
			}
		}

		bool SetElement(lua_State* L, unsigned arrayIndex, int luaIndex) override
		{
			if (arrayIndex > 0 && arrayIndex <= object_->Size) {
				lua_pushvalue(L, luaIndex);
				LuaRead(L, (*object_)[arrayIndex - 1]);
				lua_pop(L, 1);
				return true;
			} else if (arrayIndex == object_->Size + 1) {
				T val;
				lua_pushvalue(L, luaIndex);
				LuaRead(L, val);
				lua_pop(L, 1);

				object_->Add(val);
				return true;
			} else {
				return false;
			}
		}

		unsigned Length() override
		{
			return object_->Size;
		}

		int Next(lua_State* L, int key) override
		{
			if (key >= 0 && key < (int)object_->Size) {
				push(L, ++key);
				LuaWrite(L, (*object_)[key - 1]);
				return 2;
			} else {
				return 0;
			}
		}

	private:
		ObjectSet<T>* object_;
		LifetimeHolder lifetime_;
	};


	template <class T, int Size>
	class StdArrayProxyByRefImpl : public ArrayProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<T>, "StdArrayProxyByRefImpl template parameter should not be a pointer type!");

		StdArrayProxyByRefImpl(LifetimeHolder const& lifetime, std::array<T, Size> * obj)
			: object_(obj), lifetime_(lifetime)
		{}
		
		~StdArrayProxyByRefImpl() override
		{}

		T* Get() const
		{
			return object_;
		}

		void* GetRaw() override
		{
			return object_;
		}

		char const* GetTypeName() const override
		{
			return TypeInfo<T>::TypeName;
		}

		bool GetElement(lua_State* L, unsigned arrayIndex) override
		{
			if (arrayIndex > 0 && arrayIndex <= Size) {
				MakeObjectRef(L, lifetime_, &(*object_)[arrayIndex - 1]);
				return true;
			} else {
				return false;
			}
		}

		bool SetElement(lua_State* L, unsigned arrayIndex, int luaIndex) override
		{
			// Appending/swapping elements to by-ref arrays not supported for now
			return false;
		}

		unsigned Length() override
		{
			return Size;
		}

		int Next(lua_State* L, int key) override
		{
			if (key >= 0 && key < Size) {
				push(L, ++key);
				MakeObjectRef(L, lifetime_, &(*object_)[key - 1]);
				return 2;
			} else {
				return 0;
			}
		}

	private:
		std::array<T, Size>* object_;
		LifetimeHolder lifetime_;
	};


	template <class T, int Size>
	class StdArrayProxyByValImpl : public ArrayProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<T>, "StdArrayProxyByValImpl template parameter should not be a pointer type!");

		StdArrayProxyByValImpl(LifetimeHolder const& lifetime, std::array<T, Size> * obj)
			: object_(obj), lifetime_(lifetime)
		{}
		
		~StdArrayProxyByValImpl() override
		{}

		T* Get() const
		{
			return object_;
		}

		void* GetRaw() override
		{
			return object_;
		}

		char const* GetTypeName() const override
		{
			return TypeInfo<T>::TypeName;
		}

		bool GetElement(lua_State* L, unsigned arrayIndex) override
		{
			if (arrayIndex > 0 && arrayIndex <= Size) {
				LuaWrite(L, (*object_)[arrayIndex - 1]);
				return true;
			} else {
				return false;
			}
		}

		bool SetElement(lua_State* L, unsigned arrayIndex, int luaIndex) override
		{
			if (arrayIndex > 0 && arrayIndex <= Size) {
				lua_pushvalue(L, luaIndex);
				LuaRead(L, (*object_)[arrayIndex - 1]);
				lua_pop(L, 1);
				return true;
			} else {
				return false;
			}
		}

		unsigned Length() override
		{
			return Size;
		}

		int Next(lua_State* L, int key) override
		{
			if (key >= 0 && key < Size) {
				push(L, ++key);
				LuaWrite(L, (*object_)[key - 1]);
				return 2;
			} else {
				return 0;
			}
		}

	private:
		std::array<T, Size>* object_;
		LifetimeHolder lifetime_;
	};


	class ArrayProxy : private Userdata<ArrayProxy>, public Indexable, public NewIndexable,
		public Lengthable, public Iterable, public Stringifiable, public Pushable, public GarbageCollected
	{
	public:
		static char const * const MetatableName;

		template <class T>
		inline static ArrayProxyByRefImpl<T>* MakeByRef(lua_State* L, Array<T>* object, LifetimeHolder const& lifetime)
		{
			static_assert(sizeof(ArrayProxyByRefImpl<T>) <= sizeof(impl_), "ArrayProxyByRefImpl implementation object too large!");
			auto self = New(L, lifetime);
			return new (self->impl_) ArrayProxyByRefImpl<T>(lifetime, object);
		}

		template <class T>
		inline static ObjectSetProxyByRefImpl<T>* MakeByRef(lua_State* L, ObjectSet<T>* object, LifetimeHolder const& lifetime)
		{
			static_assert(sizeof(ObjectSetProxyByRefImpl<T>) <= sizeof(impl_), "ObjectSetProxyByRefImpl implementation object too large!");
			auto self = New(L, lifetime);
			return new (self->impl_) ObjectSetProxyByRefImpl<T>(lifetime, object);
		}

		template <class T, int Size>
		inline static StdArrayProxyByRefImpl<T, Size>* MakeByRef(lua_State* L, std::array<T, Size>* object, LifetimeHolder const& lifetime)
		{
			static_assert(sizeof(StdArrayProxyByRefImpl<T, Size>) <= sizeof(impl_), "StdArrayProxyByRefImpl implementation object too large!");
			auto self = New(L, lifetime);
			return new (self->impl_) StdArrayProxyByRefImpl<T, Size>(lifetime, object);
		}

		template <class T>
		inline static ArrayProxyByValImpl<T>* MakeByVal(lua_State* L, Array<T>* object, LifetimeHolder const& lifetime)
		{
			static_assert(sizeof(ArrayProxyByValImpl<T>) <= sizeof(impl_), "ArrayProxyByValImpl implementation object too large!");
			auto self = New(L, lifetime);
			return new (self->impl_) ArrayProxyByValImpl<T>(lifetime, object);
		}

		template <class T>
		inline static ObjectSetProxyByValImpl<T>* MakeByVal(lua_State* L, ObjectSet<T>* object, LifetimeHolder const& lifetime)
		{
			static_assert(sizeof(ObjectSetProxyByValImpl<T>) <= sizeof(impl_), "ObjectSetProxyByValImpl implementation object too large!");
			auto self = New(L, lifetime);
			return new (self->impl_) ObjectSetProxyByValImpl<T>(lifetime, object);
		}

		template <class T, int Size>
		inline static StdArrayProxyByValImpl<T, Size>* MakeByVal(lua_State* L, std::array<T, Size>* object, LifetimeHolder const& lifetime)
		{
			static_assert(sizeof(StdArrayProxyByValImpl<T, Size>) <= sizeof(impl_), "StdArrayProxyByValImpl implementation object too large!");
			auto self = New(L, lifetime);
			return new (self->impl_) StdArrayProxyByValImpl<T, Size>(lifetime, object);
		}

		inline ArrayProxyImplBase* GetImpl()
		{
			return reinterpret_cast<ArrayProxyImplBase*>(impl_);
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

			if (strcmp(TypeInfo<T>::TypeName, GetImpl()->GetTypeName()) == 0) {
				return reinterpret_cast<T*>(GetImpl()->GetRaw());
			} else {
				return nullptr;
			}
		}

	private:
		LifetimeReference lifetime_;
		uint8_t impl_[40];

		ArrayProxy(LifetimeHolder const& lifetime)
			: lifetime_(lifetime)
		{}

		~ArrayProxy()
		{
			GetImpl()->~ArrayProxyImplBase();
		}

	protected:
		friend Userdata<ArrayProxy>;

		int Index(lua_State* L);
		int NewIndex(lua_State* L);
		int Length(lua_State* L);
		int Next(lua_State* L);
		int ToString(lua_State* L);
		int GC(lua_State* L);
	};

	template <class T>
	inline void push_array_ref_proxy(lua_State* L, LifetimeHolder const& lifetime, T* v)
	{
		ArrayProxy::MakeByRef<T>(L, v, lifetime);
	}

	template <class T>
	inline T* checked_get_array_proxy(lua_State* L, int index)
	{
		auto proxy = Userdata<ArrayProxy>::CheckUserData(L, index);
		auto const& typeName = TypeInfo<T>::TypeName;
		if (strcmp(proxy->GetImpl()->GetTypeName(), typeName) == 0) {
			auto obj = proxy->Get<T>();
			if (obj == nullptr) {
				luaL_error(L, "Argument %d: got Array<%s> whose lifetime has expired", index, typeName);
				return nullptr;
			} else {
				return obj;
			}
		} else {
			luaL_error(L, "Argument %d: expected Array<%s>, got Array<%s>", index, typeName, proxy->GetImpl()->GetTypeName());
			return nullptr;
		}
	}
}
