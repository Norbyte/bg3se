#pragma once

#include <GameDefinitions/Enumerations.h>

BEGIN_SE()

struct TypeInformation;

struct StaticTypeInformation
{
	using InitializerProc = TypeInformation * ();

	TypeInformation* Type{ nullptr };
	InitializerProc* Initializer{ nullptr };

	void DeferredInitialize();
};

class TypeInformationRef
{
public:
	inline TypeInformationRef() {}
	inline TypeInformationRef(StaticTypeInformation& ref) : ref_(&ref) {}

	inline operator bool() const
	{
		return ref_ && ref_->Type;
	}

	inline bool operator !() const
	{
		return !ref_ || !ref_->Type;
	}

	inline bool IsBound() const
	{
		return ref_ != nullptr;
	}

	inline StaticTypeInformation* GetStatic() const
	{
		return ref_;
	}

	TypeInformation const& Get() const;

private:
	StaticTypeInformation* ref_{ nullptr };
};

struct TypeInformation
{
	FixedString TypeName;
	FixedString NativeName;
	LuaTypeId Kind{ LuaTypeId::Unknown };
	TypeInformationRef KeyType;
	TypeInformationRef ElementType;
	TypeInformationRef ParentType;
	Map<FixedString, TypeInformationRef> Members;
	Map<FixedString, TypeInformation> Methods;
	bool HasWildcardProperties{ false };
	Map<FixedString, uint64_t> EnumValues;
	Vector<TypeInformationRef> ReturnValues;
	Vector<TypeInformationRef> Params;
	bool VarargParams{ false };
	bool VarargsReturn{ false };
	bool IsBuiltin{ false };
	bool DeferredInitializationDone{ false };
	bool Validated{ false };
	FixedString ModuleRole;

	void DeferredInitialize();
	void Validate();
};

template <class T>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<T>)
{
	return nullptr;
}

template <class T>
TypeInformation* MakeDeferredOptionalType();

template <class T>
TypeInformation* MakeDeferredArrayType();

template <class TKey, class TValue>
TypeInformation* MakeDeferredMapType();

template <class T>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<Array<T>>)
{
	return &MakeDeferredArrayType<T>;
}

template <class T>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<Vector<T>>)
{
	return &MakeDeferredArrayType<T>;
}

template <class T>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<std::span<T>>)
{
	return &MakeDeferredArrayType<T>;
}

template <class T, class Allocator, bool StoreSize>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<ObjectSet<T, Allocator, StoreSize>>)
{
	return &MakeDeferredArrayType<T>;
}

template <class T, class Allocator, bool StoreSize>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<Set<T, Allocator, StoreSize>>)
{
	return &MakeDeferredArrayType<T>;
}

template <class T, class Allocator>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<PrimitiveSmallSet<T, Allocator>>)
{
	return &MakeDeferredArrayType<T>;
}

template <class T, int Size>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<std::array<T, Size>>)
{
	return &MakeDeferredArrayType<T>;
}

template <class TKey, class TValue>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<Map<TKey, TValue>>)
{
	return &MakeDeferredMapType<TKey, TValue>;
}

template <class TKey, class TValue>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<RefMap<TKey, TValue>>)
{
	return &MakeDeferredMapType<TKey, TValue>;
}

/*template <class T, class... Trait>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<lua::Traits<T, Trait...>>)
{
	return &MakeDeferredOptionalType<T>;
}*/

template <class T>
inline StaticTypeInformation::InitializerProc* MakeDeferredTypeInitializer(Overload<std::optional<T>>)
{
	return &MakeDeferredOptionalType<T>;
}

template <class T>
StaticTypeInformation& GetStaticTypeInfoInternal(Overload<T>)
{
	static StaticTypeInformation info{ nullptr, MakeDeferredTypeInitializer(Overload<T>{}) };
	return info;
}

template <class T, class Allocator, bool StoreSize>
StaticTypeInformation& GetStaticTypeInfoInternal(Overload<ObjectSet<T, Allocator, StoreSize>>)
{
	return GetStaticTypeInfoInternal(Overload<Set<T, Allocator, StoreSize>>{});
}

template <class T>
StaticTypeInformation& GetStaticTypeInfo(Overload<T>)
{
	if constexpr (std::is_pointer_v<T>) {
		return GetStaticTypeInfo(Overload<std::remove_pointer_t<T>>{});
	} else if constexpr (std::is_reference_v<T>) {
		return GetStaticTypeInfo(Overload<std::remove_reference_t<T>>{});
	} else if constexpr (std::is_const_v<T> || std::is_volatile_v<T>) {
		return GetStaticTypeInfo(Overload<std::remove_cv_t<T>>{});
	} else {
		return GetStaticTypeInfoInternal(Overload<T>{});
	}
}

/*template <class T>
StaticTypeInformation& GetStaticTypeInfo(Overload<OverrideableProperty<T>>)
{
	return GetStaticTypeInfo(Overload<T>{});
}*/

/*template <class T>
StaticTypeInformation& GetStaticTypeInfo(Overload<RefReturn<T>>)
{
	return GetStaticTypeInfo(Overload<T>{});
}

template <class T>
StaticTypeInformation& GetStaticTypeInfo(Overload<ByValReturn<T>>)
{
	return GetStaticTypeInfo(Overload<T>{});
}

template <class T>
StaticTypeInformation& GetStaticTypeInfo(Overload<ProxyParam<T>>)
{
	return GetStaticTypeInfo(Overload<T>{});
}*/

template <class T>
TypeInformation const& GetTypeInfo()
{
	auto type = GetStaticTypeInfo(Overload<T>{});
	assert(type.Type != nullptr);
	return *type.Type;
}

template <class T>
TypeInformationRef GetTypeInfoRef()
{
	return TypeInformationRef(GetStaticTypeInfo(Overload<T>{}));
}

class TypeInformationRepository
{
public:
	static TypeInformationRepository& GetInstance();

	TypeInformationRepository();
	void Initialize();
	TypeInformation& RegisterType(FixedString const& typeName);
	void RegisterType(FixedString const& typeName, TypeInformation* typeInfo);
	TypeInformation const& GetType(FixedString const& typeName);
	TypeInformation const& GetUndefinedType();
	Map<FixedString, TypeInformation*> const& GetAllTypes();

	template <class T>
	void DeferredRegister(Overload<Array<T>>)
	{
		RegisterArrayType("Array", GetTypeInfoRef<T>());
	}

private:
	Map<FixedString, TypeInformation*> types_;
	TypeInformation undefinedType_;
	bool initialized_;

	void RegisterArrayType(char const* arrayClass, TypeInformationRef const& elementType);
	TypeInformation& RegisterType(char const* name, LuaTypeId typeId);

	template <class T>
	TypeInformation& RegisterStaticType(char const* name, LuaTypeId typeId)
	{
		auto& ty = RegisterType(name, typeId);
		GetStaticTypeInfo(Overload<T>{}).Type = &ty;
		return ty;
	}
};

template <class T>
TypeInformation* MakeDeferredOptionalType()
{
	auto& subType = GetStaticTypeInfo(Overload<T>{});
	subType.DeferredInitialize();
	if (subType.Type) {
		auto ty = GameAlloc<TypeInformation>();
		ty->TypeName = FixedString(STDString("Optional<") + subType.Type->TypeName.GetString() + ">");
		ty->Kind = LuaTypeId::Nullable;
		ty->ParentType = subType;
		return ty;
	} else {
		return nullptr;
	}
}

template <class T>
TypeInformation* MakeDeferredArrayType()
{
	auto ty = GameAlloc<TypeInformation>();
	ty->Kind = LuaTypeId::Array;
	ty->ElementType = GetStaticTypeInfo(Overload<T>{});
	return ty;
}

template <class TKey, class TValue>
TypeInformation* MakeDeferredMapType()
{
	auto ty = GameAlloc<TypeInformation>();
	ty->Kind = LuaTypeId::Map;
	ty->KeyType = GetStaticTypeInfo(Overload<TKey>{});
	ty->ElementType = GetStaticTypeInfo(Overload<TValue>{});
	return ty;
}

template <class R, class T, class... Args>
R GetFunctionReturnType(R (T::*)(Args...)) {}

template <class T>
inline void AddFunctionReturnType(TypeInformation& ty, Overload<T>)
{
	ty.ReturnValues.push_back(GetTypeInfoRef<T>());
}

template <>
inline void AddFunctionReturnType(TypeInformation& ty, Overload<void>)
{
	// No return value, nothing to do
}

/*template <>
inline void AddFunctionReturnType(TypeInformation& ty, Overload<UserReturn>)
{
	// User defined number of return types and values
	ty.VarargsReturn = true;
}*/

template <class T>
inline void AddFunctionParamType(TypeInformation& ty, Overload<T>)
{
	ty.Params.push_back(GetTypeInfoRef<T>());
}

// Pointer to member function without Lua state parameter
template <class R, class T, class... Args>
void ConstructFunctionSignature(TypeInformation& sig, R (T::*)(Args...))
{
	sig.Kind = LuaTypeId::Function;
	AddFunctionReturnType(sig, Overload<R>{});
	(AddFunctionParamType(sig, Overload<Args>{}), ...);
}

// Pointer to member function with Lua state parameter
/*template <class R, class T, class... Args>
void ConstructFunctionSignature(TypeInformation& sig, R (T::*)(lua_State* L, Args...))
{
	sig.Kind = LuaTypeId::Function;
	AddFunctionReturnType(sig, Overload<R>{});
	(AddFunctionParamType(sig, Overload<Args>{}), ...);
}*/

// Unbound function without Lua state parameter
template <class R, class... Args>
void ConstructFunctionSignature(TypeInformation& sig, R (*)(Args...))
{
	sig.Kind = LuaTypeId::Function;
	AddFunctionReturnType(sig, Overload<R>{});
	(AddFunctionParamType(sig, Overload<Args>{}), ...);
}

// Unbound function with Lua state parameter
/*template <class R, class... Args>
void ConstructFunctionSignature(TypeInformation& sig, R (*)(lua_State* L, Args...))
{
	sig.Kind = LuaTypeId::Function;
	AddFunctionReturnType(sig, Overload<R>{});
	(AddFunctionParamType(sig, Overload<Args>{}), ...);
}*/

END_SE()
