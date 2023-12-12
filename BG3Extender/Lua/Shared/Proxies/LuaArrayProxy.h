#pragma once

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
BY_VAL(Version);
BY_VAL(glm::ivec2);
BY_VAL(glm::vec2);
BY_VAL(glm::vec3);
BY_VAL(glm::vec4);
BY_VAL(glm::quat);
BY_VAL(glm::mat3);
BY_VAL(glm::mat3x4);
BY_VAL(glm::mat4x3);
BY_VAL(glm::mat4);
BY_VAL(TypeInformationRef);

template <class T> 
struct ByVal<OverrideableProperty<T>> { static constexpr bool Value = IsByVal<T>; };

END_SE()

BEGIN_NS(lua)

class ArrayProxyImplBase
{
public:
	ArrayProxyImplBase();
	void Register();
	int GetRegistryIndex() const;
	virtual unsigned GetContainerClass() = 0;
	virtual TypeInformation const& GetContainerType() const = 0;
	virtual TypeInformation const& GetElementType() const = 0;
	virtual bool GetElement(lua_State* L, CppObjectMetadata& self, unsigned arrayIndex) = 0;
	virtual bool SetElement(lua_State* L, CppObjectMetadata& self, unsigned arrayIndex, int luaIndex) = 0;
	virtual int Next(lua_State* L, CppObjectMetadata& self, int key) = 0;
	virtual unsigned Length(CppObjectMetadata& self) = 0;
	virtual bool Unserialize(lua_State* L, CppObjectMetadata& self, int index) = 0;
	virtual void Serialize(lua_State* L, CppObjectMetadata& self) = 0;

private:
	int registryIndex_{ -1 };
};


template <class TContainer, class T, unsigned TContainerClassId>
class DynamicArrayProxyImpl : public ArrayProxyImplBase
{
public:
	static_assert(!std::is_pointer_v<TContainer>, "DynamicArrayProxyImpl template parameter should not be a pointer type!");

	using ContainerType = TContainer;
	static constexpr unsigned ContainerClassId = TContainerClassId;

	unsigned GetContainerClass() override
	{
		return ContainerClassId;
	}

	TypeInformation const& GetContainerType() const override
	{
		return GetTypeInfo<TContainer>();
	}

	TypeInformation const& GetElementType() const override
	{
		return GetTypeInfo<T>();
	}

	bool GetElement(lua_State* L, CppObjectMetadata& self, unsigned arrayIndex) override
	{
		auto obj = reinterpret_cast<TContainer*>(self.Ptr);
		if (arrayIndex > 0 && arrayIndex <= (int)obj->size()) {
			push(L, &(*obj)[arrayIndex - 1], self.Lifetime);
			return true;
		} else {
			return false;
		}
	}

	bool SetElement(lua_State* L, CppObjectMetadata& self, unsigned arrayIndex, int luaIndex) override
	{
		auto obj = reinterpret_cast<TContainer*>(self.Ptr);
		if constexpr (std::is_default_constructible_v<T>) {
			auto size = obj->size();
			if (arrayIndex > 0 && arrayIndex <= size) {
				if (lua_type(L, luaIndex) == LUA_TNIL) {
					obj->remove_at(arrayIndex - 1);
				} else {
					(*obj)[arrayIndex - 1] = get<T>(L, luaIndex);
				}
				return true;
			} else if (arrayIndex == size + 1) {
				obj->push_back(get<T>(L, luaIndex));
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	}

	unsigned Length(CppObjectMetadata& self) override
	{
		auto obj = reinterpret_cast<TContainer*>(self.Ptr);
		return obj->size();
	}

	bool Unserialize(lua_State* L, CppObjectMetadata& self, int index) override
	{
		auto obj = reinterpret_cast<TContainer*>(self.Ptr);
		if constexpr (std::is_default_constructible_v<T>) {
			lua::Unserialize(L, index, obj);
			return true;
		} else {
			return false;
		}
	}

	void Serialize(lua_State* L, CppObjectMetadata& self) override
	{
		auto obj = reinterpret_cast<TContainer*>(self.Ptr);
		lua::Serialize(L, obj);
	}

	int Next(lua_State* L, CppObjectMetadata& self, int key) override
	{
		auto obj = reinterpret_cast<TContainer*>(self.Ptr);
		if (key >= 0 && key < (int)obj->size()) {
			push(L, ++key);
			push(L, &(*obj)[key - 1], self.Lifetime);
			return 2;
		} else {
			return 0;
		}
	}
};


template <class TContainer, class T, unsigned TContainerClassId>
class ConstSizeArrayProxyImpl : public ArrayProxyImplBase
{
public:
	static_assert(!std::is_pointer_v<TContainer>, "ConstSizeArrayProxyImpl template parameter should not be a pointer type!");

	using ContainerType = TContainer;
	static constexpr unsigned ContainerClassId = TContainerClassId;

	unsigned GetContainerClass() override
	{
		return ContainerClassId;
	}

	TypeInformation const& GetContainerType() const override
	{
		return GetTypeInfo<TContainer>();
	}

	TypeInformation const& GetElementType() const override
	{
		return GetTypeInfo<bool>();
	}

	bool GetElement(lua_State* L, CppObjectMetadata& self, unsigned arrayIndex) override
	{
		auto obj = reinterpret_cast<TContainer*>(self.Ptr);
		if (arrayIndex > 0 && arrayIndex <= obj->size()) {
			push(L, &(*obj)[arrayIndex - 1], self.Lifetime);
			return true;
		} else {
			return false;
		}
	}

	bool SetElement(lua_State* L, CppObjectMetadata& self, unsigned arrayIndex, int luaIndex) override
	{
		auto obj = reinterpret_cast<TContainer*>(self.Ptr);
		if constexpr (std::is_default_constructible_v<T>) {
			if (arrayIndex > 0 && arrayIndex <= obj->size()) {
				(*obj)[arrayIndex - 1] = get<T>(L, luaIndex);
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	}

	unsigned Length(CppObjectMetadata& self) override
	{
		auto obj = reinterpret_cast<TContainer*>(self.Ptr);
		return (unsigned)obj->size();
	}

	bool Unserialize(lua_State* L, CppObjectMetadata& self, int index) override
	{
		auto obj = reinterpret_cast<TContainer*>(self.Ptr);
		if constexpr (std::is_default_constructible_v<T>) {
			lua::Unserialize(L, index, obj);
			return true;
		} else {
			return false;
		}
	}

	void Serialize(lua_State* L, CppObjectMetadata& self) override
	{
		auto obj = reinterpret_cast<TContainer*>(self.Ptr);
		lua::Serialize(L, obj);
	}

	int Next(lua_State* L, CppObjectMetadata& self, int key) override
	{
		auto obj = reinterpret_cast<TContainer*>(self.Ptr);
		if (key >= 0 && (decltype(obj->size()))key < obj->size()) {
			push(L, ++key);
			push(L, &(*obj)[key - 1], self.Lifetime);
			return 2;
		} else {
			return 0;
		}
	}
};

template <unsigned Words, unsigned TContainerClassId>
class ConstSizeArrayProxyImpl<BitArray<Words>, bool, TContainerClassId> : public ArrayProxyImplBase
{
public:
	static constexpr unsigned ContainerClassId = TContainerClassId;

	using ContainerType = BitArray<Words>;

	unsigned GetContainerClass() override
	{
		return ContainerClassId;
	}

	TypeInformation const& GetContainerType() const override
	{
		return GetTypeInfo<ContainerType>();
	}

	TypeInformation const& GetElementType() const override
	{
		return GetTypeInfo<bool>();
	}

	bool GetElement(lua_State* L, CppObjectMetadata& self, unsigned arrayIndex) override
	{
		auto obj = reinterpret_cast<ContainerType*>(self.Ptr);
		if (arrayIndex > 0 && arrayIndex <= obj->size()) {
			bool isSet = obj->IsSet(arrayIndex);
			push(L, &isSet, self.Lifetime);
			return true;
		} else {
			return false;
		}
	}

	bool SetElement(lua_State* L, CppObjectMetadata& self, unsigned arrayIndex, int luaIndex) override
	{
		auto obj = reinterpret_cast<ContainerType*>(self.Ptr);
		if (arrayIndex > 0 && arrayIndex <= obj->size()) {
			if (get<bool>(L, luaIndex))
			{
				obj->Set(arrayIndex);
			}
			else
			{
				obj->Clear(arrayIndex);
			}
			return true;
		} else {
			return false;
		}
	}

	unsigned Length(CppObjectMetadata& self) override
	{
		auto obj = reinterpret_cast<ContainerType*>(self.Ptr);
		return (unsigned)obj->size();
	}

	bool Unserialize(lua_State* L, CppObjectMetadata& self, int index) override
	{
		auto obj = reinterpret_cast<ContainerType*>(self.Ptr);
		lua::Unserialize(L, index, obj);
        return true;
	}

	void Serialize(lua_State* L, CppObjectMetadata& self) override
	{
		auto obj = reinterpret_cast<ContainerType*>(self.Ptr);
		lua::Serialize(L, obj);
	}

	int Next(lua_State* L, CppObjectMetadata& self, int key) override
	{
		auto obj = reinterpret_cast<ContainerType*>(self.Ptr);
		if (key >= 0 && (decltype(obj->size()))key < obj->size()) {
			push(L, ++key);
			bool isSet = obj->IsSet(key - 1);
			push(L, &isSet, self.Lifetime);
			return 2;
		} else {
			return 0;
		}
	}
};

class ArrayProxyMetatable : public LightCppObjectMetatable<ArrayProxyMetatable>, public Indexable, public NewIndexable,
	public Lengthable, public Iterable, public Stringifiable, public EqualityComparable
{
public:
	static constexpr MetatableTag MetaTag = MetatableTag::ArrayProxy;
	static constexpr bool HasLifetime = true;

	template <class TImpl>
	static ArrayProxyImplBase* GetImplementation()
	{
		static ArrayProxyImplBase* impl = new TImpl();
		return impl;
	}

	inline static void MakeImpl(lua_State* L, void* object, LifetimeHandle const& lifetime, ArrayProxyImplBase* impl)
	{
		lua_push_cppobject(L, MetatableTag::ArrayProxy, impl->GetRegistryIndex(), object, lifetime);
	}

	template <class T>
	static inline void Make(lua_State* L, StaticArray<T>* object, const LifetimeHandle& lifetime)
	{
		MakeImpl(L, object, lifetime, GetImplementation<ConstSizeArrayProxyImpl<StaticArray<T>, T, 2>>());
	}

	template <class T>
	inline static void Make(lua_State* L, Array<T>* object, LifetimeHandle const& lifetime)
	{
		MakeImpl(L, object, lifetime, GetImplementation<DynamicArrayProxyImpl<Array<T>, T, 2>>());
	}

	template <unsigned Words>
	inline static void Make(
		lua_State* L, BitArray<Words>* object, const LifetimeHandle& lifetime)
	{
		MakeImpl(L, object, lifetime, GetImplementation<ConstSizeArrayProxyImpl<BitArray<Words>, bool, 5>>());
	}

	template <class T>
	inline static void Make(lua_State* L, ObjectSet<T>* object, LifetimeHandle const& lifetime)
	{
		MakeImpl(L, object, lifetime, GetImplementation<DynamicArrayProxyImpl<ObjectSet<T>, T, 3>>());
	}

	template <class T, int Size>
	inline static void Make(lua_State* L, std::array<T, Size>* object, LifetimeHandle const& lifetime)
	{
		MakeImpl(L, object, lifetime, GetImplementation<ConstSizeArrayProxyImpl<std::array<T, Size>, T, 1>>());
	}

	template <class T>
	inline static void Make(lua_State* L, std::vector<T>* object, LifetimeHandle const& lifetime)
	{
		MakeImpl(L, object, lifetime, GetImplementation<DynamicArrayProxyImpl<std::vector<T>, T, 4>>());
	}

	template <class T, size_t Extent>
	inline static void Make(lua_State* L, std::span<T, Extent>* object, LifetimeHandle const& lifetime)
	{
		MakeImpl(L, object, lifetime, GetImplementation<ConstSizeArrayProxyImpl<std::span<T, Extent>, T, 6>>());
	}

	template <class T>
	inline static typename T::ContainerType* Get(lua_State* L, int index)
	{
		auto ptr = GetRaw(L, index, GetImplementation<T>()->GetRegistryIndex());
		return reinterpret_cast<T::ContainerType*>(ptr);
	}

	inline static ArrayProxyImplBase* GetImpl(CppObjectMetadata const& meta)
	{
		assert(meta.MetatableTag == MetatableTag::ArrayProxy);
		return GetImpl(meta.PropertyMapTag);
	}

	static int Index(lua_State* L, CppObjectMetadata& self);
	static int NewIndex(lua_State* L, CppObjectMetadata& self);
	static int Length(lua_State* L, CppObjectMetadata& self);
	static int Next(lua_State* L, CppObjectMetadata& self);
	static int ToString(lua_State* L, CppObjectMetadata& self);
	static bool IsEqual(lua_State* L, CppObjectMetadata& self, CppObjectMetadata& other);
	static char const* GetTypeName(lua_State* L, CppObjectMetadata& self);

private:
	static void* GetRaw(lua_State* L, int index, int propertyMapIndex);
	static ArrayProxyImplBase* GetImpl(int propertyMapIndex);
};

END_NS()
