#include <Lua/Shared/LuaStats.h>

/// <lua_module>Stats</lua_module>
BEGIN_NS(lua::stats)

using namespace bg3se::stats;

stats::RPGStats* GetStatsManager()
{
    return GetStaticSymbols().GetStats();
}

/*Array<FixedString> FetchSkillSetEntries(RPGStats* stats)
{
    Array<FixedString> names;
    for (auto skillSet : stats->SkillSetManager->Elements) {
        names.push_back(skillSet->Name);
    }

    return names;
}

Array<FixedString> FetchItemComboEntries(RPGStats* stats)
{
    ObjectSet<FixedString> names;
    for (auto itemCombo : stats->ItemCombinationManager->Elements) {
        names.push_back(itemCombo->Name);
    }

    return names;
}

Array<FixedString> FetchItemComboPropertyEntries(RPGStats* stats)
{
    ObjectSet<FixedString> names;
    for (auto const& combo : stats->ItemCombinationManager->ComboProperties) {
        names.push_back(combo.Key);
    }

    return names;
}

Array<FixedString> FetchItemComboPreviewDataEntries(RPGStats* stats)
{
    Array<FixedString> names;
    for (auto const& preview : stats->ItemCombinationManager->PreviewData) {
        names.push_back(preview.Key);
    }

    return names;
}

Array<FixedString> FetchItemGroupEntries(RPGStats* stats)
{
    Array<FixedString> names;
    for (auto const& group : stats->ItemProgressionManager->ItemGroups) {
        names.push_back(group.Key);
    }

    return names;
}

Array<FixedString> FetchItemNameGroupEntries(RPGStats* stats)
{
    Array<FixedString> names;
    for (auto const& group : stats->ItemProgressionManager->NameGroups) {
        names.push_back(group.Key);
    }

    return names;
}*/


/*Array<FixedString> FetchEquipmentSetEntries(RPGStats* stats)
{
    Array<FixedString> names;
    for (auto equipmentSet : stats->EquipmentSetManager->Elements) {
        names.push_back(equipmentSet->Name);
    }

    return names;
}*/

Array<FixedString> FetchTreasureTableEntries(RPGStats* stats)
{
    Array<FixedString> names;
    for (auto treasureTable : stats->TreasureTables.Values) {
        names.push_back(treasureTable->Name);
    }

    return names;
}

Array<FixedString> FetchTreasureCategoryEntries(RPGStats* stats)
{
    Array<FixedString> names;
    for (auto treasureCategory : stats->TreasureCategories.Values) {
        names.push_back(treasureCategory->Category);
    }

    return names;
}

Array<FixedString> FetchStatEntries(RPGStats * stats, FixedString const& statType)
{
    ModifierList* modifierList{ nullptr };
    if (statType) {
        modifierList = stats->ModifierLists.GetByName(statType);
        if (modifierList == nullptr) {
            OsiError("Unknown stats entry type: " << statType);
            return {};
        }
    }

    Array<FixedString> names;
    for (auto object : stats->Objects.Values) {
        if (statType) {
            auto type = stats->GetTypeInfo(object);
            if (modifierList != nullptr && type != modifierList) {
                continue;
            }
        }

        names.push_back(object->Name);
    }

    return names;
}

Array<FixedString> FetchStatEntriesBefore(RPGStats* stats, FixedString const& modId, std::optional<FixedString> statType)
{
    ModifierList* modifierList{ nullptr };
    if (statType) {
        modifierList = stats->ModifierLists.GetByName(*statType);
        if (modifierList == nullptr) {
            OsiError("Unknown stats entry type: " << *statType);
            return {};
        }
    }

    auto entries = gExtender->GetStatLoadOrderHelper().GetStatsLoadedBefore(modId);

    Array<FixedString> names;
    for (auto object : entries) {
        if (statType) {
            auto type = stats->GetTypeInfo(object);
            if (modifierList != nullptr && type != modifierList) {
                continue;
            }
        }

        names.push_back(object->Name);
    }

    return names;
}

/// <summary>
/// Returns a table with the names of all stat entries. 
/// When the optional parameter `statType` is specified, it'll only return stats with the specified type. (The type of a stat entry is specified in the stat .txt file itself (eg. `type "StatusData"`).
/// The following types are supported:  `StatusData`, `SkillData`, `Armor`, `Shield`, `Weapon`, `Potion`, `Character`, `Object`, `SkillSet`, `EquipmentSet`, `TreasureTable`, `ItemCombination`, `ItemComboProperty`, `CraftingPreviewData`, `ItemGroup`, `NameGroup`, `DeltaMod`
/// </summary>
/// <lua_export>GetStats</lua_export>
/// <param name="statType">Type of stat to fetch</param>
/// <returns></returns>
Array<FixedString> GetStats(std::optional<FixedString> statType)
{
    OPTICK_EVENT();
    auto stats = GetStaticSymbols().GetStats();
    if (stats == nullptr) {
        OsiError("RPGStats not available");
        return {};
    }

    Array<FixedString> names;
    if (!statType) {
        names = FetchStatEntries(stats, FixedString{});
    /* } else if (*statType == GFS.strSkillSet) {
        names = FetchSkillSetEntries(stats);
    } else if (*statType == GFS.strEquipmentSet) {
        names = FetchEquipmentSetEntries(stats);*/
    } else if (*statType == GFS.strTreasureTable) {
        names = FetchTreasureTableEntries(stats);
    } else if (*statType == GFS.strTreasureCategory) {
        names = FetchTreasureCategoryEntries(stats);
    /* } else if (*statType == GFS.strItemCombination) {
        names = FetchItemComboEntries(stats);
    } else if (*statType == GFS.strItemComboProperty) {
        names = FetchItemComboPropertyEntries(stats);
    } else if (*statType == GFS.strCraftingPreviewData) {
        names = FetchItemComboPreviewDataEntries(stats);
    } else if (*statType == GFS.strItemGroup) {
        names = FetchItemGroupEntries(stats);
    } else if (*statType == GFS.strNameGroup) {
        names = FetchItemNameGroupEntries(stats);*/
    } else {
        names = FetchStatEntries(stats, *statType);
    }

    return names;
}

/// <summary>
/// Returns a table with the names of all stat entries that were loaded before the specified mod.
/// This function is useful for retrieving stats that can be overridden by a mod according to the module load order.
/// When the optional parameter `statType` is specified, it'll only return stats with the specified type. (The type of a stat entry is specified in the stat .txt file itself (eg. `type "StatusData"`).
/// </summary>
/// <lua_export>GetStatsLoadedBefore</lua_export>
/// <lua_legacy>Ext.GetStatEntriesLoadedBefore</lua_legacy>
/// <param name="modUuid">Return stats entries declared before this module was loaded</param>
/// <param name="statType">Type of stat to fetch</param>
/// <returns></returns>
Array<FixedString> GetStatsLoadedBefore(FixedString const& modUuid, std::optional<FixedString> statType)
{
    OPTICK_EVENT();
    auto stats = GetStaticSymbols().GetStats();
    if (stats == nullptr) {
        OsiError("RPGStats not available");
        return {};
    }

    return FetchStatEntriesBefore(stats, modUuid, statType);
}
/*
ByValReturn<SkillSet> GetSkillSet(char const* skillSetName)
{
    auto stats = GetStaticSymbols().GetStats();
    return stats->SkillSetManager->Find(skillSetName);
}

void UpdateSkillSet(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    auto name = checked_getfield<FixedString>(L, "Name", 1);

    auto stats = GetStaticSymbols().GetStats();
    auto skillSet = stats->SkillSetManager->Find(name);
    bool isNew = (skillSet == nullptr);

    lua_pushvalue(L, 1);
    LuaRead(L, skillSet);
    lua_pop(L, 1);

    if (isNew) {
        stats->SkillSetManager->Add(name, skillSet);
    }
}

UserReturn GetEquipmentSet(lua_State * L, FixedString const& equipmentSetName)
{
    auto stats = GetStaticSymbols().GetStats();
    auto equipmentSet = stats->EquipmentSetManager->Find(equipmentSetName);
    return LuaWrite(L, equipmentSet);
}

void UpdateEquipmentSet(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    auto name = checked_getfield<FixedString>(L, "Name", 1);

    auto stats = GetStaticSymbols().GetStats();
    auto equipmentSet = stats->EquipmentSetManager->Find(name);
    bool isNew = (equipmentSet == nullptr);

    lua_pushvalue(L, 1);
    LuaRead(L, equipmentSet);
    lua_pop(L, 1);

    if (isNew) {
        stats->EquipmentSetManager->Add(name, equipmentSet);
    }
}*/

UserReturn GetTreasureTableLegacy(lua_State* L, FixedString const& tableName)
{
    auto stats = GetStaticSymbols().GetStats();
    auto table = stats->TreasureTables.GetByName(tableName);
    if (table) {
        return LuaWrite(L, table);
    } else {
        push(L, nullptr);
        return 1;
    }
}

stats::TreasureTable* GetTreasureTable(FixedString const& tableName)
{
    auto stats = GetStaticSymbols().GetStats();
    return stats->TreasureTables.GetByName(tableName);
}

void UpdateTreasureTable(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    auto name = checked_getfield<FixedString>(L, "Name", 1);

    auto stats = GetStaticSymbols().GetStats();
    auto table = stats->TreasureTables.GetByName(name);
    bool isNew = (table == nullptr);

    lua_pushvalue(L, 1);
    LuaRead(L, table);
    lua_pop(L, 1);

    if (isNew) {
        stats->TreasureTables.Insert(table);
    }
}

UserReturn GetTreasureCategory(lua_State* L, FixedString const& categoryName)
{
    auto const* stats = GetStaticSymbols().GetStats();
    auto category = stats->TreasureCategories.GetByName(categoryName);
    if (category) {
        return LuaWrite(L, category);
    } else {
        push(L, nullptr);
        return 1;
    }
}

void UpdateTreasureCategory(lua_State* L, FixedString const& name)
{
    luaL_checktype(L, 2, LUA_TTABLE);

    auto stats = GetStaticSymbols().GetStats();
    auto category = stats->TreasureCategories.GetByName(name);
    bool isNew = (category == nullptr);

    lua_pushvalue(L, 2);
    LuaRead(L, category);
    lua_pop(L, 1);

    if (isNew) {
        stats->TreasureCategories.Insert(category);
    }
}

/*UserReturn GetItemCombo(lua_State* L, FixedString const& comboName)
{
    auto combo = GetStaticSymbols().GetStats()->ItemCombinationManager->Find(comboName);
    return LuaWrite(L, combo);
}

void UpdateItemCombo(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    auto name = checked_getfield<FixedString>(L, "Name", 1);

    auto stats = GetStaticSymbols().GetStats();
    auto combo = stats->ItemCombinationManager->Find(name);
    bool isNew = (combo == nullptr);

    lua_pushvalue(L, 1);
    LuaRead(L, combo);
    lua_pop(L, 1);

    if (isNew) {
        stats->ItemCombinationManager->Add(name, combo);
    }
}

UserReturn GetItemComboPreviewData(lua_State* L, FixedString const& comboName)
{
    auto preview = GetStaticSymbols().GetStats()->ItemCombinationManager->PreviewData.try_get(comboName);
    return LuaWrite(L, preview);
}

void UpdateItemComboPreviewData(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    auto name = checked_getfield<FixedString>(L, "Name", 1);

    auto stats = GetStaticSymbols().GetStats();
    auto existing = stats->ItemCombinationManager->PreviewData.try_get(name);
    ItemCombinationPreviewData* previewData = existing ? existing : nullptr;
    bool isNew = (previewData == nullptr);

    lua_pushvalue(L, 1);
    LuaRead(L, previewData);
    lua_pop(L, 1);

    if (isNew) {
        stats->ItemCombinationManager->PreviewData.insert(name, previewData);
    }
}

UserReturn GetItemComboProperty(lua_State* L, FixedString const& propertyName)
{
    auto prop = GetStaticSymbols().GetStats()->ItemCombinationManager->ComboProperties.try_get(propertyName);
    return LuaWrite(L, prop);
}

void UpdateItemComboProperty(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    auto name = checked_getfield<FixedString>(L, "Name", 1);

    auto stats = GetStaticSymbols().GetStats();
    auto existing = stats->ItemCombinationManager->ComboProperties.try_get(name);
    ItemCombinationProperty* comboProperty = existing ? existing : nullptr;
    bool isNew = (comboProperty == nullptr);

    lua_pushvalue(L, 1);
    LuaRead(L, comboProperty);
    lua_pop(L, 1);

    if (isNew) {
        stats->ItemCombinationManager->ComboProperties.insert(name, comboProperty);
    }
}

UserReturn GetItemGroup(lua_State* L, FixedString const& name)
{
    auto group = GetStaticSymbols().GetStats()->ItemProgressionManager->ItemGroups.try_get(name);
    return LuaWrite(L, group);
}


UserReturn GetNameGroup(lua_State* L, FixedString const& name)
{
    auto nameGroup = GetStaticSymbols().GetStats()->ItemProgressionManager->NameGroups.try_get(name);
    return LuaWrite(L, nameGroup);
}

UserReturn GetItemSet(lua_State* L, FixedString const& itemSetName)
{
    auto itemSet = GetStaticSymbols().GetStats()->ItemSetsManager->Find(itemSetName);
    return LuaWrite(L, itemSet);
}

void UpdateItemSet(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    auto name = checked_getfield<FixedString>(L, "Name", 1);

    auto stats = GetStaticSymbols().GetStats();
    auto itemSet = stats->ItemSetsManager->Find(name);
    bool isNew = (itemSet == nullptr);

    lua_pushvalue(L, 1);
    LuaRead(L, itemSet);
    lua_pop(L, 1);

    if (isNew) {
        stats->ItemSetsManager->Add(name, itemSet);
    }
}

LegacyRefMap<FixedString, ItemColorDefinition>* GetAllItemColors()
{
    return &GetStaticSymbols().GetStats()->Colors;
}

ItemColorDefinition* GetItemColor(lua_State* L, FixedString const& colorName)
{
    return GetStaticSymbols().GetStats()->Colors.try_get_ptr(colorName);
}

void UpdateItemColor(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    auto name = checked_getfield<FixedString>(L, "Name", 1);

    auto stats = GetStaticSymbols().GetStats();
    auto itemColor = stats->Colors.try_get_ptr(name);
    bool isNew = (itemColor == nullptr);

    lua_pushvalue(L, 1);
    LuaRead(L, itemColor);
    lua_pop(L, 1);

    if (isNew) {
        stats->Colors.insert(name, *itemColor);
    }
}*/

/// <summary>
/// Returns the specified stats entry as an object for easier manipulation.
/// If the `level` argument is not specified or is `nil`, the table will contain stat values as specified in the stat entry.
/// If the `level` argument is not `nil`, the table will contain level - scaled values for the specified level.
/// A `level` value of `-1` will use the level specified in the stat entry.
/// 
/// The behavior of getting a table entry is identical to that of `StatGetAttribute` and setting a table entry is identical to `StatSetAttribute`.
/// 
/// The `StatSetAttribute` example rewritten using `GetStat`:
/// ```lua
/// -- Swap DamageType from Poison to Air on all skills
/// for i, name in pairs(Ext.Stats.GetEntries("SkillData")) do
///     local stat = Ext.Stats.Get(name)
///     if stat.DamageType == "Poison" then
///         stat.DamageType = "Air"
///     end
/// end
/// ```
/// </summary>
/// <lua_export>Get</lua_export>
/// <lua_legacy>Ext.GetGet</lua_legacy>
/// <param name="statName">Stats name to fetch</param>
/// <param name="level">Specify `nil` to return raw (unscaled) stat values, `-1` to return values scaled to the stats level, or a specific level value to scale returned stats to that level</param>
/// <param name="warnOnError">Log a warning in the console if the stats object could not be found?</param>
/// <param name="byRef">Specifies whether the returned object should use by-value or by-ref properties (default: by-value)</param>
/// <returns></returns>
Object* Get(lua_State * L, char const* statName, std::optional<int> level, std::optional<bool> warnOnError, std::optional<bool> byRef)
{
    return StatFindObject(statName, warnOnError.value_or(false));
}

SpellPrototype* GetCachedSpell(lua_State * L, FixedString name)
{
    auto proto = (*GetStaticSymbols().eoc__SpellPrototypeManager)->Spells.try_get(name);
    return proto ? *proto : nullptr;
}

StatusPrototype* GetCachedStatus(lua_State * L, FixedString name)
{
    auto proto = (*GetStaticSymbols().eoc__StatusPrototypeManager)->Statuses.try_get(name);
    return proto ? *proto : nullptr;
}

PassivePrototype* GetCachedPassive(lua_State * L, FixedString name)
{
    return (*GetStaticSymbols().eoc__PassivePrototypeManager)->Passives.try_get_ptr(name);
}

InterruptPrototype* GetCachedInterrupt(lua_State * L, FixedString name)
{
    return (*GetStaticSymbols().eoc__InterruptPrototypeManager)->Interrupts.try_get(name);
}

bool CopyStats(Object* obj, FixedString const& copyFrom)
{
    auto stats = GetStaticSymbols().GetStats();
    auto copyFromObject = stats->Objects.GetByName(copyFrom);
    if (copyFromObject == nullptr) {
        OsiError("Cannot copy stats from nonexistent object: " << copyFrom);
        return false;
    }

    return obj->CopyFrom(copyFromObject);
}

/// <summary>
/// Creates a new stats entry. 
/// If a stat object with the same name already exists, the specified modifier type is invalid or the specified template doesn't exist, the function returns `nil`.
/// After all stat properties were initialized, the stats entry must be synchronized by calling `SyncStat()`.
/// 
/// - If the entry was created on the server, `SyncStat()` will replicate the stats entry to all clients.If the entry was created on the client, `SyncStat()` will only update it locally.
/// Example:
/// ```lua
/// local stat = Ext.Stats.Create("NRD_Dynamic_Skill", "SkillData", "Rain_Water")
/// stat.RainEffect = "RS3_FX_Environment_Rain_Fire_01"
/// stat.SurfaceType = "Fire"
/// Ext.Stats.Sync("NRD_Dynamic_Skill")
/// ```
/// </summary>
/// <lua_export>Create</lua_export>
/// <lua_legacy>Ext.CreateStat</lua_legacy>
/// <param name="statName">Name of stats entry to create; it should be globally unique</param>
/// <param name="modifierList">Stats entry type (eg. `SkillData`, `StatusData`, `Weapon`, etc.)</param>
/// <param name="copyFromTemplate">If this parameter is not `nil`, stats properties are copied from the specified stats entry to the newly created entry</param>
/// <param name="byRef">Specifies whether the created object should use by-value or by-ref properties (default: by-value)</param>
/// <returns>stats::Object</returns>
Object* Create(lua_State * L, FixedString const& statName, FixedString const& modifierList, std::optional<FixedString> copyFromTemplate, std::optional<bool> byRef)
{
    auto lua = State::FromLua(L);
    if (lua->RestrictionFlags & State::ScopeModulePreLoad) {
        luaL_error(L, "Stat functions unavailable during module preload");
        return nullptr;
    }

    if (!(lua->RestrictionFlags & State::ScopeModuleLoad)) {
        if (gExtender->GetServer().IsInContext()) {
            static bool syncWarningShown{ false };
            if (!syncWarningShown) {
                OsiWarn("Stats entres created after ModuleLoad must be synced manually; make sure that you call SyncStat() on it when you're finished!");
                syncWarningShown = true;
            }
        }
    }

    auto stats = GetStaticSymbols().GetStats();
    auto object = stats->CreateObject(statName, modifierList);
    if (!object) {
        return nullptr;
    }

    if (copyFromTemplate) {
        if (!CopyStats(*object, *copyFromTemplate)) {
            return nullptr;
        }
    }

    return *object;
}

/// <summary>
/// Synchronizes the changes made to the specified stats entry with the cached prototype manager.
/// `Sync` must be called each time a stats entry is modified dynamically.
/// </summary>
/// <lua_export>Sync</lua_export>
/// <param name="statName">Name of stats entry to sync</param>
void Sync(FixedString const& statName, std::optional<bool> persist)
{
    auto stats = GetStaticSymbols().GetStats();
    auto object = stats->Objects.GetByName(statName);
    if (!object) {
        OsiError("Cannot sync nonexistent stat: " << statName);
        return;
    }

    stats->SyncWithPrototypeManager(object);

    if (persist) {
        WARN_ONCE("The 'persist' argument to Ext.Stats.Sync() is deprecated");
    }
}

void SetPersistence(FixedString const& statName, bool persist)
{
    WARN_ONCE("Ext.Stats.SetPersistence() is deprecated");
}

std::optional<FixedString> EnumIndexToLabel(FixedString const& enumName, int index)
{
    auto enumInfo = EnumRegistry::Get().EnumsByName.find(enumName);
    if (enumInfo != EnumRegistry::Get().EnumsByName.end()) {
        auto value = enumInfo.Value()->Find((EnumUnderlyingType)index);
        if (value) {
            return value;
        } else {
            return {};
        }
    }
    
    auto bitfieldInfo = BitfieldRegistry::Get().BitfieldsByName.find(enumName);
    if (bitfieldInfo != BitfieldRegistry::Get().BitfieldsByName.end()) {
        auto value = bitfieldInfo.Value()->Find((EnumUnderlyingType)index);
        if (value) {
            return value;
        } else {
            return {};
        }
    }

    auto valueList = GetStaticSymbols().GetStats()->ModifierValueLists.GetByName(enumName);
    if (valueList) {
        std::optional<FixedString> value;
        for (auto const& val : valueList->Values) {
            if (val.Value == index) {
                value = val.Key;
            }
        }

        if (value) {
            return value;
        } else {
            OsiError("Enumeration '" << enumName << "' has no label with index " << index);
            return {};
        }
    }

    OsiError("No such enumeration: " << enumName);
    return {};
}

std::optional<int64_t> EnumLabelToIndex(FixedString const& enumName, FixedString const& label)
{
    auto enumInfo = EnumRegistry::Get().EnumsByName.find(enumName);
    if (enumInfo != EnumRegistry::Get().EnumsByName.end()) {
        auto key = enumInfo.Value()->Find(label);
        if (key) {
            return (int64_t)* key;
        } else {
            return {};
        }
    }
    
    auto bitfieldInfo = BitfieldRegistry::Get().BitfieldsByName.find(enumName);
    if (bitfieldInfo != BitfieldRegistry::Get().BitfieldsByName.end()) {
        auto key = bitfieldInfo.Value()->Find(label);
        if (key) {
            return (int64_t)*key;
        } else {
            return {};
        }
    }

    auto valueList = GetStaticSymbols().GetStats()->ModifierValueLists.GetByName(enumName);
    if (valueList) {
        auto value = valueList->Values.find(FixedString(label));

        if (value) {
            return value.Value();
        } else {
            OsiError("Enumeration '" << enumName << "' has no label named '" << label << "'");
            return {};
        }
    }

    OsiError("No such enumeration: " << enumName);
    return {};
}


bool AddAttribute(FixedString const& modifierList, FixedString const& modifierName, FixedString const& typeName)
{
    if (GetStaticSymbols().GetStats()->Objects.Values.Size() > 0) {
        OsiError("It is not safe to modify stats types after stats data files were loaded!");
        OsiError("(Try using the StatsStructureLoaded event)");
        return false;
    } 
    
    auto modList = GetStaticSymbols().GetStats()->ModifierLists.GetByName(modifierList);
    if (!modList) {
        OsiError("No such modifier list: " << modifierList);
        return false;
    }
    
    if (modList->Attributes.GetByName(modifierName)) {
        OsiError("Modifier list already has an attribute named '" << modifierName << "'");
        return false;
    }

    auto valueListIdx = GetStaticSymbols().GetStats()->ModifierValueLists.GetHandleByName(typeName);
    if (valueListIdx == -1) {
        OsiError("No such stats value type: " << typeName);
        return false;
    }

    auto modifier = GameAlloc<Modifier>();
    modifier->EnumerationIndex = valueListIdx;
    modifier->Name = modifierName;
    modList->Attributes.Insert(modifier);
    return true;
}

std::optional<int32_t> AddEnumerationValue(FixedString const& typeName, FixedString const& enumLabel)
{
    auto valueList = GetStaticSymbols().GetStats()->ModifierValueLists.GetByName(typeName);
    if (!valueList) {
        OsiError("No such stats value type: " << typeName);
        return {};
    }

    if (valueList->GetPropertyType() != RPGEnumerationType::Enumeration) {
        OsiError("Stats value type is not an enumeration: " << typeName);
        return {};
    }

    if (valueList->Values.find(enumLabel) != valueList->Values.end()) {
        OsiError("Stats value type already has a value named '" << typeName << "'");
        return {};
    }

    auto value = valueList->Values.size();
    valueList->Values.insert(std::make_pair(enumLabel, value));
    return value;
}

template <class T>
void DoExecuteFunctors(ExecuteFunctorProc<T>* proc, Functors* functors, HitResult& hit, T* context)
{
    if (!proc) {
        ERR("Handle not found for executing functors in context %d", context->Type);
        return;
    }

    proc(&hit, functors, context);
}

void DoExecuteInterruptFunctors(ExecuteInterruptFunctorProc* proc, ecs::EntityWorld* world, Functors* functors, HitResult& hit, InterruptContextData* context)
{
    if (!proc) {
        ERR("Handle not found for executing functors in context %d", context->Type);
        return;
    }

    proc(&hit, world, functors, context);
}

#define P(ty) \
    case FunctorContextType::ty: \
        DoExecuteFunctors(GetStaticSymbols().esv__ExecuteStatsFunctor_##ty##Context, functors, hit, static_cast<ty##ContextData*>(context)); \
        break;

void ExecuteFunctors(lua_State* L, Functors* functors, ContextData* context)
{
    OPTICK_EVENT();
    HitResult hit;
    switch (context->Type) {
    P(AttackTarget)
    P(AttackPosition)
    P(Move)
    P(Target)
    P(NearbyAttacked)
    P(NearbyAttacking)
    P(Equip)
    P(Source)

    case FunctorContextType::Interrupt:
        DoExecuteInterruptFunctors(GetStaticSymbols().esv__ExecuteStatsFunctor_InterruptContext, State::FromLua(L)->GetEntityWorld(), functors, hit, static_cast<InterruptContextData*>(context));
        break;

    default:
        ERR("Don't know how to execute functors in context %d", context->Type);
        break;
    }
}

#undef P

void ExecuteFunctor(lua_State* L, Functor* functor, ContextData* context)
{
    OPTICK_EVENT();
    Functors functors;
    functors.Insert(functor->Clone());
    ExecuteFunctors(L, &functors, context);
}

template <class T>
T* DefaultInitFunctorParams(lua_State* L)
{
    auto* helpers = State::FromLua(L)->GetEntitySystemHelpers();
    auto classDescs = helpers->GetResourceManager<resource::ClassDescription>();

    static T ctx;

    new (&ctx) T();
    ctx.Type = T::ContextType;
    ctx.PropertyContext = stats::PropertyContext::TARGET | stats::PropertyContext::AOE;
    ctx.ClassResources = *classDescs;
    return &ctx;
}

#define CTX(ty) case FunctorContextType::ty: return DefaultInitFunctorParams<ty##ContextData>(L);

ContextData* PrepareFunctorParams(lua_State* L, FunctorContextType type)
{
    switch (type) {
        CTX(AttackTarget)
        CTX(AttackPosition)
        CTX(Move)
        CTX(Target)
        CTX(NearbyAttacked)
        CTX(NearbyAttacking)
        CTX(Equip)
        CTX(Source)
        CTX(Interrupt)

    default:
        {
            luaL_error(L, "Unsupported context type");
            return nullptr;
        }
    }
}

void RegisterStatsLib()
{
    DECLARE_MODULE(Stats, Both)
    BEGIN_MODULE()
    MODULE_FUNCTION(GetStatsManager)
    MODULE_FUNCTION(GetModifierAttributes)
    MODULE_FUNCTION(GetStats)
    MODULE_FUNCTION(GetStatsLoadedBefore)
    MODULE_FUNCTION(Get)
    MODULE_FUNCTION(GetCachedSpell)
    MODULE_FUNCTION(GetCachedStatus)
    MODULE_FUNCTION(GetCachedPassive)
    MODULE_FUNCTION(GetCachedInterrupt)
    MODULE_FUNCTION(Create)
    // TODO - move to stats object method
    MODULE_FUNCTION(Sync)
    MODULE_FUNCTION(SetPersistence)
    MODULE_FUNCTION(EnumIndexToLabel)
    MODULE_FUNCTION(EnumLabelToIndex)
    MODULE_FUNCTION(AddAttribute)
    MODULE_FUNCTION(AddEnumerationValue)
    MODULE_FUNCTION(ExecuteFunctors)
    MODULE_FUNCTION(ExecuteFunctor)
    MODULE_FUNCTION(PrepareFunctorParams)
    END_MODULE()
        
/*    DECLARE_SUBMODULE(Stats, SkillSet, Both)
    BEGIN_MODULE()
    MODULE_NAMED_FUNCTION("GetLegacy", GetSkillSet)
    MODULE_NAMED_FUNCTION("Update", UpdateSkillSet)
    END_MODULE()
        
    DECLARE_SUBMODULE(Stats, EquipmentSet, Both)
    BEGIN_MODULE()
    MODULE_NAMED_FUNCTION("GetLegacy", GetEquipmentSet)
    MODULE_NAMED_FUNCTION("Update", UpdateEquipmentSet)
    END_MODULE()*/
        
    DECLARE_SUBMODULE(Stats, TreasureTable, Both)
    BEGIN_MODULE()
    MODULE_NAMED_FUNCTION("Get", GetTreasureTable)
    MODULE_NAMED_FUNCTION("GetLegacy", GetTreasureTableLegacy)
    MODULE_NAMED_FUNCTION("Update", UpdateTreasureTable)
    END_MODULE()
        
    DECLARE_SUBMODULE(Stats, TreasureCategory, Both)
    BEGIN_MODULE()
    MODULE_NAMED_FUNCTION("GetLegacy", GetTreasureCategory)
    MODULE_NAMED_FUNCTION("Update", UpdateTreasureCategory)
    END_MODULE()
        
/*    DECLARE_SUBMODULE(Stats, ItemCombo, Both)
    BEGIN_MODULE()
    MODULE_NAMED_FUNCTION("GetLegacy", GetItemCombo)
    MODULE_NAMED_FUNCTION("Update", UpdateItemCombo)
    END_MODULE()
        
    DECLARE_SUBMODULE(Stats, ItemComboPreview, Both)
    BEGIN_MODULE()
    MODULE_NAMED_FUNCTION("GetLegacy", GetItemComboPreviewData)
    MODULE_NAMED_FUNCTION("Update", UpdateItemComboPreviewData)
    END_MODULE()
        
    DECLARE_SUBMODULE(Stats, ItemComboProperty, Both)
    BEGIN_MODULE()
    MODULE_NAMED_FUNCTION("GetLegacy", GetItemComboProperty)
    MODULE_NAMED_FUNCTION("Update", UpdateItemComboProperty)
    END_MODULE()
        
    DECLARE_SUBMODULE(Stats, ItemGroup, Both)
    BEGIN_MODULE()
    MODULE_NAMED_FUNCTION("GetLegacy", GetItemGroup)
    END_MODULE()
        
    DECLARE_SUBMODULE(Stats, NameGroup, Both)
    BEGIN_MODULE()
    MODULE_NAMED_FUNCTION("GetLegacy", GetNameGroup)
    END_MODULE()

    DECLARE_SUBMODULE(Stats, ItemSet, Both)
    BEGIN_MODULE()
    MODULE_NAMED_FUNCTION("GetLegacy", GetItemSet)
    MODULE_NAMED_FUNCTION("Update", UpdateItemSet)
    END_MODULE()

    DECLARE_SUBMODULE(Stats, ItemColor, Both)
    BEGIN_MODULE()
    MODULE_NAMED_FUNCTION("Get", GetItemColor)
    MODULE_NAMED_FUNCTION("Update", UpdateItemColor)
    MODULE_NAMED_FUNCTION("GetAll", GetAllItemColors)
    END_MODULE()*/
}

END_NS()
