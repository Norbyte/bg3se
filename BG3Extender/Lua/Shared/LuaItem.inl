#include <OsirisProxy.h>
#include <PropertyMaps.h>
#include <GameDefinitions/Stats/Item.h>
#include <Lua/LuaSerializers.h>

namespace bg3se::lua
{
	int ItemFetchStat(lua_State* L, CDivinityStats_Item* item, char const* prop)
	{
		/*if (strcmp(prop, "DynamicStats") == 0) {
			lua_newtable(L);
			unsigned statIdx = 1;
			for (auto dynamicStat : item->DynamicAttributes) {
				push(L, statIdx++);
				ObjectProxy<CDivinityStats_Equipment_Attributes>::New(L, dynamicStat);
				lua_settable(L, -3);
			}

			return 1;
		}

		if (strncmp(prop, "TALENT_", 7) == 0) {
			auto talentId = EnumInfo<TalentType>::Find(prop + 7);
			if (talentId) {
				bool hasTalent = item->HasTalent(*talentId);
				push(L, hasTalent);
				return 1;
			}
			else {
				return 0;
			}
		}

		auto abilityId = EnumInfo<AbilityType>::Find(prop);
		if (abilityId) {
			int abilityLevel = item->GetAbility(*abilityId);
			push(L, abilityLevel);
			return 1;
		}

		auto fetched = LuaPropertyMapGet(L, gItemStatsPropertyMap, item, prop, false);
		if (fetched) {
			return 1;
		}*/

		return stats::LuaStatGetAttribute(L, item, prop, {});
	}


	char const* const ObjectProxy<CDivinityStats_Item>::MetatableName = "CDivinityStats_Item";

	CDivinityStats_Item* ObjectProxy<CDivinityStats_Item>::Get(lua_State* L)
	{
		if (obj_) return obj_;
		auto item = gOsirisProxy->GetServerEntityHelpers().GetComponent<esv::Item>(handle_);
		if (item == nullptr) luaL_error(L, "Item handle invalid");
		luaL_error(L, "Not implemented yet!"); 
		return nullptr;
		/*if (item->Stats == nullptr) luaL_error(L, "Item has no stats!");
		return item->Stats;*/
	}

	int ObjectProxy<CDivinityStats_Item>::Index(lua_State* L)
	{
		auto stats = Get(L);
		if (!stats) return 0;


		auto prop = luaL_checkstring(L, 2);
		return ItemFetchStat(L, stats, prop);
	}

	int ObjectProxy<CDivinityStats_Item>::NewIndex(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*return GenericSetter(L, gItemStatsPropertyMap);*/
	}


	char const* const ObjectProxy<CDivinityStats_Equipment_Attributes>::MetatableName = "CDivinityStats_Equipment_Attributes";

	CDivinityStats_Equipment_Attributes* ObjectProxy<CDivinityStats_Equipment_Attributes>::Get(lua_State* L)
	{
		if (obj_) return obj_;
		luaL_error(L, "Equipment stats no longer available");
		return nullptr;
	}

	int ObjectProxy<CDivinityStats_Equipment_Attributes>::Index(lua_State* L)
	{
		auto stats = Get(L);
		if (!stats) return 0;

		return luaL_error(L, "Not implemented yet!");

		/*return GenericGetter(L, stats->GetPropertyMap());*/
	}

	int ObjectProxy<CDivinityStats_Equipment_Attributes>::NewIndex(lua_State* L)
	{
		auto stats = Get(L);
		if (!stats) return 0;

		return luaL_error(L, "Not implemented yet!");

		/*return GenericSetter(L, stats->GetPropertyMap());*/
	}


	/*ItemOrCharacterPushPin::ItemOrCharacterPushPin(lua_State* L, CRPGStats_Object* obj)
	{
		if (obj == nullptr) {
			lua_pushnil(L);
		}
		else if (obj->ModifierListIndex == GetStaticSymbols().GetStats()->modifierList.FindIndex(ToFixedString("Character"))) {
			auto ch = reinterpret_cast<CDivinityStats_Character*>(obj);
			character_ = ObjectProxy<CDivinityStats_Character>::New(L, ch);
		}
		else if (obj->ModifierListIndex == GetStaticSymbols().GetStats()->modifierList.FindIndex(ToFixedString("Item"))) {
			auto item = reinterpret_cast<CDivinityStats_Item*>(obj);
			item_ = ObjectProxy<CDivinityStats_Item>::New(L, item);
		}
		else {
			object_ = StatsProxy::New(L, obj, -1);
			OsiWarnS("Could not determine stats type of object");
		}
	}

	ItemOrCharacterPushPin::~ItemOrCharacterPushPin()
	{
		if (character_) character_->Unbind();
		if (item_) item_->Unbind();
		if (object_) object_->Unbind();
	}*/

}
