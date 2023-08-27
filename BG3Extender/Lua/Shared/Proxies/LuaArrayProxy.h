#pragma once

#include <Lua/LuaHelpers.h>
#include <Lua/LuaSerializers.h>
#include <Lua/Shared/LuaLifetime.h>

BEGIN_SE()

BY_VAL(uint8_t);
BY_VAL(int16_t);
BY_VAL(uint16_t);
BY_VAL(int32_t);
BY_VAL(uint32_t);
BY_VAL(int64_t);
BY_VAL(uint64_t);
BY_VAL(float);
BY_VAL(double);
BY_VAL(bool);
BY_VAL(ComponentHandle);
BY_VAL(EntityHandle);
BY_VAL(ecs::EntityRef);
BY_VAL(FixedString);
BY_VAL(STDString);
BY_VAL(STDWString);
BY_VAL(Guid);
BY_VAL(Path);
BY_VAL(NetId);
BY_VAL(UserId);
BY_VAL(glm::ivec2);
BY_VAL(glm::vec2);
BY_VAL(glm::vec3);
BY_VAL(glm::vec4);
BY_VAL(glm::mat3);
BY_VAL(glm::mat3x4);
BY_VAL(glm::mat4x3);
BY_VAL(glm::mat4);

END_SE()

namespace bg3se::lua
{
	LifetimeHandle GetCurrentLifetime();

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

		ArrayProxyByRefImpl(LifetimeHandle const& lifetime, Array<T> * obj)
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
			return GetTypeInfo<T>().TypeName.GetString();
		}

		bool GetElement(lua_State* L, unsigned arrayIndex) override
		{
			if (arrayIndex > 0 && arrayIndex <= (int)object_->Size()) {
				MakeObjectRef(L, &(*object_)[arrayIndex - 1], lifetime_);
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
				MakeObjectRef(L, &(*object_)[key - 1], lifetime_);
				return 2;
			} else {
				return 0;
			}
		}

	private:
		Array<T>* object_;
		LifetimeHandle lifetime_;
	};


	template <class T>
	class ArrayProxyByValImpl : public ArrayProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<T>, "ArrayProxyByValImpl template parameter should not be a pointer type!");

		ArrayProxyByValImpl(LifetimeHandle const& lifetime, Array<T> * obj)
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
			return GetTypeInfo<T>().TypeName.GetString();
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
		LifetimeHandle lifetime_;
	};


	template <class T>
	class ObjectSetProxyByRefImpl : public ArrayProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<T>, "ObjectSetProxyByRefImpl template parameter should not be a pointer type!");

		ObjectSetProxyByRefImpl(LifetimeHandle const& lifetime, ObjectSet<T> * obj)
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
			return GetTypeInfo<T>().TypeName.GetString();
		}

		bool GetElement(lua_State* L, unsigned arrayIndex) override
		{
			if (arrayIndex > 0 && arrayIndex <= object_->Size) {
				MakeObjectRef(L, &(*object_)[arrayIndex - 1], lifetime_);
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
				MakeObjectRef(L, &(*object_)[key - 1], lifetime_);
				return 2;
			} else {
				return 0;
			}
		}

	private:
		ObjectSet<T>* object_;
		LifetimeHandle lifetime_;
	};


	template <class T>
	class ObjectSetProxyByValImpl : public ArrayProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<T>, "ObjectSetProxyByValImpl template parameter should not be a pointer type!");

		ObjectSetProxyByValImpl(LifetimeHandle const& lifetime, ObjectSet<T> * obj)
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
			return GetTypeInfo<T>().TypeName.GetString();
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
		LifetimeHandle lifetime_;
	};


	template <class T, int Size>
	class StdArrayProxyByRefImpl : public ArrayProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<T>, "StdArrayProxyByRefImpl template parameter should not be a pointer type!");

		StdArrayProxyByRefImpl(LifetimeHandle const& lifetime, std::array<T, Size> * obj)
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
			return GetTypeInfo<T>().TypeName.GetString();
		}

		bool GetElement(lua_State* L, unsigned arrayIndex) override
		{
			if (arrayIndex > 0 && arrayIndex <= Size) {
				MakeObjectRef(L, &(*object_)[arrayIndex - 1], lifetime_);
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
				MakeObjectRef(L, &(*object_)[key - 1], lifetime_);
				return 2;
			} else {
				return 0;
			}
		}

	private:
		std::array<T, Size>* object_;
		LifetimeHandle lifetime_;
	};


	template <class T, int Size>
	class StdArrayProxyByValImpl : public ArrayProxyImplBase
	{
	public:
		static_assert(!std::is_pointer_v<T>, "StdArrayProxyByValImpl template parameter should not be a pointer type!");

		StdArrayProxyByValImpl(LifetimeHandle const& lifetime, std::array<T, Size> * obj)
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
			return GetTypeInfo<T>().TypeName.GetString();
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
		LifetimeHandle lifetime_;
	};


	class ArrayProxy : private Userdata<ArrayProxy>, public Indexable, public NewIndexable,
		public Lengthable, public Iterable, public Stringifiable, public Pushable, public GarbageCollected
	{
	public:
		static char const * const MetatableName;

		template <class TImpl, class... Args>
		inline static TImpl* MakeImplByRef(lua_State* L, LifetimeHandle const& lifetime, Args... args)
		{
			auto self = NewWithExtraData(L, sizeof(TImpl), lifetime);
			return new (self->GetImpl()) TImpl(lifetime, args...);
		}

		template <class T>
		inline static ArrayProxyByRefImpl<T>* MakeByRef(lua_State* L, Array<T>* object, LifetimeHandle const& lifetime)
		{
			return MakeImplByRef<ArrayProxyByRefImpl<T>>(L, lifetime, object);
		}

		template <class T>
		inline static ObjectSetProxyByRefImpl<T>* MakeByRef(lua_State* L, ObjectSet<T>* object, LifetimeHandle const& lifetime)
		{
			return MakeImplByRef<ObjectSetProxyByRefImpl<T>>(L, lifetime, object);
		}

		template <class T, int Size>
		inline static StdArrayProxyByRefImpl<T, Size>* MakeByRef(lua_State* L, std::array<T, Size>* object, LifetimeHandle const& lifetime)
		{
			return MakeImplByRef<StdArrayProxyByRefImpl<T, Size>>(L, lifetime, object);
		}

		template <class T>
		inline static ArrayProxyByValImpl<T>* MakeByVal(lua_State* L, Array<T>* object, LifetimeHandle const& lifetime)
		{
			return MakeImplByRef<ArrayProxyByValImpl<T>>(L, lifetime, object);
		}

		template <class T>
		inline static ObjectSetProxyByValImpl<T>* MakeByVal(lua_State* L, ObjectSet<T>* object, LifetimeHandle const& lifetime)
		{
			return MakeImplByRef<ObjectSetProxyByValImpl<T>>(L, lifetime, object);
		}

		template <class T, int Size>
		inline static StdArrayProxyByValImpl<T, Size>* MakeByVal(lua_State* L, std::array<T, Size>* object, LifetimeHandle const& lifetime)
		{
			return MakeImplByRef<StdArrayProxyByValImpl<T, Size>>(L, lifetime, object);
		}

		inline ArrayProxyImplBase* GetImpl()
		{
			return reinterpret_cast<ArrayProxyImplBase*>(this + 1);
		}

		inline bool IsAlive(lua_State* L) const
		{
			return lifetime_.IsAlive(L);
		}

		template <class T>
		T* Get(lua_State* L)
		{
			if (!lifetime_.IsAlive(L)) {
				return nullptr;
			}

			if (strcmp(GetTypeInfo<T>().TypeName.GetString(), GetImpl()->GetTypeName()) == 0) {
				return reinterpret_cast<T*>(GetImpl()->GetRaw(L));
			} else {
				return nullptr;
			}
		}

	private:
		LifetimeHandle lifetime_;

		ArrayProxy(LifetimeHandle const& lifetime)
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
	struct IsArrayLike { static constexpr bool Value = false; };

	template <class T>
	struct IsArrayLike<Array<T>> { static constexpr bool Value = true; using TElement = T; };

	template <class T>
	struct IsArrayLike<StaticArray<T>> { static constexpr bool Value = true; using TElement = T; };

	template <class T>
	struct IsArrayLike<Vector<T>> { static constexpr bool Value = true; using TElement = T; };

	template <class T, class Allocator, bool StoreSize>
	struct IsArrayLike<Set<T, Allocator, StoreSize>> { static constexpr bool Value = true; using TElement = T; };

	template <class T, class Allocator, bool StoreSize>
	struct IsArrayLike<ObjectSet<T, Allocator, StoreSize>> { static constexpr bool Value = true; using TElement = T; };

	template <class T, size_t Size>
	struct IsArrayLike<std::array<T, Size>> { static constexpr bool Value = true; using TElement = T; };

	template <class T>
	inline void push_array_ref_proxy(lua_State* L, LifetimeHandle const& lifetime, T* v)
	{
		ArrayProxy::MakeByRef<T>(L, v, lifetime);
	}

	template <class T>
	inline T* checked_get_array_proxy(lua_State* L, int index)
	{
		auto proxy = Userdata<ArrayProxy>::CheckUserData(L, index);
		auto const& typeName = GetTypeInfo<T>().TypeName.GetString();
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
