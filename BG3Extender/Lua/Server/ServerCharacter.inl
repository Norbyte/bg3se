#include <GameDefinitions/Character.h>
#include <Lua/LuaBinding.h>

namespace bg3se::lua
{
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

	int CharacterGetInventoryItems(lua_State* L, esv::Character* self)
	{
		StackCheck _(L, 1);
		GetInventoryItems(L, self->InventoryHandle);
		return 1;
	}

	int CharacterGetNearbyCharacters(lua_State* L, esv::Character* self)
	{
		return luaL_error(L, "Not implemented yet!");

		/*StackCheck _(L, 1);
		auto pos = self->Get(L)->WorldPos;
		auto distance = checked_get<float>(L, 2);

		esv::lua::GetCharactersGeneric(L, FixedString{}, [pos, distance](esv::Character* c) {
			return abs(glm::length(pos - c->WorldPos)) < distance;
		});
		return 1;*/
	}

	int CharacterGetSummons(lua_State* L, esv::Character* self)
	{
		return luaL_error(L, "Not implemented yet!");

		/*StackCheck _(L, 1);

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

	int CharacterGetSkills(lua_State* L, esv::Character* self)
	{
		return luaL_error(L, "Not implemented yet!");

		/*StackCheck _(L, 1);

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

	int CharacterGetSkillInfo(lua_State* L, esv::Character* self)
	{
		return luaL_error(L, "Not implemented yet!");

		/*StackCheck _(L, 1);
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
}


namespace bg3se::esv::lua
{
	using namespace bg3se::lua;

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
			ObjectProxy2<esv::Character>::New(L, character);
			return 1;
		} else {
			push(L, nullptr);
			return 1;
		}
	}
}
