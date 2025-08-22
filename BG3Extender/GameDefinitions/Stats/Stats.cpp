#include "stdafx.h"

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Symbols.h>
#include <Extender/Shared/ScriptHelpers.h>
#include <Extender/ScriptExtender.h>

#include <GameDefinitions/Stats/Functors.inl>

BEGIN_NS(stats)

bool SpellPrototypeManager::SyncStat(Object* object, SpellPrototype* proto)
{
    auto sync = GetStaticSymbols().eoc__SpellPrototype__Init;
    if (!sync) {
        OsiError("eoc::SpellPrototype::Init not mapped!");
        return false;
    }

    proto->SpellTypeId = EnumInfo<SpellType>::Find(object->GetFixedString(GFS.strSpellType).value_or(GFS.strEmpty)).value_or((SpellType)0);
    proto->UseCosts.clear();
    proto->RitualCosts.clear();
    proto->DualWieldingUseCosts.clear();
    proto->HitCostGroups.clear();
    proto->VariableUseCosts.clear();
    proto->VariableDualWieldingUseCosts.clear();
    proto->VariableRitualCosts.clear();
    proto->SpellAnimation = SpellPrototypeAnimationData{};
    proto->DualWieldingSpellAnimation = SpellPrototypeAnimationData{};
    proto->AlternativeCastTextEvents.clear();
    proto->ContainerSpells.clear();
    proto->Trajectories.clear();

    proto->SpellFlags = (SpellFlags)0;
    proto->LineOfSightFlags = 0;
    proto->CinematicArenaFlags = 0;
    proto->WeaponTypes = 0;
    proto->AiFlags = 0;
    proto->RequirementEvents = 0;

    sync(proto, object->Name);
    return true;
}


void SpellPrototypeManager::SyncStat(Object* object)
{
    auto stats = GetStaticSymbols().GetStats();
    auto pProto = Spells.try_get(object->Name);
    if (!pProto) {
        auto proto = GameAlloc<SpellPrototype>();
        if (SyncStat(object, proto)) {
            Spells.set(proto->SpellId, proto);
            SpellNames.push_back(proto->SpellId);
        }
    } else {
        SyncStat(object, *pProto);
    }
}

bool StatusPrototypeManager::SyncStat(Object* object, StatusPrototype* proto)
{
    auto sync = GetStaticSymbols().eoc__StatusPrototype__Init;
    if (!sync) {
        OsiError("eoc::StatusPrototype::Init not mapped!");
        return false;
    }

    proto->StatusId = EnumInfo<StatusType>::Find(object->GetFixedString(GFS.strStatusType).value_or(GFS.strEmpty)).value_or((StatusType)0);
    proto->StatusPropertyFlags = 0;
    proto->StatusGroups = 0;
    proto->Flags = 0;
    proto->RemoveEvents = 0;
    proto->Boosts.clear();

    sync(proto, object->Name, 0);

    auto parseBoosts = GetStaticSymbols().eoc__ParseStaticBoosts;
    auto boosts = object->GetFixedString(GFS.strBoosts);
    if (parseBoosts != nullptr && boosts) {
        LSStringView sv(boosts->GetStringView());
        uint64_t temp[0x10]{ 0 };
        parseBoosts(sv, proto->Boosts, &temp);
    }

    return true;
}


void StatusPrototypeManager::SyncStat(Object* object)
{
    auto stats = GetStaticSymbols().GetStats();
    auto pProto = Statuses.try_get(object->Name);
    if (!pProto) {
        auto proto = GameAlloc<StatusPrototype>();
        if (SyncStat(object, proto)) {
            Statuses.set(proto->StatusName, proto);
            StatusNames.push_back(proto->StatusName);
        }
    } else {
        SyncStat(object, *pProto);
    }
}


bool PassivePrototypeManager::SyncStat(Object* object, PassivePrototype* proto)
{
    auto sync = GetStaticSymbols().eoc__PassivePrototype__Init;
    if (!sync) {
        OsiError("eoc::PassivePrototype::Init not mapped!");
        return false;
    }

    proto->Properties = 0;
    proto->EnabledContext = 0;
    proto->StatsFunctorContext = 0;
    proto->ToggleOffContext = 0;
    proto->BoostContext = 0;
    proto->Boosts_SV.clear();

    sync(proto, object);
    return true;
}


void PassivePrototypeManager::SyncStat(Object* object)
{
    auto pProto = Passives.find(object->Name);
    if (pProto == Passives.end()) {
        auto proto = Passives.get_or_insert(object->Name);
        SyncStat(object, proto);
    } else {
        SyncStat(object, &pProto.Value());
    }
}


bool InterruptPrototypeManager::SyncStat(Object* object, InterruptPrototype* proto)
{
    auto sync = GetStaticSymbols().eoc__InterruptPrototype__Init;
    if (!sync) {
        OsiError("eoc::InterruptPrototype::Init not mapped!");
        return false;
    }

    proto->Costs.clear();

    sync(proto, object);
    return true;
}


void InterruptPrototypeManager::SyncStat(Object* object)
{
    auto proto = Interrupts.try_get(object->Name);
    if (!proto) {
        proto = Interrupts.add_key(object->Name);
    }

    SyncStat(object, proto);
}

    

bool RPGEnumeration::IsFlagType(FixedString const& typeName)
{
    return
        typeName == GFS.strAttributeFlags
        || typeName == GFS.strWeaponFlags
        || typeName == GFS.strResistanceFlags
        || typeName == GFS.strPassiveFlags
        || typeName == GFS.strSpellFlagList
        || typeName == GFS.strStatusEvent
        || typeName == GFS.strStatusPropertyFlags
        || typeName == GFS.strProficiencyGroupFlags
        || typeName == GFS.strCinematicArenaFlags
        || typeName == GFS.strLineOfSightFlags
        || typeName == GFS.strSpellCategoryFlags
        || typeName == GFS.strStatsFunctorContext
        || typeName == GFS.strStatusGroupFlags
        || typeName == GFS.strInterruptContext
        || typeName == GFS.strInterruptContextScope
        || typeName == GFS.strInterruptDefaultValue
        || typeName == GFS.strInterruptFlagsList
        || typeName == GFS.strAuraFlags
        || typeName == GFS.strAbilityFlags;
}

RPGEnumerationType RPGEnumeration::GetPropertyType() const
{
    if (Name == GFS.strConstantInt) {
        return RPGEnumerationType::Int;
    }
        
    if (Name == GFS.strConstantFloat) {
        return RPGEnumerationType::Float;
    }
        
    if (Name == GFS.strFixedString
        || Name == GFS.strStatusIDs) {
        return RPGEnumerationType::FixedString;
    }

    if (Name == GFS.strGuid) {
        return RPGEnumerationType::GUID;
    }

    if (Name == GFS.strStatsFunctors) {
        return RPGEnumerationType::StatsFunctors;
    }

    if (Name == GFS.strConditions
        || Name == GFS.strTargetConditions
        || Name == GFS.strUseConditions) {
        return RPGEnumerationType::Conditions;
    }

    if (Name == GFS.strRollConditions) {
        return RPGEnumerationType::RollConditions;
    }

    if (Name == GFS.strRequirements) {
        return RPGEnumerationType::Requirements;
    }

    if (Name == GFS.strMemorizationRequirements) {
        return RPGEnumerationType::MemorizationRequirements;
    }

    if (Name == GFS.strTranslatedString) {
        return RPGEnumerationType::TranslatedString;
    }

    if (Values.size() > 0) {
        if (IsFlagType(Name)) {
            return RPGEnumerationType::Flags;
        } else {
            return RPGEnumerationType::Enumeration;
        }
    }
        
    return RPGEnumerationType::Unknown;
}

Modifier * ModifierList::GetAttributeInfo(FixedString const& name, int * attributeIndex) const
{
    auto index = Attributes.GetHandleByName(name);
    if (index == -1) {
        return nullptr;
    } else {
        *attributeIndex = index;
        return Attributes.GetByHandle(index);
    }
}

bool RPGStats::ObjectExists(FixedString const& statsId, FixedString const& type)
{
    auto object = Objects.GetByName(statsId);
    if (object == nullptr) {
        return false;
    }

    auto typeInfo = GetTypeInfo(object);
    if (typeInfo == nullptr) {
        return false;
    }

    return typeInfo->Name == type;
}


std::optional<Object*> RPGStats::CreateObject(FixedString const& name, FixedString const& type)
{
    auto modifierIdx = ModifierLists.GetHandleByName(type);
    if (modifierIdx == -1) {
        OsiError("Unknown modifier list type: " << type);
        return {};
    }

    return CreateObject(name, modifierIdx);
}

std::optional<Object*> RPGStats::CreateObject(FixedString const& name, int32_t modifierListIndex)
{
    auto modifierList = ModifierLists.GetByHandle(modifierListIndex);
    if (!modifierList) {
        OsiError("Modifier list doesn't exist: " << name);
        return {};
    }

    auto object = Objects.GetByName(name);
    if (object) {
        OsiError("A stats object already exists with this name: " << name);
        return {};
    }

    if (Objects.Values.empty()) {
        OsiError("No stats object found to copy VMT from!");
        return {};
    }

    object = GameAlloc<Object>();
    object->VMT = Objects.Values[0]->VMT;
    object->ModifierListIndex = modifierListIndex;
    object->IndexedProperties.resize(modifierList->Attributes.Values.size(), 0);
    object->Name = name;

    for (unsigned i = 0; i < modifierList->Attributes.Values.size(); i++) {
        auto const& modifier = modifierList->Attributes.Values[i];
        auto valueList = ModifierValueLists.GetByHandle(modifier->EnumerationIndex);
        if (valueList) {
            auto type = valueList->GetPropertyType();
            if (type == RPGEnumerationType::Conditions || type == RPGEnumerationType::RollConditions) {
                object->IndexedProperties[i] = -1;
            }
        }
    }

    Objects.Insert(object);
    return object;
}

void RPGStats::SyncWithPrototypeManager(Object* object)
{
    auto modifier = ModifierLists.GetByHandle(object->ModifierListIndex);
    if (modifier->Name == GFS.strSpellData) {
        auto spellProtoMgr = GetStaticSymbols().eoc__SpellPrototypeManager;
        if (spellProtoMgr && *spellProtoMgr) {
            (*spellProtoMgr)->SyncStat(object);
        }
    } else if (modifier->Name == GFS.strStatusData) {
        auto statusProtoMgr = GetStaticSymbols().eoc__StatusPrototypeManager;
        if (statusProtoMgr && *statusProtoMgr) {
            (*statusProtoMgr)->SyncStat(object);
        }
    } else if (modifier->Name == GFS.strPassiveData) {
        auto passiveMgr = GetStaticSymbols().eoc__PassivePrototypeManager;
        if (passiveMgr && *passiveMgr) {
            (*passiveMgr)->SyncStat(object);
        }
    } else if (modifier->Name == GFS.strInterruptData) {
        auto interruptMgr = GetStaticSymbols().eoc__InterruptPrototypeManager;
        if (interruptMgr && *interruptMgr) {
            (*interruptMgr)->SyncStat(object);
        }
    }
}

std::optional<int> RPGStats::EnumLabelToIndex(FixedString const& enumName, char const* enumLabel)
{
    auto rpgEnum = ModifierValueLists.GetByName(enumName);
    if (rpgEnum == nullptr) {
        OsiError("No enum named '" << enumName << "' exists");
        return {};
    }

    auto index = rpgEnum->Values.find(FixedString(enumLabel));
    if (index == rpgEnum->Values.end()) {
        return {};
    } else {
        return index.Value();
    }
}

FixedString RPGStats::EnumIndexToLabel(FixedString const& enumName, int index)
{
    auto rpgEnum = ModifierValueLists.GetByName(enumName);
    if (rpgEnum == nullptr) {
        OsiError("No enum named '" << enumName << "' exists");
        return FixedString{};
    }

    auto value = rpgEnum->Values.find_by_value(index);
    if (value != rpgEnum->Values.end()) {
        return value.Key();
    }
    else {
        return FixedString{};
    }
}

std::optional<FixedString*> RPGStats::GetFixedString(int stringId)
{
    if (stringId > 0) {
        return &FixedStrings[stringId];
    } else {
        return {};
    }
}

FixedString* RPGStats::GetOrCreateFixedString(int& stringId)
{
    if (stringId < 0) {
        stringId = (int)FixedStrings.size();
        FixedStrings.push_back(FixedString{});
    }

    return &FixedStrings[stringId];
}

std::optional<int64_t*> RPGStats::GetInt64(int attributeId)
{
    if (attributeId > 0) {
        return Int64s[attributeId];
    } else {
        return {};
    }
}

int64_t* RPGStats::GetOrCreateInt64(int& attributeId)
{
    if (attributeId < 0) {
        attributeId = (int)Int64s.Size();
        auto val = GameAlloc<int64_t>();
        *val = (int64_t)0;
        Int64s.push_back(val);
    }

    return Int64s[attributeId];
}

std::optional<float*> RPGStats::GetFloat(int attributeId)
{
    if (attributeId > 0) {
        return &Floats[attributeId];
    } else {
        return {};
    }
}

float* RPGStats::GetOrCreateFloat(int& attributeId)
{
    if (attributeId < 0) {
        attributeId = (int)Floats.Size();
        Floats.push_back(.0f);
    }

    return &Floats[attributeId];
}

std::optional<Guid*> RPGStats::GetGuid(int attributeId)
{
    if (attributeId > 0) {
        return &GUIDs[attributeId];
    } else {
        return {};
    }
}

Guid* RPGStats::GetOrCreateGuid(int& attributeId)
{
    if (attributeId < 0) {
        attributeId = (int)GUIDs.Size();
        GUIDs.push_back(Guid{});
    }

    return &GUIDs[attributeId];
}

std::optional<TranslatedString*> RPGStats::GetTranslatedString(int attributeId)
{
    if (attributeId > 0) {
        return &TranslatedStrings[attributeId];
    } else {
        return {};
    }
}

TranslatedString* RPGStats::GetOrCreateTranslatedString(int& attributeId)
{
    if (attributeId < 0) {
        attributeId = (int)TranslatedStrings.Size();
        TranslatedStrings.push_back(TranslatedString{});
    }

    return &TranslatedStrings[attributeId];
}

std::optional<STDString*> RPGStats::GetConditions(int conditionsId)
{
    if (conditionsId > 0) {
        return &Conditions[conditionsId];
    } else {
        return {};
    }
}

int RPGStats::GetOrCreateConditions(STDString const& conditions)
{
    if (conditions.empty()) {
        return -1;
    }

    for (unsigned i = 0; i < Conditions.Size(); i++) {
        if (Conditions[i] == conditions) {
            return (int)i;
        }
    }

    Conditions.push_back(conditions);
    return (int)Conditions.Size() - 1;
}

Modifier * RPGStats::GetModifierInfo(FixedString const& modifierListName, FixedString const& modifierName)
{
    auto modifiers = ModifierLists.GetByName(modifierListName);
    if (modifiers != nullptr) {
        return modifiers->Attributes.GetByName(modifierName);
    } else {
        return nullptr;
    }
}

ModifierList * RPGStats::GetTypeInfo(Object * object)
{
    return ModifierLists.GetByHandle(object->ModifierListIndex);
}

RPGStats::VMTMappings RPGStats::sVMTMappings;

void RPGStats::VMTMappings::Update()
{
    if (VMTsMapped) return;
    auto stats = GetStaticSymbols().GetStats();

    if (stats->Objects.Values.Size() > 0) {
        ObjectVMT = stats->Objects.Values[0]->VMT;
    }

    VMTsMapped = true;
}

Functors* RPGStats::ConstructFunctorSet(FixedString const& propertyName)
{
    sVMTMappings.Update();

    auto functorSet = GameAlloc<Functors>();
    functorSet->UniqueName = propertyName;
    return functorSet;
}

Functor* RPGStats::ConstructFunctor(FunctorId action)
{
    sVMTMappings.Update();
    auto stats = GetStaticSymbols().GetStats();

#define V(type) case type::FunctorType: \
    functor = GameAlloc<type>(); \
    break;

    Functor* functor{ nullptr };
    switch (action) {
        V(CustomDescriptionFunctor)
        V(ResurrectFunctor)
        V(SabotageFunctor)
        V(SummonFunctor)
        V(ForceFunctor)
        V(DouseFunctor)
        V(SwapPlacesFunctor)
        V(PickupFunctor)
        V(CreateSurfaceFunctor)
        V(CreateConeSurfaceFunctor)
        V(RemoveStatusFunctor)
        V(ExecuteWeaponFunctorsFunctor)
        V(TeleportSourceFunctor)
        V(SetStatusDurationFunctor)
        V(UseAttackFunctor)
        V(BreakConcentrationFunctor)
        V(RestoreResourceFunctor)
        V(SpawnFunctor)
        V(StabilizeFunctor)
        V(UnlockFunctor)
        V(ResetCombatTurnFunctor)
        V(RemoveAuraByChildStatusFunctor)
        V(ApplyStatusFunctor)
        V(DealDamageFunctor)
        V(UseActionResourceFunctor)
        V(CreateExplosionFunctor)
        V(SurfaceChangeFunctor)
        V(ApplyEquipmentStatusFunctor)
        V(RegainHitPointsFunctor)
        V(UseSpellFunctor)
        V(SummonInInventoryFunctor)
        V(SpawnInInventoryFunctor)
        V(RemoveUniqueStatusFunctor)
        V(DisarmWeaponFunctor)
        V(ExtenderFunctor)

    default:
        OsiError("Unhandled stats functor action: " << (unsigned)action);
        return nullptr;
    }

    functor->TypeId = action;

    return functor;
}

Object * StatFindObject(char const * name, bool warnOnError)
{
    auto stats = GetStaticSymbols().GetStats();
    if (stats == nullptr) {
        OsiError("RPGStats not available");
        return nullptr;
    }

    auto object = stats->Objects.GetByName(FixedString(name));
    if (object == nullptr) {
        if (warnOnError) {
            OsiError("Stat object '" << name << "' does not exist");
        }
        return nullptr;
    }

    return object;
}

Object * StatFindObject(int index)
{
    auto stats = GetStaticSymbols().GetStats();
    if (stats == nullptr) {
        OsiError("RPGStats not available");
        return nullptr;
    }

    auto object = stats->Objects.GetByHandle(index);
    if (object == nullptr) {
        OsiError("Stat object #" << index << " does not exist");
        return nullptr;
    }

    return object;
}

Object * SpellPrototype::GetStats() const
{
    return StatFindObject(StatsObjectIndex);
}

Object * StatusPrototype::GetStats() const
{
    return StatFindObject(StatsObjectIndex);
}

END_NS()
