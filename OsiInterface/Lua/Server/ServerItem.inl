#include <GameDefinitions/Item.h>
#include <Lua/LuaBinding.h>

namespace dse::lua
{
	char const* const ObjectProxy<esv::Item>::MetatableName = "esv::Item";

	esv::Item* ObjectProxy<esv::Item>::Get(lua_State* L)
	{
		if (obj_) return obj_;
		auto item = gOsirisProxy->GetServerEntityHelpers().GetComponent<esv::Item>(handle_);
		if (item == nullptr) luaL_error(L, "Item handle invalid");
		return item;
	}

	int ItemGetInventoryItems(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<ObjectProxy<esv::Item>*>(L, 1);

		GetInventoryItems(L, self->Get(L)->InventoryHandle);

		return 1;
	}

	int ItemGetNearbyCharacters(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*StackCheck _(L, 1);
		auto self = checked_get<ObjectProxy<esv::Item>*>(L, 1);
		auto pos = self->Get(L)->WorldPos;
		auto distance = checked_get<float>(L, 2);

		esv::lua::GetCharactersGeneric(L, FixedString{}, [pos, distance](esv::Character* c) {
			return abs(glm::length(pos - c->WorldPos)) < distance;
		});
		return 1;*/
	}

	int ItemGetGeneratedBoosts(lua_State* L)
	{
		auto self = checked_get<ObjectProxy<esv::Item>*>(L, 1);
		auto item = self->Get(L);
		if (!item) return 0;

		StackCheck _(L, 1);
		lua_newtable(L);
		int32_t index{ 1 };
		if (item->Generation != nullptr) {
			for (auto const& boost : item->Generation->Boosts) {
				settable(L, index++, boost);
			}
		}

		return 1;
	}

	int ObjectProxy<esv::Item>::Index(lua_State* L)
	{
		auto item = Get(L);
		if (!item) return 0;

		StackCheck _(L, 1);
		auto prop = luaL_checkstring(L, 2);
		FixedString propFS(prop);
		if (!propFS) {
			OsiError("[esv::Item] has no property named '" << prop << "'");
			push(L, nullptr);
			return 1;
		}

		if (propFS == GFS.strGetInventoryItems) {
			lua_pushcfunction(L, &ItemGetInventoryItems);
			return 1;
		}

		if (propFS == GFS.strGetNearbyCharacters) {
			lua_pushcfunction(L, &ItemGetNearbyCharacters);
			return 1;
		}

		if (propFS == GFS.strGetDeltaMods) {
			lua_pushcfunction(L, &ItemGetDeltaMods<esv::Item>);
			return 1;
		}

		if (propFS == GFS.strGetGeneratedBoosts) {
			lua_pushcfunction(L, &ItemGetGeneratedBoosts);
			return 1;
		}

		if (propFS == GFS.strHasTag) {
			lua_pushcfunction(L, &GameObjectHasTag<esv::Item>);
			return 1;
		}

		if (propFS == GFS.strGetTags) {
			lua_pushcfunction(L, &GameObjectGetTags<esv::Item>);
			return 1;
		}

		if (propFS == GFS.strGetStatus) {
			lua_pushcfunction(L, (&GameObjectGetStatus<esv::Item, esv::Status>));
			return 1;
		}

		if (propFS == GFS.strGetStatusByType) {
			lua_pushcfunction(L, (&GameObjectGetStatusByType<esv::Item, esv::Status>));
			return 1;
		}

		if (propFS == GFS.strGetStatuses) {
			lua_pushcfunction(L, (&GameObjectGetStatuses<esv::Item>));
			return 1;
		}

		if (propFS == GFS.strGetStatusObjects) {
			lua_pushcfunction(L, (&GameObjectGetStatusObjects<esv::Item, esv::Status>));
			return 1;
		}

		if (propFS == GFS.strStats) {
			if (item->Stats != nullptr) {
				ObjectProxy<CDivinityStats_Item>::New(L, handle_);
				return 1;
			} else {
				OsiError("Item has no stats.");
				push(L, nullptr);
				return 1;
			}
		}

		if (propFS == GFS.strHandle) {
			push(L, item->Base.ComponentHandle);
			return 1;
		}

		if (propFS == GFS.strRootTemplate) {
			ObjectProxy<ItemTemplate>::New(L, item->CurrentTemplate);
			return 1;
		}

		if (propFS == GFS.strDisplayName) {
			return GameObjectGetDisplayName<esv::Item>(L, item);
		}

		/* FIXME
		bool fetched = false;
		if (item->Stats != nullptr) {
			fetched = LuaPropertyMapGet(L, gItemStatsPropertyMap, item->Stats, propFS, false);
		}

		if (!fetched) {
			fetched = LuaPropertyMapGet(L, gItemPropertyMap, item, propFS, true);
		}

		if (!fetched)*/ push(L, nullptr);
		return 1;
	}

	int ObjectProxy<esv::Item>::NewIndex(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*
		return GenericSetter(L, gItemPropertyMap);*/
	}


	/*char const* const ObjectProxy<eoc::ItemDefinition>::MetatableName = "eoc::ItemDefinition";

	eoc::ItemDefinition* ObjectProxy<eoc::ItemDefinition>::Get(lua_State* L)
	{
		if (obj_) return obj_;
		luaL_error(L, "ItemDefinition object has expired!");
		return nullptr;
	}

	int ItemDefinitionResetProgression(lua_State* L)
	{
		StackCheck _(L, 0);
		auto self = ObjectProxy<eoc::ItemDefinition>::CheckedGet(L, 2);
		self->LevelGroupIndex = -1;
		self->RootGroupIndex = -1;
		self->NameIndex = -1;
		self->NameCool = 0;
		return 0;
	}

	int ObjectProxy<eoc::ItemDefinition>::Index(lua_State* L)
	{
		StackCheck _(L, 1);
		auto prop = checked_get<FixedString>(L, 2);
		if (prop == GFS.strResetProgression) {
			lua_pushcfunction(L, &ItemDefinitionResetProgression);
			return 1;
		}

		if (prop == GFS.strGenerationBoosts) {
			LuaWrite(L, Get(L)->GenerationBoosts);
			return 1;
		}

		if (prop == GFS.strRuneBoosts) {
			LuaWrite(L, Get(L)->RuneBoosts);
			return 1;
		}

		if (prop == GFS.strDeltaMods) {
			LuaWrite(L, Get(L)->DeltaMods);
			return 1;
		}

		return GenericGetter(L, gEoCItemDefinitionPropertyMap);
	}

	int ObjectProxy<eoc::ItemDefinition>::NewIndex(lua_State* L)
	{
		StackCheck _(L, 0);
		auto prop = checked_get<FixedString>(L, 2);
		if (prop == GFS.strGenerationBoosts) {
			lua_pushvalue(L, 3);
			LuaRead(L, Get(L)->GenerationBoosts);
			lua_pop(L, 1);
			return 0;
		}

		if (prop == GFS.strRuneBoosts) {
			lua_pushvalue(L, 3);
			LuaRead(L, Get(L)->RuneBoosts);
			lua_pop(L, 1);
			return 0;
		}

		if (prop == GFS.strDeltaMods) {
			lua_pushvalue(L, 3);
			LuaRead(L, Get(L)->DeltaMods);
			lua_pop(L, 1);
			return 0;
		}

		return GenericSetter(L, gEoCItemDefinitionPropertyMap);
	}*/
}

namespace dse::esv::lua
{
	using namespace dse::lua;

	/*char const* const ItemConstructor::MetatableName = "ItemConstructor";

	int ItemConstructorConstructItem(lua_State* L)
	{
		auto self = ItemConstructor::CheckUserData(L, 1);

		auto item = GetStaticSymbols().CreateItemFromParsed(&self->Get(), 0);
		if (item == nullptr) {
			OsiErrorS("Failed to create parsed item.");
			return 0;
		}

		ObjectProxy<esv::Item>::New(L, item);
		return 1;
	}

	int ItemConstructor::Index(lua_State* L)
	{
		StackCheck _(L, 1);
		if (lua_type(L, 2) == LUA_TSTRING) {
			auto func = checked_get<FixedString>(L, 2);
			if (func == GFS.strConstruct) {
				lua_pushcfunction(L, &ItemConstructorConstructItem);
				return 1;
			}

			OsiError("ItemConstructor has no property named " << func.Str);
			push(L, nullptr);
			return 1;
		} else {
			auto idx = checked_get<int>(L, 2);
			if (idx < 1 || idx > (int)definition_.Size) {
				return luaL_error(L, "Clone set only has %d elements", definition_.Size);
			}

			ObjectProxy<eoc::ItemDefinition>::New(L, &definition_[idx - 1]);
			return 1;
		}
	}*/
}

namespace dse::esv::lua
{
	using namespace dse::lua;

	int GetItem(lua_State* L)
	{
		LuaServerPin lua(esv::ExtensionState::Get());
		if (lua->RestrictionFlags & State::RestrictHandleConversion) {
			return luaL_error(L, "Attempted to resolve item handle in restricted context");
		}

		StackCheck _(L, 1);
		esv::Item* item = nullptr;
		switch (lua_type(L, 1)) {
		case LUA_TLIGHTUSERDATA:
		{
			auto handle = checked_get<ObjectHandle>(L, 1);
			item = gOsirisProxy->GetServerEntityHelpers().GetComponent<esv::Item>(handle);
			break;
		}

		case LUA_TNUMBER:
		{
			OsiError("Resolving integer object handles is deprecated since v52!")
			auto handle = ObjectHandle(lua_tointeger(L, 1));
			item = gOsirisProxy->GetServerEntityHelpers().GetComponent<esv::Item>(handle);
			break;
		}

		case LUA_TSTRING:
		{
			auto guid = lua_tostring(L, 1);
			item = gOsirisProxy->GetServerEntityHelpers().GetComponent<esv::Item>(guid);
			break;
		}

		default:
			OsiError("Expected item GUID or handle, got " << lua_typename(L, lua_type(L, 1)));
			push(L, nullptr);
			return 1;
		}

		if (item != nullptr) {
			ObjectHandle handle;
			item->GetObjectHandle(handle);
			ObjectProxy<esv::Item>::New(L, handle);
		} else {
			push(L, nullptr);
		}

		return 1;
	}


	int GetGameObject(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*LuaServerPin lua(ExtensionState::Get());
		if (lua->RestrictionFlags & State::RestrictHandleConversion) {
			return luaL_error(L, "Attempted to resolve game object handle in restricted context");
		}

		StackCheck _(L, 1);
		esv::Item* item = nullptr;
		esv::Character* character = nullptr;
		esv::Projectile* projectile = nullptr;
		switch (lua_type(L, 1)) {
		case LUA_TLIGHTUSERDATA:
		{
			auto handle = checked_get<ObjectHandle>(L, 1);
			if (handle) {
				switch ((ObjectType)handle.GetType()) {
				case ObjectType::ServerCharacter:
					character = GetEntityWorld()->GetCharacter(handle);
					break;

				case ObjectType::ServerItem:
					item = GetEntityWorld()->GetItem(handle);
					break;

				case ObjectType::ServerProjectile:
					projectile = GetEntityWorld()->GetProjectile(handle);
					break;

				default:
					OsiError("Cannot resolve unsupported server handle type: " << handle.GetType());
					break;
				}
			}

			break;
		}

		case LUA_TNUMBER:
		{
			OsiError("Resolving integer object handles is deprecated since v52!")
			auto handle = ObjectHandle(lua_tointeger(L, 1));
			if (handle) {
				switch ((ObjectType)handle.GetType()) {
				case ObjectType::ServerCharacter:
					character = GetEntityWorld()->GetCharacter(handle);
					break;

				case ObjectType::ServerItem:
					item = GetEntityWorld()->GetItem(handle);
					break;

				case ObjectType::ServerProjectile:
					projectile = GetEntityWorld()->GetProjectile(handle);
					break;

				default:
					OsiError("Cannot resolve unsupported server handle type: " << handle.GetType());
					break;
				}
			}

			break;
		}

		case LUA_TSTRING:
		{
			auto guid = lua_tostring(L, 1);
			character = GetEntityWorld()->GetCharacter(guid, false);
			item = GetEntityWorld()->GetItem(guid, false);
			break;
		}

		default:
			OsiError("Expected object GUID or handle, got " << lua_typename(L, lua_type(L, 1)));
			push(L, nullptr);
			return 1;
		}

		if (item != nullptr) {
			ObjectHandle handle;
			item->GetObjectHandle(handle);
			ObjectProxy<esv::Item>::New(L, handle);
			return 1;
		} else if (character != nullptr) {
			ObjectHandle handle;
			character->GetObjectHandle(handle);
			ObjectProxy<esv::Character>::New(L, handle);
			return 1;
		} else if (projectile != nullptr) {
			ObjectHandle handle;
			projectile->GetObjectHandle(handle);
			ObjectProxy<esv::Projectile>::New(L, handle);
			return 1;
		} else {
			push(L, nullptr);
			return 1;
		}*/
	}
	

	int CreateItemConstructor(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*
		StackCheck _(L, 1);
		auto type = lua_type(L, 1);
		if (type == LUA_TSTRING) {
			auto templateGuid = checked_get<char const*>(L, 1);
			auto constructor = ItemConstructor::New(L);
			if (!script::CreateItemDefinition(templateGuid, constructor->Get())) {
				lua_pop(L, 1);
				push(L, nullptr);
			}
		} else {
			auto item = ObjectProxy<esv::Item>::CheckedGet(L, 1);
			bool recursive{ false };
			if (lua_gettop(L) > 1) {
				recursive = checked_get<bool>(L, 2);
			}

			auto constructor = ItemConstructor::New(L);
			if (!script::ParseItem(item, constructor->Get(), recursive)) {
				lua_pop(L, 1);
				push(L, nullptr);
			}
		}

		return 1;*/
	}
}