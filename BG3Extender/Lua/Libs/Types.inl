/// <lua_module>Types</lua_module>
BEGIN_NS(lua::types)

std::optional<STDString> GetCppObjectTypeName(lua_State * L, int index)
{
	CppObjectMetadata meta;
	lua_get_cppobject(L, index, meta);

	switch (meta.MetatableTag) {
	case MetatableTag::ObjectRef:
	{
		auto propertyMap = gStructRegistry.Get(meta.PropertyMapTag);
		return propertyMap->Name.GetString();
	}

	case MetatableTag::Array:
	{
		auto impl = gExtender->GetPropertyMapManager().GetArrayProxy(meta.PropertyMapTag);
		return impl->GetContainerType().TypeName.GetString();
	}

	case MetatableTag::Map:
	{
		auto impl = gExtender->GetPropertyMapManager().GetMapProxy(meta.PropertyMapTag);
		return impl->GetContainerType().TypeName.GetString();
	}

	case MetatableTag::Set:
	{
		auto impl = gExtender->GetPropertyMapManager().GetSetProxy(meta.PropertyMapTag);
		return impl->GetContainerType().TypeName.GetString();
	}

	case MetatableTag::EnumValue:
	{
		CppValueMetadata val;
		lua_get_cppvalue(L, index, val);
		return EnumValueMetatable::GetTypeName(L, val);
	}

	case MetatableTag::BitfieldValue:
	{
		CppValueMetadata val;
		lua_get_cppvalue(L, index, val);
		return BitfieldValueMetatable::GetTypeName(L, val);
	}

	case MetatableTag::UserVariableHolder:
	{
		CppValueMetadata val;
		lua_get_cppvalue(L, index, val);
		return UserVariableHolderMetatable::GetTypeName(L, val);
	}

	case MetatableTag::ModVariableHolder:
	{
		CppValueMetadata val;
		lua_get_cppvalue(L, index, val);
		return ModVariableHolderMetatable::GetTypeName(L, val);
	}

	case MetatableTag::Entity:
	{
		return "Entity";
	}

#if defined(ENABLE_IMGUI)
	case MetatableTag::ImguiObject:
	{
		CppValueMetadata val;
		lua_get_cppvalue(L, index, val);
		return ImguiObjectProxyMetatable::GetTypeName(L, val);
	}
#endif

	case MetatableTag::OsiFunctionName:
	{
		return "OsiFunction";
	}

	default:
		return {};
	}
}

TypeInformation const* GetCppObjectType(lua_State * L, int index)
{
#if defined(ENABLE_IMGUI)
	CppValueMetadata val;
	if (lua_try_get_cppvalue(L, index, val) && val.MetatableTag == MetatableTag::ImguiObject) {
		auto obj = ImguiObjectProxyMetatable::GetRenderable(val);
		if (obj != nullptr) {
			return obj->GetRTTI().TypeInfo;
		} else {
			return nullptr;
		}
	}
#endif

	CppObjectMetadata meta;
	lua_get_cppobject(L, index, meta);

	switch (meta.MetatableTag) {
	case MetatableTag::ObjectRef:
	{
		auto propertyMap = gStructRegistry.Get(meta.PropertyMapTag);
		return propertyMap->TypeInfo;
	}

	case MetatableTag::Array:
	{
		auto impl = gExtender->GetPropertyMapManager().GetArrayProxy(meta.PropertyMapTag);
		return &impl->GetContainerType();
	}

	case MetatableTag::Map:
	{
		auto impl = gExtender->GetPropertyMapManager().GetMapProxy(meta.PropertyMapTag);
		return &impl->GetContainerType();
	}

	case MetatableTag::Set:
	{
		auto impl = gExtender->GetPropertyMapManager().GetSetProxy(meta.PropertyMapTag);
		return &impl->GetContainerType();
	}

	case MetatableTag::EnumValue:
	case MetatableTag::BitfieldValue:
		// TODO - implement reflection for enum/bitfield types!

	case MetatableTag::UserVariableHolder:
	case MetatableTag::ModVariableHolder:
	case MetatableTag::Entity:
	default:
		return nullptr;
	}
}

std::optional<STDString> GetObjectType(lua_State* L, AnyRef object)
{
	switch (lua_type(L, object.Index)) {
	case LUA_TLIGHTCPPOBJECT:
	case LUA_TCPPOBJECT:
		return GetCppObjectTypeName(L, 1);
	default:
		return {};
	}
}

TypeInformation* TypeOf(lua_State* L, AnyRef object)
{
	switch (lua_type(L, object.Index)) {
	case LUA_TLIGHTCPPOBJECT:
	case LUA_TCPPOBJECT:
		return const_cast<TypeInformation*>(GetCppObjectType(L, 1));
	default:
		return nullptr;
	}
}

bool IsA(lua_State* L, AnyRef object, FixedString typeName)
{
	auto expectedType = TypeInformationRepository::GetInstance().TryGetType(typeName);
	if (expectedType == nullptr) {
		luaL_error(L, "No such type: %s", typeName.GetString());
	}

	auto type = TypeOf(L, object);
	if (type == nullptr) return false;

	// Simple case: Types are equivalent
	if (type == expectedType) return true;

	// Check if both types are part of an inheritance tree
	if (type->PropertyMap != nullptr && expectedType->PropertyMap != nullptr) {
		// Yes, check if one of an ancestor of the other
		return type->PropertyMap->IsA(expectedType->PropertyMap->RegistryIndex);
	} else {
		// No, they cannot be the same type
		return false;
	}
}

TypeInformation* GetTypeInfo(FixedString const& typeName)
{
	auto const& type = TypeInformationRepository::GetInstance().GetType(typeName);
	if (type.Kind == LuaTypeId::Unknown) {
		return nullptr;
	} else {
		return const_cast<TypeInformation*>(&type);
	}
}

Array<FixedString> GetAllTypes()
{
	Array<FixedString> types;
	auto const& allTypes = TypeInformationRepository::GetInstance().GetAllTypes();
	for (auto const& type : allTypes) {
		types.push_back(type.Key);
	}

	return types;
}

void RegisterEnumeration(lua_State* L, EnumInfoStore const& ty)
{
	lua_newtable(L);

	for (auto const& elem : ty.Values) {
		EnumValueMetatable::Make(L, elem.Value, ty.RegistryIndex);
		lua_setfield(L, -2, elem.Key.GetString());

		push(L, elem.Value);
		EnumValueMetatable::Make(L, elem.Value, ty.RegistryIndex);
		lua_rawset(L, -3);
	}

	lua_setfield(L, -2, ty.LuaName.GetString());
}

void RegisterEnumeration(lua_State* L, BitfieldInfoStore const& ty)
{
	lua_newtable(L);

	for (auto const& elem : ty.Values) {
		BitfieldValueMetatable::Make(L, elem.Value, ty.RegistryIndex);
		lua_setfield(L, -2, elem.Key.GetString());

		push(L, elem.Value);
		BitfieldValueMetatable::Make(L, elem.Value, ty.RegistryIndex);
		lua_rawset(L, -3);
	}

	lua_setfield(L, -2, ty.LuaName.GetString());
}

void RegisterEnumerations(lua_State* L)
{
	StackCheck _(L);
	lua_getglobal(L, "Ext");
	lua_newtable(L);

	for (auto const& ty : EnumRegistry::Get().EnumsById) {
		RegisterEnumeration(L, *ty);
	}
	
	for (auto const& ty : BitfieldRegistry::Get().BitfieldsById) {
		RegisterEnumeration(L, *ty);
	}

	lua_setfield(L, -2, "Enums");
	lua_pop(L, 1);
}

bool Validate(lua_State* L)
{
	CppObjectMetadata meta;
	lua_get_cppobject(L, 1, MetatableTag::ObjectRef, meta);
	auto& pm = LightObjectProxyMetatable::GetPropertyMap(meta);
	return pm.ValidateObject(meta.Ptr);
}

UserReturn Serialize(lua_State* L)
{
	auto type = lua_type(L, 1);

	if (type != LUA_TLIGHTCPPOBJECT) {
		return luaL_error(L, "Don't know how to serialize values of type %s", lua_typename(L, type));
	}

	CppObjectMetadata meta;
	lua_get_cppobject(L, 1, meta);

	switch (meta.MetatableTag) {
		case MetatableTag::ObjectRef:
		{
			auto& pm = LightObjectProxyMetatable::GetPropertyMap(meta);
			pm.Serialize(L, meta.Ptr);
			return 1;
		}
			
		case MetatableTag::Array:
		{
			auto impl = ArrayProxyMetatable::GetImpl(meta);
			impl->Serialize(L, meta);
			return 1;
		}

		case MetatableTag::Map:
		{
			auto impl = MapProxyMetatable::GetImpl(meta);
			impl->Serialize(L, meta);
			return 1;
		}

		case MetatableTag::Set:
		{
			auto impl = SetProxyMetatable::GetImpl(meta);
			impl->Serialize(L, meta);
			return 1;
		}

		default:
			return luaL_error(L, "Don't know how to serialize userdata of metatype %d", (unsigned)meta.MetatableTag);
	}
}

void Unserialize(lua_State* L)
{
	auto type = lua_type(L, 1);
	luaL_checktype(L, 2, LUA_TTABLE);

	if (type != LUA_TLIGHTCPPOBJECT) {
		luaL_error(L, "Don't know how to unserialize objects of this type");
	}

	CppObjectMetadata meta;
	lua_get_cppobject(L, 1, meta);

	switch (meta.MetatableTag) {
		case MetatableTag::ObjectRef:
		{
			auto& pm = LightObjectProxyMetatable::GetPropertyMap(meta);
			pm.Unserialize(L, 2, meta.Ptr);
			return;
		}

		case MetatableTag::Array:
		{
			auto impl = ArrayProxyMetatable::GetImpl(meta);
			impl->Unserialize(L, meta, 2);
			return;
		}

		case MetatableTag::Map:
		{
			auto impl = MapProxyMetatable::GetImpl(meta);
			impl->Unserialize(L, meta, 2);
			return;
		}

		case MetatableTag::Set:
		{
			auto impl = SetProxyMetatable::GetImpl(meta);
			impl->Unserialize(L, meta, 2);
			return;
		}
	}
}

UserReturn Construct(lua_State* L, FixedString const& typeName)
{
	auto const& type = TypeInformationRepository::GetInstance().GetType(typeName);
	if (type.Kind == LuaTypeId::Unknown) {
		return luaL_error(L, "Unknown type name '%s'", typeName.GetString());
	}
	
	if (type.Kind != LuaTypeId::Object) {
		return luaL_error(L, "Unable to construct non-object type '%s'", typeName.GetString());
	}
	
	if (type.PropertyMap == nullptr || type.PropertyMap->Construct == nullptr) {
		return luaL_error(L, "Type '%s' is not constructible", typeName.GetString());
	}

	// TODO
	return 0;
}

std::optional<STDString> GetValueType(lua_State* L, AnyRef object)
{
	if (lua_type(L, object.Index) == LUA_TLIGHTCPPOBJECT) {
		CppObjectMetadata meta;
		lua_get_cppobject(L, object.Index, meta);

		switch (meta.MetatableTag) {
		case MetatableTag::ObjectRef: return "CppObject";
		case MetatableTag::Array: return "Array";
		case MetatableTag::Map: return "Map";
		case MetatableTag::Set: return "Set";
		case MetatableTag::EnumValue: return "Enum";
		case MetatableTag::BitfieldValue: return "Bitfield";
		case MetatableTag::UserVariableHolder: return "UserVariableHolder";
		case MetatableTag::ModVariableHolder: return "ModVariableHolder";
		case MetatableTag::Entity: return "Entity";
	#if defined(ENABLE_IMGUI)
		case MetatableTag::ImguiObject: return "ImguiObject";
	#endif
		case MetatableTag::OsiFunctionName: return "OsiFunction";

		default: return {};
		}
	} else {
		return lua_typename(L, lua_type(L, object.Index));
	}
}

void RegisterTypesLib()
{
	DECLARE_MODULE(Types, Both)
	BEGIN_MODULE()
	MODULE_FUNCTION(GetValueType)
	MODULE_FUNCTION(GetObjectType)
	MODULE_FUNCTION(GetTypeInfo)
	MODULE_FUNCTION(TypeOf)
	MODULE_FUNCTION(IsA)
	MODULE_FUNCTION(GetAllTypes)
	MODULE_FUNCTION(Validate)
	MODULE_FUNCTION(Serialize)
	MODULE_FUNCTION(Unserialize)
	MODULE_FUNCTION(Construct)
	END_MODULE()
}

END_NS()
