#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>
#include <optional>

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Misc.h>
#include <ExtensionHelpers.h>

struct lua_State;

namespace bg3se
{
	enum class PropertyType
	{
		kBool,
		kUInt8,
		kInt16,
		kUInt16,
		kInt32,
		kUInt32,
		kInt64,
		kUInt64,
		kFloat,
		kFixedString,
		kDynamicFixedString,
		kFixedStringGuid,
		kStringPtr,
		kStdString,
		kStdWString,
		kTranslatedString,
		kObjectHandle,
		kVector3
	};

	enum PropertyFlag
	{
		kPropRead = 1,
		kPropWrite = 2
	};

	template <class T>
	constexpr PropertyType GetPropertyType()
	{
		if constexpr (std::is_same<T, bool>::value) {
			return PropertyType::kBool;
		} else if constexpr (std::is_same<T, uint8_t>::value) {
			return PropertyType::kUInt8;
		} else if constexpr (std::is_same<T, int16_t>::value) {
			return PropertyType::kInt16;
		} else if constexpr (std::is_same<T, uint16_t>::value) {
			return PropertyType::kUInt16;
		} else if constexpr (std::is_same<T, int32_t>::value) {
			return PropertyType::kInt32;
		} else if constexpr (std::is_same<T, uint32_t>::value) {
			return PropertyType::kUInt32;
		} else if constexpr (std::is_same<T, int64_t>::value) {
			return PropertyType::kInt64;
		} else if constexpr (std::is_same<T, uint64_t>::value) {
			return PropertyType::kUInt64;
		} else if constexpr (std::is_same<T, float>::value) {
			return PropertyType::kFloat;
		} else if constexpr (std::is_same<T, FixedString>::value) {
			return PropertyType::kFixedString;
		} else if constexpr (std::is_same<T, char *>::value) {
			return PropertyType::kStringPtr;
		} else if constexpr (std::is_same<T, STDString>::value) {
			return PropertyType::kStdString;
		} else if constexpr (std::is_same<T, STDWString>::value) {
			return PropertyType::kStdWString;
		} else if constexpr (std::is_same<T, TranslatedString>::value) {
			return PropertyType::kTranslatedString;
		} else if constexpr (std::is_same<T, ObjectHandle>::value) {
			return PropertyType::kObjectHandle;
		} else if constexpr (std::is_same<T, Vector3>::value) {
			return PropertyType::kVector3;
		} else if constexpr (std::is_same<T, NetId>::value) {
			return PropertyType::kUInt32;
		} else {
			static_assert(false, "Unsupported property type");
		}
	}

	struct PropertyMapBase
	{
		struct PropertyInfo
		{
			PropertyType Type;
			std::uintptr_t Offset;
			uint32_t Flags;

			std::function<bool (void *, int64_t)> SetInt;
			std::function<bool (void *, float)> SetFloat;
			std::function<bool (void *, char const *)> SetString;
			std::function<bool (void *, ObjectHandle)> SetHandle;
			std::function<bool (void *, Vector3)> SetVector3;
			std::function<std::optional<int64_t> (void *)> GetInt;
			std::function<std::optional<float> (void *)> GetFloat;
			std::function<std::optional<char const *> (void *)> GetString;
			std::function<std::optional<ObjectHandle> (void *)> GetHandle;
			std::function<std::optional<Vector3> (void *)> GetVector3;
		};

		struct FlagInfo
		{
			FixedString Property;
			uint64_t Mask;
			uint32_t Flags;

			std::function<bool (void *, bool)> Set;
			std::function<std::optional<bool> (void *)> Get;
		};

		STDString Name;
		std::unordered_map<FixedString, PropertyInfo> Properties;
		std::unordered_map<FixedString, FlagInfo> Flags;

		PropertyMapBase * Parent{ nullptr };

		virtual void * toParent(void * obj) const = 0;

		PropertyInfo const* findProperty(FixedString const& name) const;
		FlagInfo const* findFlag(FixedString const& name) const;

		std::optional<int64_t> getInt(void* obj, FixedString const& name, bool raw, bool throwError) const;
		bool setInt(void* obj, FixedString const& name, int64_t value, bool raw, bool throwError) const;
		std::optional<float> getFloat(void* obj, FixedString const& name, bool raw, bool throwError) const;
		bool setFloat(void* obj, FixedString const& name, float value, bool raw, bool throwError) const;
		std::optional<char const*> getString(void* obj, FixedString const& name, bool raw, bool throwError) const;
		bool setString(void* obj, FixedString const& name, char const* value, bool raw, bool throwError) const;
		std::optional<ObjectHandle> getHandle(void* obj, FixedString const& name, bool raw, bool throwError) const;
		bool setHandle(void* obj, FixedString const& name, ObjectHandle value, bool raw, bool throwError) const;
		std::optional<Vector3> getVector3(void* obj, FixedString const& name, bool raw, bool throwError) const;
		bool setVector3(void* obj, FixedString const& name, Vector3 const& value, bool raw, bool throwError) const;
		std::optional<bool> getFlag(void* obj, FixedString const& name, bool raw, bool throwError) const;
		bool setFlag(void* obj, FixedString const& name, bool value, bool raw, bool throwError) const;
	};

	template <class T>
	struct PropertyMapInterface : public PropertyMapBase
	{
		using ObjType = T;
	};

	template <class T, class TBase>
	struct PropertyMap : public PropertyMapInterface<T>
	{
		using ObjectType = T;
		using BaseType = typename std::conditional<!std::is_same<TBase, void>::value, TBase, T>::type;

		virtual void * toParent(void * obj) const
		{
			auto typedObj = reinterpret_cast<T *>(obj);
			return static_cast<TBase *>(obj);
		}
	};

	inline PropertyMapBase::PropertyInfo& AddPropertyInternal(PropertyMapBase& map, char const* name, PropertyMapBase::PropertyInfo const& info)
	{
		FixedString fs(name);
#if !defined(NDEBUG)
		if (map.Properties.find(fs) != map.Properties.end()
			|| map.Flags.find(fs) != map.Flags.end()) {
			throw std::runtime_error("Tried to add duplicate property!");
		}
#endif

		auto it = map.Properties.insert(std::make_pair(fs, info));
		return it.first->second;
	}

	template <class TValue>
	typename PropertyMapBase::PropertyInfo & AddProperty(PropertyMapBase & map, char const* name, std::uintptr_t offset)
	{
		PropertyMapBase::PropertyInfo info;
		info.Type = GetPropertyType<TValue>();
		info.Offset = offset;
		info.Flags = kPropRead | kPropWrite;
		return AddPropertyInternal(map, name, info);
	}

	template <class TValue>
	typename PropertyMapBase::PropertyInfo & AddPropertyRO(PropertyMapBase & map, char const* name, std::uintptr_t offset)
	{
		PropertyMapBase::PropertyInfo info;
		info.Type = GetPropertyType<TValue>();
		info.Offset = offset;
		info.Flags = kPropRead;
		return AddPropertyInternal(map, name, info);
	}

	template <class TEnum>
	typename PropertyMapBase::PropertyInfo & AddPropertyEnum(PropertyMapBase & map, char const* name, std::uintptr_t offset, bool writeable)
	{
		using TValue = std::underlying_type<TEnum>::type;
		PropertyMapBase::PropertyInfo info;
		info.Type = GetPropertyType<TValue>();
		info.Offset = offset;
		info.Flags = kPropRead | (writeable ? kPropWrite : 0);

		info.GetInt = [offset](void * obj) -> std::optional<int64_t> {
			auto ptr = reinterpret_cast<TEnum *>(reinterpret_cast<std::uintptr_t>(obj) + offset);
			return (int64_t)*ptr;
		};

		info.GetString = [offset](void * obj) -> std::optional<char const *> {
			auto ptr = reinterpret_cast<TEnum *>(reinterpret_cast<std::uintptr_t>(obj) + offset);
			auto val = EnumInfo<TEnum>::Find(*ptr);
			if (val) {
				return val.Str;
			} else {
				return {};
			}
		};

		info.SetInt = [offset](void * obj, int64_t val) -> bool {
			auto label = EnumInfo<TEnum>::Find((TEnum)val);
			if (!label) {
				return false;
			}

			auto ptr = reinterpret_cast<TEnum *>(reinterpret_cast<std::uintptr_t>(obj) + offset);
			*ptr = (TEnum)val;
			return true;
		};

		info.SetString = [offset](void * obj, char const * str) -> bool {
			auto enumVal = EnumInfo<TEnum>::Find(str);
			if (!enumVal) {
				return false;
			}

			auto ptr = reinterpret_cast<TEnum *>(reinterpret_cast<std::uintptr_t>(obj) + offset);
			*ptr = *enumVal;
			return true;
		};

		return AddPropertyInternal(map, name, info);
	}

	template <class TValue, class TEnum>
	void AddPropertyFlags(PropertyMapBase & map, char const* name,
		std::uintptr_t offset, bool canWrite)
	{
		using Enum = EnumInfo<TEnum>;

		auto fieldName = MakeFixedString(name);
		PropertyMapBase::PropertyInfo info;
		info.Type = GetPropertyType<TValue>();
		info.Offset = offset;
		info.Flags = 0;
		AddPropertyInternal(map, name, info);

		Enum::Values.Iterate([&map, canWrite, fieldName](auto const& k, auto v) {
			PropertyMapBase::FlagInfo flag;
			flag.Property = fieldName;
			flag.Flags = kPropRead | (canWrite ? kPropWrite : 0);
			flag.Mask = (int64_t)v;

#if !defined(NDEBUG)
			if (map.Properties.find(k) != map.Properties.end()
				|| map.Flags.find(k) != map.Flags.end()) {
				throw std::runtime_error("Tried to add duplicate property!");
			}
#endif

			map.Flags.insert(std::make_pair(k, flag));
		});
	}

	template <class TValue>
	typename void AddPropertyGuidString(PropertyMapBase & map, char const* name,
		std::uintptr_t offset, bool canWrite)
	{
		static_assert(std::is_same<TValue, FixedString>::value, "Only FixedString GUID values are supported");
		PropertyMapBase::PropertyInfo info;
		info.Type = PropertyType::kFixedStringGuid;
		info.Offset = offset;
		info.Flags = kPropRead | (canWrite ? kPropWrite : 0);
		AddPropertyInternal(map, name, info);
	}

	template <class TValue>
	typename void AddPropertyDynamicFixedString(PropertyMapBase& map, char const* name,
		std::uintptr_t offset, bool canWrite)
	{
		static_assert(std::is_same<TValue, FixedString>::value, "Only FixedString values are supported");
		PropertyMapBase::PropertyInfo info;
		info.Type = PropertyType::kDynamicFixedString;
		info.Offset = offset;
		info.Flags = kPropRead | (canWrite ? kPropWrite : 0);
		AddPropertyInternal(map, name, info);
	}

	enum class OsiPropertyMapType
	{
		Integer,
		Integer64,
		Real,
		String,
		GuidStringHandle,
		Vector3
	};


	bool OsirisPropertyMapSetRaw(PropertyMapBase const & propertyMap, void * obj,
		OsiArgumentDesc const & args, uint32_t firstArg, OsiPropertyMapType type, bool throwError = true);
	bool OsirisPropertyMapGetRaw(PropertyMapBase const & propertyMap, void * obj,
		OsiArgumentDesc & args, uint32_t firstArg, OsiPropertyMapType type, bool throwError = true);

	template <class T>
	bool OsirisPropertyMapGet(PropertyMapInterface<T> const & propertyMap, T * obj,
		OsiArgumentDesc & args, uint32_t firstArg, OsiPropertyMapType type, bool throwError = true)
	{
		return OsirisPropertyMapGetRaw(propertyMap, obj, args, firstArg, type, throwError);
	}

	template <class T>
	bool OsirisPropertyMapSet(PropertyMapInterface<T> const & propertyMap, T * obj,
		OsiArgumentDesc const & args, uint32_t firstArg, OsiPropertyMapType type, bool throwError = true)
	{
		return OsirisPropertyMapSetRaw(propertyMap, obj, args, firstArg, type, throwError);
	}

	bool LuaPropertyMapGet(lua_State * L, PropertyMapBase const & propertyMap, void * obj,
		char const * propertyName, bool throwError);
	bool LuaPropertyMapGet(lua_State* L, PropertyMapBase const& propertyMap, void* obj,
		FixedString const& propertyName, bool throwError);
	bool LuaPropertyMapSet(lua_State * L, int index, PropertyMapBase const & propertyMap,
		void * obj, char const * propertyName, bool throwError);
}
