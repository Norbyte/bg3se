#include <stdafx.h>
#include <PropertyMap.h>
#include <Lua/LuaHelpers.h>

namespace bg3se
{
	PropertyMapBase::PropertyInfo const * PropertyMapBase::findProperty(FixedString const& name) const
	{
		PropertyMapBase const * propMap = this;
		do {
			auto prop = propMap->Properties.find(name);
			if (prop != propMap->Properties.end()) {
				return &prop->second;
			}

			propMap = propMap->Parent;
		} while (propMap != nullptr);
			
		return nullptr;
	}

	PropertyMapBase::FlagInfo const * PropertyMapBase::findFlag(FixedString const& name) const
	{
		PropertyMapBase const * propMap = this;
		do {
			auto prop = propMap->Flags.find(name);
			if (prop != propMap->Flags.end()) {
				return &prop->second;
			}

			propMap = propMap->Parent;
		} while (propMap != nullptr);

		return nullptr;
	}

	std::optional<int64_t> PropertyMapBase::getInt(void * obj, FixedString const& name, bool raw, bool throwError) const
	{
		auto prop = Properties.find(name);
		if (prop == Properties.end()) {
			if (Parent != nullptr) {
				return Parent->getInt(toParent(obj), name, raw, throwError);
			} else {
				if (throwError) {
					OsiError("Failed to get int property '" << name << "' of [" << Name << "]: Property does not exist!");
				}
				return {};
			}
		}

		if (!raw && prop->second.GetInt) {
			return prop->second.GetInt(obj);
		}

		if (!raw && !(prop->second.Flags & kPropRead)) {
			OsiError("Failed to get int property '" << name << "' of [" << Name << "]: Property not readable");
			return {};
		}

		auto ptr = reinterpret_cast<std::uintptr_t>(obj) + prop->second.Offset;
		switch (prop->second.Type) {
		case PropertyType::kBool: return (int64_t)*reinterpret_cast<bool *>(ptr);
		case PropertyType::kUInt8: return (int64_t)*reinterpret_cast<uint8_t *>(ptr);
		case PropertyType::kInt16: return (int64_t)*reinterpret_cast<int16_t *>(ptr);
		case PropertyType::kUInt16: return (int64_t)*reinterpret_cast<uint16_t *>(ptr);
		case PropertyType::kInt32: return (int64_t)*reinterpret_cast<int32_t *>(ptr);
		case PropertyType::kUInt32: return (int64_t)*reinterpret_cast<uint32_t *>(ptr);
		case PropertyType::kInt64: return (int64_t)*reinterpret_cast<int64_t *>(ptr);
		case PropertyType::kUInt64: return (int64_t)*reinterpret_cast<uint64_t *>(ptr);
		case PropertyType::kFloat: return (int64_t)*reinterpret_cast<float *>(ptr);
		default:
			OsiError("Failed to get property '" << name << "' of [" << Name << "]: Property is not an int");
			return {};
		}
	}

	std::optional<float> PropertyMapBase::getFloat(void * obj, FixedString const& name, bool raw, bool throwError) const
	{
		auto prop = Properties.find(name);
		if (prop == Properties.end()) {
			if (Parent != nullptr) {
				return Parent->getFloat(toParent(obj), name, raw, throwError);
			} else {
				if (throwError) {
					OsiError("Failed to get float property '" << name << "' of [" << Name << "]: Property does not exist");
				}
				return {};
			}
		}

		if (!raw && prop->second.GetFloat) {
			return prop->second.GetFloat(obj);
		}

		if (!raw && !(prop->second.Flags & kPropRead)) {
			OsiError("Failed to get float property '" << name << "' of [" << Name << "]: Property not readable");
			return {};
		}

		auto ptr = reinterpret_cast<std::uintptr_t>(obj) + prop->second.Offset;
		switch (prop->second.Type) {
		case PropertyType::kFloat: return *reinterpret_cast<float *>(ptr);
		default:
			OsiError("Failed to get property '" << name << "' of [" << Name << "]: Property is not a float");
			return {};
		}
	}

	bool PropertyMapBase::setInt(void * obj, FixedString const & name, int64_t value, bool raw, bool throwError) const
	{
		auto prop = Properties.find(name);
		if (prop == Properties.end()) {
			if (Parent != nullptr) {
				return Parent->setInt(toParent(obj), name, value, raw, throwError);
			} else {
				if (throwError) {
					OsiError("Failed to set int property '" << name << "' of [" << Name << "]: Property does not exist");
				}
				return false;
			}
		}

		if (!raw && prop->second.SetInt) {
			return prop->second.SetInt(obj, value);
		}

		if (!raw && !(prop->second.Flags & kPropWrite)) {
			OsiError("Failed to set int property '" << name << "' of [" << Name << "]: Property not writeable; contact Norbyte on Discord if you have a legitimate use case for doing this.");
			return false;
		}

		auto ptr = reinterpret_cast<std::uintptr_t>(obj) + prop->second.Offset;
		switch (prop->second.Type) {
		case PropertyType::kBool: *reinterpret_cast<bool *>(ptr) = (bool)value; break;
		case PropertyType::kUInt8: *reinterpret_cast<uint8_t *>(ptr) = (uint8_t)value; break;
		case PropertyType::kInt16: *reinterpret_cast<int16_t *>(ptr) = (int16_t)value; break;
		case PropertyType::kUInt16: *reinterpret_cast<uint16_t *>(ptr) = (uint16_t)value; break;
		case PropertyType::kInt32: *reinterpret_cast<int32_t *>(ptr) = (int32_t)value; break;
		case PropertyType::kUInt32: *reinterpret_cast<uint32_t *>(ptr) = (uint32_t)value; break;
		case PropertyType::kInt64: *reinterpret_cast<int64_t *>(ptr) = (int64_t)value; break;
		case PropertyType::kUInt64: *reinterpret_cast<uint64_t *>(ptr) = (uint64_t)value; break;
		case PropertyType::kFloat: *reinterpret_cast<float *>(ptr) = (float)value; break;
		default:
			OsiError("Failed to set property '" << name << "' of [" << Name << "]: Property is not an int");
			return false;
		}

		return true;
	}

	bool PropertyMapBase::setFloat(void * obj, FixedString const & name, float value, bool raw, bool throwError) const
	{
		auto prop = Properties.find(name);
		if (prop == Properties.end()) {
			if (Parent != nullptr) {
				return Parent->setFloat(toParent(obj), name, value, raw, throwError);
			} else {
				if (throwError) {
					OsiError("Failed to set float property '" << name << "' of [" << Name << "]: Property does not exist");
				}
				return false;
			}
		}

		if (!raw && prop->second.SetFloat) {
			return prop->second.SetFloat(obj, value);
		}

		if (!raw && !(prop->second.Flags & kPropWrite)) {
			OsiError("Failed to set float property '" << name << "' of [" << Name << "]: Property not writeable; contact Norbyte on Discord if you have a legitimate use case for doing this.");
			return false;
		}

		auto ptr = reinterpret_cast<std::uintptr_t>(obj) + prop->second.Offset;
		switch (prop->second.Type) {
		case PropertyType::kFloat: *reinterpret_cast<float *>(ptr) = value; break;
		default:
			OsiError("Failed to set property '" << name << "' of [" << Name << "]: Property is not a float");
			return false;
		}

		return true;
	}

	std::optional<char const *> PropertyMapBase::getString(void * obj, FixedString const & name, bool raw, bool throwError) const
	{
		auto prop = Properties.find(name);
		if (prop == Properties.end()) {
			if (Parent != nullptr) {
				return Parent->getString(toParent(obj), name, raw, throwError);
			} else {
				if (throwError) {
					OsiError("Failed to get string property '" << name << "' of [" << Name << "]: Property does not exist");
				}
				return {};
			}
		}

		if (!raw && prop->second.GetString) {
			return prop->second.GetString(obj);
		}

		if (!raw && !(prop->second.Flags & kPropRead)) {
			OsiError("Failed to get string property '" << name << "' of [" << Name << "]: Property not readable");
			return {};
		}

		auto ptr = reinterpret_cast<std::uintptr_t>(obj) + prop->second.Offset;
		switch (prop->second.Type) {
		case PropertyType::kFixedString:
		case PropertyType::kDynamicFixedString:
		case PropertyType::kFixedStringGuid:
		{
			throw std::runtime_error("FIXME!");
			/*auto p = reinterpret_cast<FixedString *>(ptr)->Str;
			if (p != nullptr) {
				return p;
			} else {
				return "";
			}*/
		}

		case PropertyType::kStringPtr:
		{
			auto p = *reinterpret_cast<char const **>(ptr);
			if (p != nullptr) {
				return p;
			} else {
				return "";
			}
		}

		case PropertyType::kStdString:
			return gTempStrings.Make(*reinterpret_cast<STDString *>(ptr));

		case PropertyType::kStdWString:
		{
			auto str = reinterpret_cast<STDWString *>(ptr);
			return gTempStrings.Make(ToUTF8(*str));
		}

		case PropertyType::kTranslatedString:
		{
			throw std::runtime_error("FIXME!");
			/*auto str = reinterpret_cast<TranslatedString*>(ptr);
			return gTempStrings.Make(ToUTF8(str->Handle.ReferenceString));*/
		}

		case PropertyType::kObjectHandle:
		{
			throw std::runtime_error("FIXME!");
			/*auto handle = reinterpret_cast<ObjectHandle *>(ptr);
			if (*handle) {
				// FIXME - support for client?
				auto object = esv::GetEntityWorld()->GetGameObject(*handle, false);
				if (object != nullptr) {
					return object->MyGuid.Str;
				} else {
					return {};
				}
			} else {
				return {};
			}*/
		}

		default:
			OsiError("Failed to get property '" << name << "' of [" << Name << "]: Property is not a string");
			return {};
		}
	}

	bool PropertyMapBase::setString(void * obj, FixedString const & name, char const * value, bool raw, bool throwError) const
	{
		auto prop = Properties.find(name);
		if (prop == Properties.end()) {
			if (Parent != nullptr) {
				return Parent->setString(toParent(obj), name, value, raw, throwError);
			} else {
				if (throwError) {
					OsiError("Failed to set string property '" << name << "' of [" << Name << "]: Property does not exist");
				}
				return false;
			}
		}

		if (!raw && prop->second.SetString) {
			return prop->second.SetString(obj, value);
		}

		if (!raw && !(prop->second.Flags & kPropWrite)) {
			OsiError("Failed to set string property '" << name << "' of [" << Name << "]: Property not writeable; contact Norbyte on Discord if you have a legitimate use case for doing this.");
			return false;
		}

		auto ptr = reinterpret_cast<std::uintptr_t>(obj) + prop->second.Offset;
		switch (prop->second.Type) {
		case PropertyType::kFixedString:
			{
				FixedString fs(value);
				if (!fs) {
					OsiError("Failed to set string property '" << name << "' of [" << Name << "]: Could not map to FixedString");
					return false;
				} else {
					*reinterpret_cast<FixedString *>(ptr) = fs;
					return true;
				}
			}

		case PropertyType::kDynamicFixedString:
			*reinterpret_cast<FixedString*>(ptr) = FixedString(value);
			return true;

		case PropertyType::kFixedStringGuid:
			{
				auto fs = NameGuidToFixedString(value);
				if (!fs) {
					OsiError("Failed to set string property '" << name << "' of [" << Name << "]: Could not map to FixedString GUID");
					return false;
				} else {
					*reinterpret_cast<FixedString *>(ptr) = fs;
					return true;
				}
			}

		case PropertyType::kStringPtr:
			OsiError("Failed to set property '" << name << "' of [" << Name << "]: Updating raw string properties not supported");
			return false;

		case PropertyType::kStdString:
			*reinterpret_cast<STDString *>(ptr) = value;
			return true;

		case PropertyType::kStdWString:
			*reinterpret_cast<STDWString *>(ptr) = FromUTF8(value);
			return true;

		case PropertyType::kTranslatedString:
			OsiError("Failed to set property '" << name << "' of [" << Name << "]: Updating TranslatedString properties not supported");
			return false;

		default:
			OsiError("Failed to set property '" << name << "' of [" << Name << "]: Property is not a string");
			return false;
		}
	}

	std::optional<ObjectHandle> PropertyMapBase::getHandle(void * obj, FixedString const & name, bool raw, bool throwError) const
	{
		auto prop = Properties.find(name);
		if (prop == Properties.end()) {
			if (Parent != nullptr) {
				return Parent->getHandle(toParent(obj), name, raw, throwError);
			} else {
				if (throwError) {
					OsiError("Failed to get handle property '" << name << "' of [" << Name << "]: Property does not exist");
				}
				return {};
			}
		}

		if (!raw && prop->second.GetHandle) {
			return prop->second.GetHandle(obj);
		}

		if (!raw && !(prop->second.Flags & kPropRead)) {
			OsiError("Failed to get handle property '" << name << "' of [" << Name << "]: Property not readable");
			return {};
		}

		auto ptr = reinterpret_cast<std::uintptr_t>(obj) + prop->second.Offset;
		if (prop->second.Type == PropertyType::kObjectHandle) {
			return *reinterpret_cast<ObjectHandle *>(ptr);
		} else {
			OsiError("Failed to get property '" << name << "' of [" << Name << "]: Property is not a handle");
			return {};
		}
	}

	bool PropertyMapBase::setHandle(void * obj, FixedString const & name, ObjectHandle value, bool raw, bool throwError) const
	{
		auto prop = Properties.find(name);
		if (prop == Properties.end()) {
			if (Parent != nullptr) {
				return Parent->setHandle(toParent(obj), name, value, raw, throwError);
			} else {
				if (throwError) {
					OsiError("Failed to set handle property '" << name << "' of [" << Name << "]: Property does not exist");
				}
				return false;
			}
		}

		if (!raw && prop->second.SetHandle) {
			return prop->second.SetHandle(obj, value);
		}

		if (!raw && !(prop->second.Flags & kPropWrite)) {
			OsiError("Failed to set handle property '" << name << "' of [" << Name << "]: Property not writeable; contact Norbyte on Discord if you have a legitimate use case for doing this.");
			return false;
		}

		auto ptr = reinterpret_cast<std::uintptr_t>(obj) + prop->second.Offset;
		if (prop->second.Type == PropertyType::kObjectHandle) {
			*reinterpret_cast<ObjectHandle *>(ptr) = value;
			return true;
		} else {
			OsiError("Failed to set property '" << name << "' of [" << Name << "]: Property is not a handle");
			return false;
		}
	}

	std::optional<Vector3> PropertyMapBase::getVector3(void * obj, FixedString const & name, bool raw, bool throwError) const
	{
		auto prop = Properties.find(name);
		if (prop == Properties.end()) {
			if (Parent != nullptr) {
				return Parent->getVector3(toParent(obj), name, raw, throwError);
			} else {
				if (throwError) {
					OsiError("Failed to get vector property '" << name << "' of [" << Name << "]: Property does not exist");
				}
				return {};
			}
		}

		if (!raw && prop->second.GetVector3) {
			return prop->second.GetVector3(obj);
		}

		if (!raw && !(prop->second.Flags & kPropRead)) {
			OsiError("Failed to get vector property '" << name << "' of [" << Name << "]: Property not readable");
			return {};
		}

		auto ptr = reinterpret_cast<std::uintptr_t>(obj) + prop->second.Offset;
		if (prop->second.Type == PropertyType::kVector3) {
			return *reinterpret_cast<Vector3 *>(ptr);
		} else {
			OsiError("Failed to get property '" << name << "' of [" << Name << "]: Property is not a vector");
			return {};
		}
	}

	bool PropertyMapBase::setVector3(void * obj, FixedString const & name, Vector3 const & value, bool raw, bool throwError) const
	{
		auto prop = Properties.find(name);
		if (prop == Properties.end()) {
			if (Parent != nullptr) {
				return Parent->setVector3(toParent(obj), name, value, raw, throwError);
			} else {
				if (throwError) {
					OsiError("Failed to set vector property '" << name << "' of [" << Name << "]: Property does not exist");
				}
				return false;
			}
		}

		if (!raw && prop->second.SetVector3) {
			return prop->second.SetVector3(obj, value);
		}

		if (!raw && !(prop->second.Flags & kPropWrite)) {
			OsiError("Failed to set vector property '" << name << "' of [" << Name << "]: Property not writeable; contact Norbyte on Discord if you have a legitimate use case for doing this.");
			return false;
		}

		auto ptr = reinterpret_cast<std::uintptr_t>(obj) + prop->second.Offset;
		if (prop->second.Type == PropertyType::kVector3) {
			*reinterpret_cast<Vector3 *>(ptr) = value;
			return true;
		} else {
			OsiError("Failed to get property '" << name << "' of [" << Name << "]: Property is not a vector");
			return false;
		}
	}

	std::optional<bool> PropertyMapBase::getFlag(void * obj, FixedString const & name, bool raw, bool throwError) const
	{
		auto flag = Flags.find(name);
		if (flag == Flags.end()) {
			if (Parent != nullptr) {
				return Parent->getFlag(toParent(obj), name, raw, throwError);
			} else {
				if (throwError) {
					OsiError("Failed to get flag property '" << name << "' of [" << Name << "]: Property does not exist");
				}
				return {};
			}
		}

		if (!raw && flag->second.Get) {
			return flag->second.Get(obj);
		}

		if (!raw && !(flag->second.Flags & kPropRead)) {
			OsiError("Failed to get flag property '" << name << "' of [" << Name << "]: Property not readable");
			return {};
		}

		auto value = getInt(obj, flag->second.Property, true, throwError);
		if (!value) {
			return {};
		}

		return (*value & flag->second.Mask) != 0;
	}

	bool PropertyMapBase::setFlag(void * obj, FixedString const & name, bool value, bool raw, bool throwError) const
	{
		auto flag = Flags.find(name);
		if (flag == Flags.end()) {
			if (Parent != nullptr) {
				return Parent->setFlag(toParent(obj), name, value, raw, throwError);
			} else {
				if (throwError) {
					OsiError("Failed to set flag property '" << name << "' of [" << Name << "]: Property does not exist");
				}
				return false;
			}
		}

		if (!raw && flag->second.Set) {
			return flag->second.Set(obj, value);
		}

		if (!raw && !(flag->second.Flags & kPropWrite)) {
			OsiError("Failed to set flag property '" << name << "' of [" << Name << "]: Property not writeable; contact Norbyte on Discord if you have a legitimate use case for doing this.");
			return false;
		}

		auto currentValue = getInt(obj, flag->second.Property, true, throwError);
		if (!currentValue) {
			return false;
		}

		if (value) {
			*currentValue |= flag->second.Mask;
		} else {
			*currentValue &= ~flag->second.Mask;
		}

		return setInt(obj, flag->second.Property, *currentValue, true, throwError);
	}


	bool OsirisPropertyMapGetRaw(PropertyMapBase const & propertyMap, void * obj,
		OsiArgumentDesc & args, uint32_t firstArg, OsiPropertyMapType type, bool throwError)
	{
		auto propertyNameStr = args[firstArg].String;
		FixedString propertyName(propertyNameStr);

		if (obj == nullptr) {
			OsiError("Attempted to get property '" << propertyNameStr << "' of null object!");
			return false;
		}

		if (!propertyName) {
			OsiError("Failed to get property '" << propertyNameStr << "' of [" << propertyMap.Name << "]: Property does not exist!");
			return false;
		}

		switch (type) {
		case OsiPropertyMapType::Integer:
		{
			auto val = propertyMap.getInt(obj, propertyName, false, false);
			if (val) {
				args[firstArg + 1].Set((int32_t)*val);
			} else {
				auto boolval = propertyMap.getFlag(obj, propertyName, false, throwError);
				if (boolval) {
					args[firstArg + 1].Set((int32_t)(*boolval ? 1 : 0));
				} else {
					return false;
				}
			}

			return true;
		}

		case OsiPropertyMapType::Integer64:
		{
			auto val = propertyMap.getInt(obj, propertyName, false, throwError);
			if (!val) {
				return false;
			}

			args[firstArg + 1].Set((int64_t)*val);
			return true;
		}

		case OsiPropertyMapType::Real:
		{
			auto val = propertyMap.getFloat(obj, propertyName, false, throwError);
			if (!val) {
				return false;
			}

			args[firstArg + 1].Set(*val);
			return true;
		}

		case OsiPropertyMapType::String:
		{
			auto val = propertyMap.getString(obj, propertyName, false, throwError);
			if (!val) {
				return false;
			}

			args[firstArg + 1].Set(*val);
			return true;
		}

		case OsiPropertyMapType::GuidStringHandle:
		{
			throw std::runtime_error("FIXME!");
			/*auto val = propertyMap.getHandle(obj, propertyName, false, throwError);
			if (!val) {
				return false;
			}

			if (!*val) {
				args[firstArg + 1].Set(ToFixedString("NULL_00000000-0000-0000-0000-000000000000").Str);
				return true;
			}

			auto gameObject = esv::GetEntityWorld()->GetGameObject(*val);
			if (gameObject != nullptr) {
				args[firstArg + 1].Set(gameObject->GetGuid()->Str);
				return true;
			}

			OsiError("Failed to get property '" << propertyNameStr << "': Could not map handle to game object: " << (int64_t)*val);
			return false;*/
		}

		case OsiPropertyMapType::Vector3:
		{
			auto val = propertyMap.getVector3(obj, propertyName, false, throwError);
			if (!val) {
				return false;
			}

			args[firstArg + 1].Set(val->x);
			args[firstArg + 2].Set(val->y);
			args[firstArg + 3].Set(val->z);
			return true;
		}

		default:
			OsiError("Failed to get property '" << propertyNameStr << "': Unknown Osi property type!");
			return false;
		}
	}


	bool OsirisPropertyMapSetRaw(PropertyMapBase const & propertyMap, void * obj,
		OsiArgumentDesc const & args, uint32_t firstArg, OsiPropertyMapType type, bool throwError)
	{
		auto propertyNameStr = args[firstArg].String;
		FixedString propertyName(propertyNameStr);

		if (obj == nullptr) {
			OsiError("Attempted to set property '" << propertyNameStr << "' of null object!");
			return false;
		}

		if (!propertyName) {
			OsiError("Failed to set property '" << propertyNameStr << "' of [" << propertyMap.Name << "]: Property does not exist!");
			return false;
		}

		switch (type) {
		case OsiPropertyMapType::Integer:
		{
			auto val = args[firstArg + 1].Int32;

			if (!propertyMap.setFlag(obj, propertyName, val != 0, false, false)) {
				return propertyMap.setInt(obj, propertyName, val, false, throwError);
			} else {
				return true;
			}
		}

		case OsiPropertyMapType::Integer64:
		{
			auto val = args[firstArg + 1].Int64;
			return propertyMap.setInt(obj, propertyName, val, false, throwError);
		}

		case OsiPropertyMapType::Real:
		{
			auto val = args[firstArg + 1].Float;
			return propertyMap.setFloat(obj, propertyName, val, false, throwError);
		}

		case OsiPropertyMapType::String:
		{
			auto val = args[firstArg + 1].String;
			return propertyMap.setString(obj, propertyName, val, false, throwError);
		}

		case OsiPropertyMapType::GuidStringHandle:
		{
			throw std::runtime_error("FIXME!");
			/*auto guid = args[firstArg + 1].String;
			if (guid == nullptr
				|| !strlen(guid)
				|| NameGuidToFixedString(guid) == ToFixedString("00000000-0000-0000-0000-000000000000")) {
				return propertyMap.setHandle(obj, propertyName, ObjectHandle{}, false, throwError);
			}

			auto gameObject = esv::GetEntityWorld()->GetGameObject(guid);
			if (gameObject != nullptr) {
				ObjectHandle handle;
				gameObject->GetObjectHandle(handle);
				return propertyMap.setHandle(obj, propertyName, handle, false, throwError);
			}

			OsiError("Failed to set property '" << propertyNameStr << "': Could not map GUID to game object: " << guid);
			return false;*/
		}

		case OsiPropertyMapType::Vector3:
		{
			auto x = args[firstArg + 1].Float;
			auto y = args[firstArg + 2].Float;
			auto z = args[firstArg + 3].Float;
			Vector3 vec(x, y, z);
			return propertyMap.setVector3(obj, propertyName, vec, false, throwError);
		}

		default:
			OsiError("Failed to set property '" << propertyNameStr << "': Unknown Osi property type!");
			return false;
		}
	}


	bool LuaPropertyMapGet(lua_State* L, PropertyMapBase const& propertyMap, void* obj,
		char const* propertyName, bool throwError)
	{
		FixedString propertyFS(propertyName);
		if (!propertyFS) {
			OsiError("Failed to get property '" << propertyName << "' of [" << propertyMap.Name << "]: Property does not exist!");
			return false;
		}

		return LuaPropertyMapGet(L, propertyMap, obj, propertyFS, throwError);
	}

	bool LuaPropertyMapGet(lua_State * L, PropertyMapBase const & propertyMap, void * obj,
		FixedString const& propertyName, bool throwError)
	{
		if (obj == nullptr) {
			if (throwError) {
				OsiError("Attempted to get property '" << propertyName << "' of null object!");
			}
			return false;
		}

		auto prop = propertyMap.findProperty(propertyName);
		if (prop == nullptr) {
			auto flag = propertyMap.findFlag(propertyName);
			if (flag == nullptr) {
				if (throwError) {
					OsiError("Failed to get property '" << propertyName << "' of [" << propertyMap.Name << "]: Property does not exist!");
				}
				return {};
			} else {
				auto val = propertyMap.getFlag(obj, propertyName, false, throwError);
				if (val) {
					lua::push(L, *val);
					return true;
				} else {
					return false;
				}
			}
		}

		auto type = prop->Type;
		if (prop->GetString) {
			// Return enumeration labels instead of IDs if possible
			type = PropertyType::kFixedString;
		}

		switch (type) {
		case PropertyType::kBool:
		{
			auto val = propertyMap.getInt(obj, propertyName, false, throwError);
			if (val) {
				lua::push(L, *val != 0);
				return true;
			} else {
				return false;
			}
		}

		case PropertyType::kUInt8:
		case PropertyType::kInt16:
		case PropertyType::kUInt16:
		case PropertyType::kInt32:
		case PropertyType::kUInt32:
		case PropertyType::kInt64:
		case PropertyType::kUInt64:
		{
			auto val = propertyMap.getInt(obj, propertyName, false, throwError);
			if (val) {
				lua::push(L, *val);
				return true;
			} else {
				return false;
			}
		}

		case PropertyType::kFloat:
		{
			auto val = propertyMap.getFloat(obj, propertyName, false, throwError);
			if (val) {
				lua::push(L, *val);
				return true;
			} else {
				return false;
			}
		}

		case PropertyType::kFixedString:
		case PropertyType::kDynamicFixedString:
		case PropertyType::kFixedStringGuid:
		case PropertyType::kStringPtr:
		case PropertyType::kStdString:
		case PropertyType::kStdWString:
		case PropertyType::kTranslatedString:
		{
			auto val = propertyMap.getString(obj, propertyName, false, throwError);
			if (val) {
				lua::push(L, *val);
				return true;
			} else {
				return false;
			}
		}

		case PropertyType::kObjectHandle:
		{
			auto val = propertyMap.getHandle(obj, propertyName, false, throwError);
			if (val) {
				lua::push(L, *val);
				return true;
			} else {
				return false;
			}
		}

		case PropertyType::kVector3:
		{
			auto val = propertyMap.getVector3(obj, propertyName, false, throwError);
			if (val) {
				lua::push(L, *val);
				return true;
			} else {
				return false;
			}
		}

		default:
			OsiError("Failed to get property '" << propertyName << "': Unsupported property type for Lua fetch!");
			return false;
		}
	}


	bool LuaPropertyMapSet(lua_State * L, int index, PropertyMapBase const & propertyMap, 
		void * obj, char const * propertyName, bool throwError)
	{
		if (obj == nullptr) {
			if (throwError) {
				OsiError("Attempted to set property '" << propertyName << "' of null object!");
			}
			return false;
		}

		FixedString propertyFS(propertyName);
		if (!propertyFS) {
			OsiError("Failed to set property '" << propertyName << "' of [" << propertyMap.Name << "]: Property does not exist!");
			return false;
		}

		auto prop = propertyMap.findProperty(propertyFS);
		if (prop == nullptr) {
			auto flag = propertyMap.findFlag(propertyFS);
			if (flag == nullptr) {
				if (throwError) {
					OsiError("Failed to set property '" << propertyName << "' of [" << propertyMap.Name << "]: Property does not exist!");
				}
				return {};
			} else {
				luaL_checktype(L, index, LUA_TBOOLEAN);
				auto val = lua_toboolean(L, index);
				return propertyMap.setFlag(obj, propertyFS, val == 1, false, throwError);
			}
		}

		auto type = prop->Type;
		if (prop->SetString && lua_type(L, index) == LUA_TSTRING) {
			// Allow setting enumerations using labels
			type = PropertyType::kFixedString;
		}

		switch (type) {
		case PropertyType::kBool:
		{
			luaL_checktype(L, index, LUA_TBOOLEAN);
			auto val = lua_toboolean(L, index);
			return propertyMap.setInt(obj, propertyFS, val, false, throwError);
		}

		case PropertyType::kUInt8:
		case PropertyType::kInt16:
		case PropertyType::kUInt16:
		case PropertyType::kInt32:
		case PropertyType::kUInt32:
		case PropertyType::kInt64:
		case PropertyType::kUInt64:
		{
			auto val = luaL_checkinteger(L, index);
			return propertyMap.setInt(obj, propertyFS, val, false, throwError);
		}

		case PropertyType::kFloat:
		{
			auto val = luaL_checknumber(L, index);
			return propertyMap.setFloat(obj, propertyFS, (float)val, false, throwError);
		}

		case PropertyType::kFixedString:
		case PropertyType::kDynamicFixedString:
		case PropertyType::kFixedStringGuid:
		case PropertyType::kStringPtr:
		case PropertyType::kStdString:
		case PropertyType::kStdWString:
		case PropertyType::kTranslatedString:
		{
			auto val = luaL_checkstring(L, index);
			return propertyMap.setString(obj, propertyFS, val, false, throwError);
		}

		case PropertyType::kObjectHandle:
		{
			auto handle = lua::checked_get<ObjectHandle>(L, index);
			return propertyMap.setHandle(obj, propertyFS, handle, false, throwError);
		}

		case PropertyType::kVector3:
		{
			auto val = lua::checked_get<glm::vec3>(L, index);
			return propertyMap.setVector3(obj, propertyFS, val, false, throwError);
		}

		default:
			OsiError("Failed to set property '" << propertyName << "': Unsupported property type for Lua update!");
			return false;
		}
	}
}