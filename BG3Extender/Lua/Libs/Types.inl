/// <lua_module>Types</lua_module>
BEGIN_NS(lua::types)

std::optional<STDString> GetUserdataObjectTypeName(lua_State * L, int index)
{
	auto object = Userdata<ObjectProxy>::AsUserData(L, index);
	if (object) {
		return object->GetImpl()->GetTypeName().GetString();
	}

	return {};
}

UserReturn GetObjectType(lua_State * L)
{
	StackCheck _(L, 1);
	std::optional<STDString> type;
	switch (lua_type(L, 1)) {
	case LUA_TUSERDATA: type = GetUserdataObjectTypeName(L, 1); break;
	}

	push(L, type);
	return 1;
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

void RegisterEnumeration(lua_State* L, EnumInfoStore<EnumUnderlyingType> const& ty)
{
	/*lua_newtable(L);

	for (auto const& elem : ty.Values) {
		EnumValueMetatable::Make(L, elem.Value, ty.RegistryIndex);
		lua_setfield(L, -2, elem.Key.GetString());

		push(L, elem.Value);
		EnumValueMetatable::Make(L, elem.Value, ty.RegistryIndex);
		lua_settable(L, -3);
	}

	lua_setfield(L, -2, ty.LuaName.GetString());*/
}

void RegisterEnumeration(lua_State* L, BitmaskInfoStore<EnumUnderlyingType> const& ty)
{
	/*lua_newtable(L);

	for (auto const& elem : ty.Values) {
		BitfieldValueMetatable::Make(L, elem.Value, ty.RegistryIndex);
		lua_setfield(L, -2, elem.Key.GetString());

		push(L, elem.Value);
		BitfieldValueMetatable::Make(L, elem.Value, ty.RegistryIndex);
		lua_settable(L, -3);
	}

	lua_setfield(L, -2, ty.LuaName.GetString());*/
}

void RegisterEnumerations(lua_State* L)
{
	StackCheck _(L);
	lua_getglobal(L, "Ext");
	lua_newtable(L);

	for (auto const& ty : EnumRegistry::Get().EnumsById) {
		RegisterEnumeration(L, *ty);
	}
	
	for (auto const& ty : BitmaskRegistry::Get().BitfieldsById) {
		RegisterEnumeration(L, *ty);
	}

	lua_setfield(L, -2, "Enums");
	lua_pop(L, 1);
}

bool Validate(lua_State* L)
{
	auto proxy = Userdata<ObjectProxy>::CheckUserData(L, 1);
	auto& pm = proxy->GetImpl()->GetPropertyMap();
	return pm.ValidateObject(proxy->GetRaw(L));
}

UserReturn Serialize(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TUSERDATA);

	auto proxy = Userdata<ObjectProxy>::AsUserData(L, 1);
	if (proxy != nullptr) {
		auto& pm = proxy->GetImpl()->GetPropertyMap();
		if (pm.Serialize != nullptr) {
			pm.Serialize(L, proxy->GetRaw(L));
			return 1;
		} else {
			return luaL_error(L, "Object class '%s' does not support serialization", pm.Name.GetString());
		}
	}

	auto arr = Userdata<ArrayProxy>::AsUserData(L, 1);
	if (arr != nullptr) {
		arr->GetImpl()->Serialize(L);
		return 1;
	}

	auto map = Userdata<MapProxy>::AsUserData(L, 1);
	if (map != nullptr) {
		map->GetImpl()->Serialize(L);
		return 1;
	}

	auto set = Userdata<SetProxy>::AsUserData(L, 1);
	if (set != nullptr) {
		set->GetImpl()->Serialize(L);
		return 1;
	}

	return luaL_error(L, "Don't know how to serialize objects of this type");
}

void Unserialize(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TUSERDATA);
	luaL_checktype(L, 2, LUA_TTABLE);

	auto proxy = Userdata<ObjectProxy>::AsUserData(L, 1);
	if (proxy) {
		auto const& pm = proxy->GetImpl()->GetPropertyMap();
		if (pm.Unserialize == nullptr) {
			luaL_error(L, "No serializer available for type '%s'", proxy->GetImpl()->GetTypeName().GetString());
		}

		pm.Unserialize(L, 2, proxy->GetRaw(L));
		return;
	}

	// Not very nice, but will do until the new reference system is added
	auto arr = Userdata<ArrayProxy>::AsUserData(L, 1);
	if (arr) {
		arr->GetImpl()->Unserialize(L, 2);
		return;
	}

	auto map = Userdata<MapProxy>::AsUserData(L, 1);
	if (map) {
		map->GetImpl()->Unserialize(L, 2);
		return;
	}

	auto set = Userdata<SetProxy>::AsUserData(L, 1);
	if (set) {
		set->GetImpl()->Unserialize(L, 2);
		return;
	}

	luaL_error(L, "Don't know how to unserialize objects of this type");
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

void RegisterTypesLib()
{
	DECLARE_MODULE(Types, Both)
	BEGIN_MODULE()
	MODULE_FUNCTION(GetObjectType)
	MODULE_FUNCTION(GetTypeInfo)
	MODULE_FUNCTION(GetAllTypes)
	MODULE_FUNCTION(Validate)
	MODULE_FUNCTION(Serialize)
	MODULE_FUNCTION(Unserialize)
	MODULE_FUNCTION(Construct)
	END_MODULE()
}

END_NS()
