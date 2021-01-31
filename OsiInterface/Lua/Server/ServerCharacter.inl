#include <GameDefinitions/Character.h>
#include <Lua/LuaBinding.h>

namespace dse::lua
{
	char const* const ObjectProxy<esv::PlayerCustomData>::MetatableName = "esv::PlayerCustomData";

	esv::PlayerCustomData* ObjectProxy<esv::PlayerCustomData>::Get(lua_State* L)
	{
		if (obj_) return obj_;
		auto character = gOsirisProxy->GetServerEntityHelpers().GetComponent<esv::Character>(handle_);
		if (character == nullptr) luaL_error(L, "Character handle invalid");

		if (character->PlayerData == nullptr
			|| !character->PlayerData->CustomData.Initialized) {
			OsiError("Character has no player data, or custom data was not initialized.");
			return nullptr;
		}

		return &character->PlayerData->CustomData;
	}

	int ObjectProxy<esv::PlayerCustomData>::Index(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*return GenericGetter(L, gPlayerCustomDataPropertyMap);*/
	}

	int ObjectProxy<esv::PlayerCustomData>::NewIndex(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*return GenericSetter(L, gPlayerCustomDataPropertyMap);*/
	}

	void GetInventoryItems(lua_State* L, ObjectHandle inventoryHandle)
	{
		luaL_error(L, "Not implemented yet!");

		/*lua_newtable(L);

		auto inventory = esv::FindInventoryByHandle(inventoryHandle);
		if (inventory != nullptr) {
			int32_t index = 1;
			for (auto itemHandle : inventory->ItemsBySlot) {
				if (itemHandle) {
					auto item = esv::GetEntityWorld()->GetItem(itemHandle);
					if (item != nullptr) {
						settable(L, index++, item->MyGuid);
					}
				}
			}
		}*/
	}

	char const* const ObjectProxy<esv::Character>::MetatableName = "esv::Character";

	int ServerCharacterFetchProperty(lua_State* L, esv::Character* character, FixedString const& prop)
	{
		return luaL_error(L, "Not implemented yet!");

		/*if (prop == GFS.strPlayerCustomData) {
			if (character->PlayerData != nullptr
				&& character->PlayerData->CustomData.Initialized) {
				ObjectHandle handle;
				character->GetObjectHandle(handle);
				ObjectProxy<esv::PlayerCustomData>::New(L, handle);
				return 1;
			} else {
				OsiError("Character has no player data, or custom data was not initialized.");
				push(L, nullptr);
				return 1;
			}
		}

		if (prop == GFS.strStats) {
			if (character->Stats != nullptr) {
				ObjectHandle handle;
				character->GetObjectHandle(handle);
				ObjectProxy<CDivinityStats_Character>::New(L, handle);
				return 1;
			} else {
				OsiError("Character has no stats.");
				push(L, nullptr);
				return 1;
			}
		}

		if (prop == GFS.strHandle) {
			push(L, character->Base.Component.Handle);
			return 1;
		}

		if (prop == GFS.strRootTemplate) {
			ObjectProxy<CharacterTemplate>::New(L, character->CurrentTemplate);
			return 1;
		}

		if (prop == GFS.strDisplayName) {
			return GameObjectGetDisplayName<esv::Character>(L, character);
		}

		auto fetched = LuaPropertyMapGet(L, gCharacterPropertyMap, character, prop, true);
		if (!fetched) push(L, nullptr);
		return 1;*/
	}

	esv::Character* ObjectProxy<esv::Character>::Get(lua_State* L)
	{
		if (obj_) return obj_;
		auto character = gOsirisProxy->GetServerEntityHelpers().GetComponent<esv::Character>(handle_);
		if (character == nullptr) luaL_error(L, "Character handle invalid");
		return character;
	}

	int CharacterGetInventoryItems(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<ObjectProxy<esv::Character>*>(L, 1);

		GetInventoryItems(L, self->Get(L)->InventoryHandle);

		return 1;
	}

	int CharacterGetNearbyCharacters(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*StackCheck _(L, 1);
		auto self = checked_get<ObjectProxy<esv::Character>*>(L, 1);
		auto pos = self->Get(L)->WorldPos;
		auto distance = checked_get<float>(L, 2);

		esv::lua::GetCharactersGeneric(L, FixedString{}, [pos, distance](esv::Character* c) {
			return abs(glm::length(pos - c->WorldPos)) < distance;
		});
		return 1;*/
	}

	int CharacterGetSummons(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*StackCheck _(L, 1);
		auto self = checked_get<ObjectProxy<esv::Character>*>(L, 1);

		lua_newtable(L);
		int32_t index{ 1 };

		for (auto const& handle : self->Get(L)->SummonHandles) {
			auto summon = esv::GetEntityWorld()->GetCharacter(handle);
			if ((summon->Flags & esv::CharacterFlags::HasOwner) == esv::CharacterFlags::HasOwner) {
				settable(L, index++, summon->MyGuid);;
			}
		}

		return 1;*/
	}

	int CharacterGetSkills(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*StackCheck _(L, 1);
		auto self = checked_get<ObjectProxy<esv::Character>*>(L, 1);

		lua_newtable(L);
		int32_t index{ 1 };

		auto skillMgr = self->Get(L)->SkillManager;
		if (skillMgr != nullptr) {
			skillMgr->Skills.Iterate([L, &index](FixedString const& skillId, esv::Skill* skill) {
				settable(L, index++, skillId);
			});
		}

		return 1;*/
	}

	int CharacterGetSkillInfo(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*StackCheck _(L, 1);
		auto self = checked_get<ObjectProxy<esv::Character>*>(L, 1);
		auto skillId = checked_get<FixedString>(L, 2);

		auto skillMgr = self->Get(L)->SkillManager;
		if (skillMgr != nullptr) {
			auto skill = skillMgr->Skills.Find(skillId);
			if (skill != nullptr) {
				auto& sk = **skill;
				lua_newtable(L);
				setfield(L, "ActiveCooldown", sk.ActiveCooldown);
				setfield(L, "IsActivated", sk.IsActivated);
				setfield(L, "IsLearned", sk.IsLearned);
				setfield(L, "ZeroMemory", sk.ZeroMemory);
				setfield(L, "OncePerCombat", sk.OncePerCombat);
				setfield(L, "NumCharges", sk.NumCharges);

				lua_newtable(L);
				int32_t causeIdx{ 1 };
				for (auto const& handle : sk.CauseList) {
					settable(L, causeIdx++, handle);
				}

				lua_setfield(L, -2, "CauseList");
				return 1;
			}
		}

		push(L, nullptr);
		return 1;*/
	}

	int ObjectProxy<esv::Character>::Index(lua_State* L)
	{
		auto character = Get(L);
		if (!character) return 0;

		StackCheck _(L, 1);
		auto prop = luaL_checkstring(L, 2);
		FixedString propFS(prop);
		if (!propFS) {
			OsiError("Illegal property name: " << prop);
			lua_pushnil(L);
			return 1;
		}

		if (propFS == GFS.strGetInventoryItems) {
			lua_pushcfunction(L, &CharacterGetInventoryItems);
			return 1;
		}

		if (propFS == GFS.strGetSkills) {
			lua_pushcfunction(L, &CharacterGetSkills);
			return 1;
		}

		if (propFS == GFS.strGetSkillInfo) {
			lua_pushcfunction(L, &CharacterGetSkillInfo);
			return 1;
		}

		if (propFS == GFS.strGetNearbyCharacters) {
			lua_pushcfunction(L, &CharacterGetNearbyCharacters);
			return 1;
		}

		if (propFS == GFS.strGetSummons) {
			lua_pushcfunction(L, &CharacterGetSummons);
			return 1;
		}

		if (propFS == GFS.strHasTag) {
			lua_pushcfunction(L, &GameObjectHasTag<esv::Character>);
			return 1;
		}

		if (propFS == GFS.strGetTags) {
			lua_pushcfunction(L, &GameObjectGetTags<esv::Character>);
			return 1;
		}

		if (propFS == GFS.strGetStatus) {
			lua_pushcfunction(L, (&GameObjectGetStatus<esv::Character, esv::Status>));
			return 1;
		}

		if (propFS == GFS.strGetStatusByType) {
			lua_pushcfunction(L, (&GameObjectGetStatusByType<esv::Character, esv::Status>));
			return 1;
		}

		if (propFS == GFS.strGetStatuses) {
			lua_pushcfunction(L, (&GameObjectGetStatuses<esv::Character>));
			return 1;
		}

		if (propFS == GFS.strGetStatusObjects) {
			lua_pushcfunction(L, (&GameObjectGetStatusObjects<esv::Character, esv::Status>));
			return 1;
		}

		if (propFS == GFS.strSetScale) {
			lua_pushcfunction(L, (&GameObjectSetScale<esv::Character>));
			return 1;
		}

		return ServerCharacterFetchProperty(L, character, propFS);
	}

	int ObjectProxy<esv::Character>::NewIndex(lua_State* L)
	{
		auto character = Get(L);
		if (!character) return 0;

		StackCheck _(L, 0);
		auto prop = luaL_checkstring(L, 2);
		FixedString propFS(prop);
		if (!propFS) {
			OsiError("Illegal property name: " << prop);
			return 0;
		}

		return luaL_error(L, "Not implemented yet!");

		/*if (propFS == GFS.strWalkSpeed) {
			if (lua_isnil(L, 3)) {
				character->WalkSpeedOverride = 0.0f;
				character->Flags3 &= ~esv::CharacterFlags3::HasWalkSpeedOverride;
			} else {
				auto speed = checked_get<float>(L, 3);
				character->WalkSpeedOverride = speed;
				character->Flags3 |= esv::CharacterFlags3::HasWalkSpeedOverride;
			}
		} else if (propFS == GFS.strRunSpeed) {
			if (lua_isnil(L, 3)) {
				character->RunSpeedOverride = 0.0f;
				character->Flags3 &= ~esv::CharacterFlags3::HasRunSpeedOverride;
			}
			else {
				auto speed = checked_get<float>(L, 3);
				character->RunSpeedOverride = speed;
				character->Flags3 |= esv::CharacterFlags3::HasRunSpeedOverride;
			}
		} else {
			return GenericSetter(L, gCharacterPropertyMap);
		}

		return 0;*/
	}
}


namespace dse::esv::lua
{
	using namespace dse::lua;

	esv::Character* GetCharacter(lua_State* L, int index)
	{
		esv::Character* character = nullptr;
		switch (lua_type(L, index)) {
		case LUA_TLIGHTUSERDATA:
		{
			auto handle = checked_get<ObjectHandle>(L, index);
			character = gOsirisProxy->GetServerEntityHelpers().GetComponent<esv::Character>(handle);
			break;
		}

		case LUA_TNUMBER:
		{
			auto value = lua_tointeger(L, index);
			NetId netId{ (uint32_t)value };
			character = gOsirisProxy->GetServerEntityHelpers().GetComponent<esv::Character>(netId);
			break;
		}

		case LUA_TSTRING:
		{
			auto guid = lua_tostring(L, index);
			character = gOsirisProxy->GetServerEntityHelpers().GetComponent<esv::Character>(guid);
			break;
		}

		default:
			OsiError("Expected character UUID, Handle or NetId; got " << lua_typename(L, lua_type(L, 1)));
			break;
		}

		return character;
	}

	int GetCharacter(lua_State* L)
	{
		LuaServerPin lua(esv::ExtensionState::Get());
		if (lua->RestrictionFlags & State::RestrictHandleConversion) {
			return luaL_error(L, "Attempted to resolve character handle in restricted context");
		}

		StackCheck _(L, 1);
		esv::Character* character = GetCharacter(L, 1);

		if (character != nullptr) {
			ObjectHandle handle;
			character->GetObjectHandle(handle);
			ObjectProxy<esv::Character>::New(L, handle);
			return 1;
		} else {
			push(L, nullptr);
			return 1;
		}
	}
}
