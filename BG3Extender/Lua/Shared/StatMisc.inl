#include <stdafx.h>
#include <OsirisProxy.h>
#include <PropertyMaps.h>
#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>
#include <GameDefinitions/Stats/Stats.h>
#include <Lua/Shared/LuaStats.h>

namespace dse::lua
{
	int LuaStatGetAttribute(lua_State* L, CRPGStats_Object* object, char const* attributeName, std::optional<int> level);
	int LuaStatSetAttribute(lua_State* L, CRPGStats_Object* object, char const* attributeName, int valueIdx);


	char const* const SpellPrototypeProxy::MetatableName = "eoc::SpellPrototype";

	SpellPrototypeProxy::SpellPrototypeProxy(SpellPrototype* obj, std::optional<int> level)
		: obj_(obj), level_(level)
	{
		stats_ = obj->GetStats();
	}

	int SpellPrototypeProxy::Index(lua_State* L)
	{
		if (stats_ == nullptr) return luaL_error(L, "Attempted to read property of null spell prototype");
		auto attributeName = luaL_checkstring(L, 2);

		if (strcmp(attributeName, "Level") == 0) {
			push(L, obj_->Level);
			return 1;
		}

		return LuaStatGetAttribute(L, stats_, attributeName, level_);
	}


	char const* const StatsExtraDataProxy::MetatableName = "CRPGStats_ExtraData";

	int StatsExtraDataProxy::Index(lua_State* L)
	{
		auto stats = GetStaticSymbols().GetStats();
		if (stats == nullptr || stats->ExtraData == nullptr) return luaL_error(L, "Stats not available");

		auto key = luaL_checkstring(L, 2);
		auto extraData = stats->ExtraData->Find(FixedString(key));
		if (extraData != nullptr) {
			push(L, *extraData);
		}
		else {
			push(L, nullptr);
		}

		return 1;
	}

	int StatsExtraDataProxy::NewIndex(lua_State* L)
	{
		auto stats = GetStaticSymbols().GetStats();
		if (stats == nullptr || stats->ExtraData == nullptr) return luaL_error(L, "Stats not available");

		auto key = luaL_checkstring(L, 2);
		auto value = checked_get<float>(L, 3);
		auto extraData = stats->ExtraData->Find(FixedString(key));
		if (extraData != nullptr) {
			*extraData = value;
		}
		else {
			LuaError("Cannot set nonexistent ExtraData value '" << key << "'");
		}

		return 0;
	}


	int GetStat(lua_State* L)
	{
		StackCheck _(L, 1);
		auto statName = luaL_checkstring(L, 1);
		std::optional<int> level;
		if (lua_gettop(L) >= 2 && !lua_isnil(L, 2)) {
			level = (int32_t)luaL_checkinteger(L, 2);
		}

		auto object = StatFindObject(statName);
		if (object != nullptr) {
			StatsProxy::New(L, object, level);
			return 1;
		}
		else {
			push(L, nullptr);
			return 1;
		}
	}

	bool CopyStats(CRPGStats_Object* obj, char const* copyFrom)
	{
		auto stats = GetStaticSymbols().GetStats();
		auto copyFromObject = stats->Objects.Find(FixedString(copyFrom));
		if (copyFromObject == nullptr) {
			OsiError("Cannot copy stats from nonexistent object: " << copyFrom);
			return false;
		}

		if (obj->ModifierListIndex != copyFromObject->ModifierListIndex) {
			auto objModifier = stats->ModifierLists.Find(obj->ModifierListIndex);
			auto copyModifier = stats->ModifierLists.Find(obj->ModifierListIndex);
			OsiError("Cannot copy stats from object '" << copyFrom << "' (a " << copyModifier->Name
				<< ") to an object of type " << objModifier->Name);
			return false;
		}

		obj->Level = copyFromObject->Level;
		obj->AIFlags = copyFromObject->AIFlags;

		for (size_t i = 0; i < obj->IndexedProperties.size(); i++) {
			obj->IndexedProperties[i] = copyFromObject->IndexedProperties[i];
		}

		/*copyFromObject->PropertyList.Iterate([obj](auto const& key, auto propertyList) {
			// TODO - is reusing property list objects allowed?
			obj->PropertyList.Insert(key, propertyList);
		});

		copyFromObject->ConditionList.Iterate([obj](auto const& key, auto propertyList) {
			// TODO - is reusing condition objects allowed?
			obj->ConditionList.Insert(key, propertyList);
		});

		obj->Requirements = copyFromObject->Requirements;
		obj->MemorizationRequirements = copyFromObject->MemorizationRequirements;
		obj->ComboCategories = copyFromObject->ComboCategories;*/

		return true;
	}

	int CreateStat(lua_State* L)
	{
		StackCheck _(L, 1);
		auto statName = luaL_checkstring(L, 1);
		auto modifierName = luaL_checkstring(L, 2);
		char const* copyFrom{ nullptr };
		if (lua_gettop(L) >= 3) {
			copyFrom = luaL_checkstring(L, 3);
		}

		LuaVirtualPin lua(gOsirisProxy->GetCurrentExtensionState());
		if (lua->RestrictionFlags & State::ScopeModulePreLoad) {
			return luaL_error(L, "Stat functions unavailable during module preload");
		}

		if (!(lua->RestrictionFlags & State::ScopeModuleLoad)) {
			if (gOsirisProxy->IsInServerThread()) {
				static bool syncWarningShown{ false };
				if (!syncWarningShown) {
					OsiWarn("Stats entres created after ModuleLoad must be synced manually; make sure that you call SyncStat() on it when you're finished!");
					syncWarningShown = true;
				}
			}
			else {
				OsiError("Cannot call CreateStat() on client after module load!");
				push(L, nullptr);
				return 1;
			}
		}

		auto stats = GetStaticSymbols().GetStats();
		auto object = stats->CreateObject(FixedString(statName), FixedString(modifierName));
		if (!object) {
			push(L, nullptr);
			return 1;
		}

		if (copyFrom != nullptr) {
			if (!CopyStats(*object, copyFrom)) {
				push(L, nullptr);
				return 1;
			}
		}

		StatsProxy::New(L, *object, -1);
		return 1;
	}

	void SyncStat(lua_State* L, char const* statName, std::optional<bool> persist)
	{
		auto stats = GetStaticSymbols().GetStats();
		auto object = stats->Objects.Find(FixedString(statName));
		if (!object) {
			OsiError("Cannot sync nonexistent stat: " << statName);
			return;
		}

		//stats->SyncWithPrototypeManager(object);

		if (gOsirisProxy->IsInServerThread()) {
			object->BroadcastSyncMessage(false);

			gOsirisProxy->GetServerExtensionState().MarkDynamicStat(FixedString(statName));
			if (persist && *persist) {
				gOsirisProxy->GetServerExtensionState().MarkPersistentStat(FixedString(statName));
			}
		}
	}

	WrapLuaFunction(SyncStat)

	void StatSetPersistence(lua_State* L, char const* statName, bool persist)
	{
		auto stats = GetStaticSymbols().GetStats();
		auto object = stats->Objects.Find(FixedString(statName));
		if (!object) {
			OsiError("Cannot set persistence for nonexistent stat: " << statName);
			return;
		}

		if (persist) {
			gOsirisProxy->GetServerExtensionState().MarkPersistentStat(FixedString(statName));
		} else {
			gOsirisProxy->GetServerExtensionState().UnmarkPersistentStat(FixedString(statName));
		}
	}

	WrapLuaFunction(StatSetPersistence)

	void RegisterStatsObjects(lua_State* L)
	{
		ObjectProxy<CDivinityStats_Character>::RegisterMetatable(L);
		ObjectProxy<CDivinityStats_CharacterDynamic>::RegisterMetatable(L);
		ObjectProxy<CDivinityStats_Item>::RegisterMetatable(L);
		ObjectProxy<CDivinityStats_Equipment_Attributes>::RegisterMetatable(L);
		StatsExtraDataProxy::RegisterMetatable(L);
		StatsProxy::RegisterMetatable(L);
		SpellPrototypeProxy::RegisterMetatable(L);
	}

	template <class T>
	int EnumIndexToLabel(lua_State* L, T index)
	{
		auto label = EnumInfo<T>::Find(index);
		if (label) {
			push(L, label);
		} else {
			push(L, nullptr);
		}

		return 1;
	}

	template <class T>
	int EnumLabelToIndex(lua_State* L, char const* label)
	{
		auto index = EnumInfo<T>::Find(label);
		if (index) {
			push(L, *index);
		} else {
			push(L, nullptr);
		}

		return 1;
	}

#define BEGIN_BITMASK_NS(NS, T, type)
#define BEGIN_BITMASK(T, type)
#define E(label)
#define EV(label, value)
#define END_ENUM_NS()
#define END_ENUM()

#define BEGIN_ENUM_NS(NS, T, type) \
	if (enumName == GFS.str##T) { \
		return EnumIndexToLabel<NS::T>(L, (NS::T)index); \
	}
#define BEGIN_ENUM(T, type) \
	if (enumName == GFS.str##T) { \
		return EnumIndexToLabel<T>(L, (T)index); \
	}

	int EnumIndexToLabel(lua_State* L)
	{
		auto enumName = checked_get<FixedString>(L, 1);
		auto index = checked_get<int>(L, 2);

#include <GameDefinitions/Enumerations.inl>

		auto valueList = GetStaticSymbols().GetStats()->ModifierValueLists.Find(enumName);
		if (valueList) {
			std::optional<FixedString> value;
			valueList->Values.Iterate([&value, index](auto const& k, auto const& idx) {
				if (idx == index) {
					value = k;
				}
			});

			if (value) {
				push(L, *value);
				return 1;
			} else {
				OsiError("Enumeration '" << enumName << "' has no label with index " << index);
				push(L, nullptr);
				return 1;
			}
		}

		OsiError("No such enumeration: " << enumName);
		push(L, nullptr);
		return 1;
	}

#undef BEGIN_ENUM_NS
#undef BEGIN_ENUM

#define BEGIN_ENUM_NS(NS, T, type) \
	if (enumName == GFS.str##T) { \
		return EnumLabelToIndex<NS::T>(L, label); \
	}
#define BEGIN_ENUM(T, type) \
	if (enumName == GFS.str##T) { \
		return EnumLabelToIndex<T>(L, label); \
	}


	int EnumLabelToIndex(lua_State* L)
	{
		auto enumName = checked_get<FixedString>(L, 1);
		auto label = checked_get<char const*>(L, 2);

#include <GameDefinitions/Enumerations.inl>

		auto valueList = GetStaticSymbols().GetStats()->ModifierValueLists.Find(enumName);
		if (valueList) {
			auto value = valueList->Values.Find(FixedString(label));

			if (value) {
				push(L, *value);
				return 1;
			} else {
				OsiError("Enumeration '" << enumName << "' has no label named '" << label << "'");
				push(L, nullptr);
				return 1;
			}
		}

		OsiError("No such enumeration: " << enumName);
		push(L, nullptr);
		return 1;
	}

#undef BEGIN_BITMASK_NS
#undef BEGIN_ENUM_NS
#undef BEGIN_BITMASK
#undef BEGIN_ENUM
#undef E
#undef EV
#undef END_ENUM_NS
#undef END_ENUM

}
