#pragma once

#include <Lua/LuaHelpers.h>
#include <Lua/Shared/LuaLifetime.h>
#include <unordered_map>

BEGIN_NS(lua)

class GenericPropertyMap
{
public:
	using TFallbackGetter = PropertyOperationResult (lua_State* L, LifetimeHandle const& lifetime, void* object, FixedString const& prop);
	using TFallbackSetter = PropertyOperationResult (lua_State* L, LifetimeHandle const& lifetime, void* object, FixedString const& prop, int index);

	struct RawPropertyAccessors
	{
		using Getter = PropertyOperationResult (lua_State* L, LifetimeHandle const& lifetime, void* object, std::size_t offset, uint64_t flag);
		using Setter = PropertyOperationResult (lua_State* L, LifetimeHandle const& lifetime, void* object, int index, std::size_t offset, uint64_t flag);

		FixedString Name;
		Getter* Get;
		Setter* Set;
		std::size_t Offset;
		uint64_t Flag;
	};

	struct RawPropertyValidators
	{
		using Validator = bool (void* object, std::size_t offset, uint64_t flag);

		FixedString Name;
		Validator* Validate;
		std::size_t Offset;
		uint64_t Flag;
	};

	enum class ValidationState
	{
		Unknown,
		Valid,
		Invalid
	};

	void Init(int registryIndex);
	void Finish();
	bool HasProperty(FixedString const& prop) const;
	PropertyOperationResult GetRawProperty(lua_State* L, LifetimeHandle const& lifetime, void* object, FixedString const& prop) const;
	PropertyOperationResult SetRawProperty(lua_State* L, LifetimeHandle const& lifetime, void* object, FixedString const& prop, int index) const;
	void AddRawProperty(char const* prop, typename RawPropertyAccessors::Getter* getter,
		typename RawPropertyAccessors::Setter* setter, std::size_t offset, uint64_t flag = 0);
	void AddRawValidator(char const* prop, typename RawPropertyValidators::Validator* validate, std::size_t offset, uint64_t flag = 0);
	void AddRawProperty(char const* prop, typename RawPropertyAccessors::Getter* getter,
		typename RawPropertyAccessors::Setter* setter, typename RawPropertyValidators::Validator* validate, std::size_t offset, uint64_t flag = 0);
	bool IsA(int typeRegistryIndex) const;
	bool ValidatePropertyMap(void* object);
	bool ValidateObject(void* object);

	FixedString Name;
	std::unordered_map<FixedString, RawPropertyAccessors> Properties;
	std::vector<RawPropertyValidators> Validators;
	std::vector<FixedString> Parents;
	std::vector<int> ParentRegistryIndices;
	TFallbackGetter* FallbackGetter{ nullptr };
	TFallbackSetter* FallbackSetter{ nullptr };
	bool IsInitializing{ false };
	bool Initialized{ false };
	ValidationState Validated{ ValidationState::Unknown };
	int RegistryIndex{ -1 };
};

inline PropertyOperationResult GenericSetNonWriteableProperty(lua_State* L, LifetimeHandle const& lifetime, void* obj, int index, std::size_t offset, uint64_t)
{
	return PropertyOperationResult::UnsupportedType;
}

inline PropertyOperationResult GenericSetReadOnlyProperty(lua_State* L, LifetimeHandle const& lifetime, void* obj, int index, std::size_t offset, uint64_t)
{
	return PropertyOperationResult::ReadOnly;
}

inline bool GenericValidateNoopProperty(void* obj, std::size_t offset, uint64_t)
{
	return true;
}

template <class T>
class LuaPropertyMap : public GenericPropertyMap
{
public:
	struct PropertyAccessors
	{
		using Getter = PropertyOperationResult (lua_State* L, LifetimeHandle const& lifetime, T* object, std::size_t offset, uint64_t flag);
		using Setter = PropertyOperationResult (lua_State* L, LifetimeHandle const& lifetime, T* object, int index, std::size_t offset, uint64_t flag);
		using Validator = bool (T* object, std::size_t offset, uint64_t flag);
		using FallbackGetter = PropertyOperationResult (lua_State* L, LifetimeHandle const& lifetime, T* object, FixedString const& prop);
		using FallbackSetter = PropertyOperationResult (lua_State* L, LifetimeHandle const& lifetime, T* object, FixedString const& prop, int index);
	};

	inline PropertyOperationResult GetProperty(lua_State* L, LifetimeHandle const& lifetime, T* object, FixedString const& prop) const
	{
#if defined(DEBUG_TRAP_GETTERS)
		__try {
			return GetRawProperty(L, lifetime, (void*)object, prop);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			ERR("Exception while reading property %s.%s", Name.GetString(), prop.GetString());
			return PropertyOperationResult::Unknown;
		}
#else
		return GetRawProperty(L, lifetime, (void*)object, prop);
#endif
	}

	inline PropertyOperationResult SetProperty(lua_State* L, LifetimeHandle const& lifetime, T* object, FixedString const& prop, int index) const
	{
#if defined(DEBUG_TRAP_GETTERS)
		__try {
			return SetRawProperty(L, lifetime, (void*)object, prop, index);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			ERR("Exception while writing property %s.%s", Name.GetString(), prop.GetString());
			return PropertyOperationResult::Unknown;
		}
#else
		return SetRawProperty(L, lifetime, (void*)object, prop, index);
#endif
	}

	inline PropertyOperationResult GetProperty(lua_State* L, LifetimeHandle const& lifetime, T* object, RawPropertyAccessors const& prop) const
	{
		auto getter = (typename PropertyAccessors::Getter*)prop.Get;

#if defined(DEBUG_TRAP_GETTERS)
		__try {
			return getter(L, lifetime, object, prop.Offset, prop.Flag);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			ERR("Exception while reading property %s.%s", Name.GetString(), prop.Name.GetString());
			return PropertyOperationResult::Unknown;
		}
#else
		return getter(L, lifetime, object, prop.Offset, prop.Flag);
#endif
	}

	inline PropertyOperationResult SetProperty(lua_State* L, LifetimeHandle const& lifetime, T* object, RawPropertyAccessors const& prop, int index) const
	{
		auto setter = (typename PropertyAccessors::Setter*)prop.Set;

#if defined(DEBUG_TRAP_GETTERS)
		__try {
			return setter(L, lifetime, object, index, prop.Offset, prop.Flag);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			ERR("Exception while writing property %s.%s", Name.GetString(), prop.Name.GetString());
			return PropertyOperationResult::Unknown;
		}
#else
		return setter(L, lifetime, object, index, prop.Offset, prop.Flag);
#endif
	}

	inline void AddProperty(char const* prop, typename PropertyAccessors::Getter* getter,
		typename PropertyAccessors::Setter* setter = nullptr, typename PropertyAccessors::Validator* validator = nullptr, 
		std::size_t offset = 0, uint64_t flag = 0)
	{
		if (setter == nullptr) {
			setter = (typename PropertyAccessors::Setter*)&GenericSetNonWriteableProperty;
		}
		
		if (validator == nullptr) {
			validator = (typename PropertyAccessors::Validator*)&GenericValidateNoopProperty;
		}

		AddRawProperty(prop, (RawPropertyAccessors::Getter*)getter, (RawPropertyAccessors::Setter*)setter, (RawPropertyValidators::Validator*)validator, offset, flag);
	}

	inline void SetFallback(PropertyAccessors::FallbackGetter* getter, PropertyAccessors::FallbackSetter* setter)
	{
		FallbackGetter = (TFallbackGetter*)getter;
		FallbackSetter = (TFallbackSetter*)setter;
	}
};

template <class T>
struct StaticLuaPropertyMap
{
	static_assert(!std::is_pointer_v<T>, "StaticLuaPropertyMap type should not be a pointer type!");
	static_assert(!IsByVal<T>, "StaticLuaPropertyMap type should not be a by-val type!");

	using ObjectType = T;
	using TPropertyMap = LuaPropertyMap<T>;

	static TPropertyMap PropertyMap;
};

END_NS()
