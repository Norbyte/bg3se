#include <optional>
#include <GameDefinitions/Stats/Character.h>
#include <Lua/LuaBinding.h>

namespace bg3se::lua
{
	char const* const ObjectProxy<CDivinityStats_Character>::MetatableName = "CDivinityStats_Character";

	int CharacterFetchStat(lua_State* L, CDivinityStats_Character* stats, char const* propStr, FixedString const& prop)
	{
		if (prop == GFS.strSight) {
			push(L, stats->Sight);
			return 1;
		}

		/* FIXME
		std::optional<int32_t> dynamicStat;
		if (!prop && strncmp(propStr, "Base", 4) == 0) {
			dynamicStat = stats->GetStat(ToFixedString(propStr + 4), true);
		}
		else {
			dynamicStat = stats->GetStat(prop, false);
		}

		if (dynamicStat) {
			push(L, *dynamicStat);
			return 1;
		}*/

		if (prop == GFS.strModId) {
			push(L, gOsirisProxy->GetStatLoadOrderHelper().GetStatsEntryMod(stats->Name));
			return 1;
		}

		if (prop == GFS.strDynamicStats) {
			lua_newtable(L);
			unsigned statIdx = 1;
			for (auto dynamicStat : stats->DynamicStats) {
				push(L, statIdx++);
				ObjectProxy<CDivinityStats_CharacterDynamic>::New(L, dynamicStat);
				lua_settable(L, -3);
			}

			return 1;
		}

		/* FIXME
		if (prop == GFS.strMainWeapon) {
			auto weapon = stats->GetMainWeapon();
			if (weapon != nullptr) {
				ObjectProxy<CDivinityStats_Item>::New(L, weapon);
				return 1;
			}
			else {
				return 0;
			}
		}

		if (prop == GFS.strOffHandWeapon) {
			auto weapon = stats->GetOffHandWeapon();
			if (weapon != nullptr) {
				ObjectProxy<CDivinityStats_Item>::New(L, weapon);
				return 1;
			}
			else {
				return 0;
			}
		}

		if (strncmp(propStr, "TALENT_", 7) == 0) {
			auto talentId = EnumInfo<TalentType>::Find(propStr + 7);
			if (talentId) {
				bool hasTalent = stats->HasTalent(*talentId, false);
				push(L, hasTalent);
				return 1;
			}
			else {
				return 0;
			}
		}*/

		if (prop == GFS.strNotSneaking) {
			push(L, (bool)(stats->Flags & StatCharacterFlags::IsSneaking));
			return 1;
		}

		/* FIXME
		auto abilityId = EnumInfo<AbilityType>::Find(prop);
		if (abilityId) {
			int abilityLevel = stats->GetAbility(*abilityId, false);
			push(L, abilityLevel);
			return 1;
		}

		auto fetched = LuaPropertyMapGet(L, gCharacterStatsPropertyMap, stats, prop, false);
		if (fetched) {
			return 1;
		}

		if (stats->Character != nullptr) {
			if (prop == GFS.strCharacter) {
				ObjectHandle handle;
				stats->Character->GetObjectHandle(handle);
				if (handle.GetType() == (uint32_t)ObjectType::ClientCharacter) {
					ObjectProxy<ecl::Character>::New(L, handle);
				}
				else if (handle.GetType() == (uint32_t)ObjectType::ServerCharacter) {
					ObjectProxy<esv::Character>::New(L, handle);
				}
				else {
					ERR("Unknown character handle type: %d", handle.GetType());
					push(L, nullptr);
				}
				return 1;
			}

			if (prop == GFS.strRotation) {
				push(L, *stats->Character->GetRotation());
				return 1;
			}

			if (prop == GFS.strPosition) {
				push(L, *stats->Character->GetTranslate());
				return 1;
			}

			if (prop == GFS.strMyGuid) {
				push(L, *stats->Character->GetGuid());
				return 1;
			}

			if (prop == GFS.strNetID) {
				push(L, stats->Character->NetID);
				return 1;
			}
		}
		*/

		OsiError("Unknown character stats property: " << prop);
		return 0;
	}

	CDivinityStats_Character* ObjectProxy<CDivinityStats_Character>::Get(lua_State* L)
	{
		throw std::runtime_error("Not implemented!");

		/*if (obj_) return obj_;
		auto character = gOsirisProxy->GetServerEntityHelpers().GetComponent<esv::Character>(handle_);
		if (character == nullptr) luaL_error(L, "Character handle invalid");
		if (character->Stats == nullptr) luaL_error(L, "Character has no stats!");
		return character->Stats;*/
	}

	int CharacterGetItemBySlot(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*auto self = checked_get<ObjectProxy<CDivinityStats_Character>*>(L, 1);
		auto slot = checked_get<ItemSlot>(L, 2);

		auto item = self->Get(L)->GetItemBySlot(slot, true);
		if (item != nullptr) {
			ObjectProxy<CDivinityStats_Item>::New(L, item);
			return 1;
		} else {
			return 0;
		}*/
	}

	int ObjectProxy<CDivinityStats_Character>::Index(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*auto stats = Get(L);
		if (!stats) return 0;

		auto prop = luaL_checkstring(L, 2);
		auto fs = ToFixedString(prop);

		if (fs == GFS.strGetItemBySlot) {
			lua_pushcfunction(L, &CharacterGetItemBySlot);
			return 1;
		}

		return CharacterFetchStat(L, stats, prop, fs);*/
	}

	int ObjectProxy<CDivinityStats_Character>::NewIndex(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*return GenericSetter(L, gCharacterStatsPropertyMap);*/
	}


	char const* const ObjectProxy<CDivinityStats_CharacterDynamic>::MetatableName = "CharacterDynamicStat";

	CDivinityStats_CharacterDynamic* ObjectProxy<CDivinityStats_CharacterDynamic>::Get(lua_State* L)
	{
		if (obj_) return obj_;
		luaL_error(L, "Character stats no longer available");
		return nullptr;
	}

	int ObjectProxy<CDivinityStats_CharacterDynamic>::Index(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*return GenericGetter(L, gCharacterDynamicStatPropertyMap);*/
	}

	int ObjectProxy<CDivinityStats_CharacterDynamic>::NewIndex(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*return GenericSetter(L, gCharacterDynamicStatPropertyMap);*/
	}
}
