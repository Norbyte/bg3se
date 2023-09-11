#include "stdafx.h"

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Symbols.h>
#include <Extender/Shared/ScriptHelpers.h>
#include <Extender/ScriptExtender.h>

BEGIN_NS(stats)

bool SpellPrototypeManager::SyncStat(Object* object, SpellPrototype* proto)
{
	auto sync = GetStaticSymbols().eoc__SpellPrototype__Init;
	if (!sync) {
		OsiError("eoc::SpellPrototype::Init not mapped!");
		return false;
	}

	proto->UseCosts2.Clear();
	proto->RitualCosts2.Clear();
	proto->DualWieldingUseCosts2.Clear();
	proto->CastTextEvents.Clear();
	proto->UseCosts.Clear();
	proto->DualWieldingUseCosts.Clear();
	proto->RitualCosts.Clear();
	proto->SpellAnimationNoneMagic.Clear();
	proto->DualWieldingSpellAnimationNoneMagic.Clear();
	proto->AlternativeCastTextEvents.Clear();
	proto->ContainerSpells.Clear();

	proto->SpellFlags = 0;
	proto->LineOfSightFlags = 0;
	proto->SpellCategoryFlags = 0;
	proto->WeaponTypes = 0;
	proto->AiFlags = 0;

	sync(proto, object->Name);
	return true;
}


void SpellPrototypeManager::SyncStat(Object* object)
{
	auto stats = GetStaticSymbols().GetStats();
	auto pProto = Spells.Find(object->Name);
	if (!pProto) {
		auto proto = GameAlloc<SpellPrototype>();
		if (SyncStat(object, proto)) {
			Spells.Set(proto->SpellId, proto);
			SpellNames.Add(proto->SpellId);
		}
	} else {
		SyncStat(object, **pProto);
	}
}

bool StatusPrototypeManager::SyncStat(Object* object, StatusPrototype* proto)
{
	auto sync = GetStaticSymbols().eoc__StatusPrototype__Init;
	if (!sync) {
		OsiError("eoc::StatusPrototype::Init not mapped!");
		return false;
	}

	proto->StatusPropertyFlags = 0;
	proto->StatusGroups = 0;
	proto->Flags = 0;
	proto->RemoveEvents = 0;
	proto->Boosts.Clear();

	sync(proto, object->Name, 1);
	return true;
}


void StatusPrototypeManager::SyncStat(Object* object)
{
	auto stats = GetStaticSymbols().GetStats();
	auto pProto = Statuses.find(object->Name);
	if (pProto == Statuses.end()) {
		auto proto = GameAlloc<StatusPrototype>();
		if (SyncStat(object, proto)) {
			Statuses.insert(proto->StatusName, proto);
		}
	}
	else {
		SyncStat(object, pProto.Value());
	}
}

bool PassiveManager::SyncStat(Object* object, Passive* proto)
{
	auto sync = GetStaticSymbols().eoc__Passive__Init;
	if (!sync) {
		OsiError("eoc::Passive::Init not mapped!");
		return false;
	}

	proto->Properties = 0;
	proto->StatsFunctorContext = 0;
	proto->ToggleOffContext = 0;
	proto->BoostContext = 0;
	proto->Boosts.Clear();

	sync(proto, object);
	return true;
}


void PassiveManager::SyncStat(Object* object)
{
	/*auto stats = GetStaticSymbols().GetStats();
	auto pProto = Passives.find(object->Name);
	if (pProto == Passives.end()) {
		auto proto = Passives.insert(object->Name);
		SyncStat(object, proto);
	} else {
		SyncStat(object, pProto.Value());
	}*/
}

/*
void Requirement::ToProtobuf(StatRequirement* msg) const
{
	msg->set_requirement((int32_t)RequirementId);
	msg->set_int_param(IntParam);
	if (StringParam) {
		msg->set_string_param(StringParam.Str);
	}
	msg->set_negate(Negate);
}

void Requirement::FromProtobuf(StatRequirement const& msg)
{
	RequirementId = (RequirementType)msg.requirement();
	IntParam = msg.int_param();
	if (!msg.string_param().empty()) {
		StringParam = MakeFixedString(msg.string_param().c_str());
	}

	Negate = msg.negate();
}*/


void Object::ToProtobuf(MsgS2CSyncStat* msg) const
{
	/*msg->set_name(Name.Str);
	msg->set_level(Level);
	msg->set_modifier_list(ModifierListIndex);

	auto stats = GetStaticSymbols().GetStats();
	auto modifierList = stats->modifierList.Find(ModifierListIndex);

	for (size_t i = 0; i < IndexedProperties.size(); i++) {
		auto value = IndexedProperties[i];
		auto indexedProp = msg->add_indexed_properties();
		auto modifier = modifierList->Attributes.Find((uint32_t)i);
		auto enumeration = stats->modifierValueList.Find(modifier->RPGEnumerationIndex);
		if (enumeration->IsIndexedProperty()) {
			if (enumeration->IsStringIndexedProperty()) {
				indexedProp->set_stringval(stats->ModifierFSSet[value].Str);
			} else {
				indexedProp->set_intval(value);
			}
		}
	}

	msg->set_ai_flags(AIFlags.Str);

	for (auto const& reqmt : Requirements) {
		reqmt.ToProtobuf(msg->add_requirements());
	}

	for (auto const& reqmt : MemorizationRequirements) {
		reqmt.ToProtobuf(msg->add_memorization_requirements());
	}

	for (auto const& category : ComboCategories) {
		msg->add_combo_categories(category.Str);
	}

	PropertyList.Iterate([msg](auto const& key, auto const& propertyList) {
		propertyList->ToProtobuf(key, msg->add_property_lists());
	});*/
}

void Object::FromProtobuf(MsgS2CSyncStat const& msg)
{
	/*auto stats = GetStaticSymbols().GetStats();
	Level = msg.level();

	if (msg.indexed_properties_size() != IndexedProperties.size()) {
		OsiError("IndexedProperties size mismatch for '" << Name << "'! Got "
			<< msg.indexed_properties_size() << ", expected " << IndexedProperties.size());
		return;
	}

	auto modifierList = stats->modifierList.Find(ModifierListIndex);
	for (size_t i = 0; i < IndexedProperties.size(); i++) {
		auto modifier = modifierList->Attributes.Find((uint32_t)i);
		auto enumeration = stats->modifierValueList.Find(modifier->RPGEnumerationIndex);
		if (enumeration->IsIndexedProperty()) {
			auto const& prop = msg.indexed_properties().Get((uint32_t)i);
			if (enumeration->IsStringIndexedProperty()) {
				IndexedProperties[i] = stats->GetOrCreateFixedString(prop.stringval().c_str());
			} else {
				IndexedProperties[i] = prop.intval();
			}
		} else {
			IndexedProperties[i] = 0;
		}
	}

	AIFlags = MakeFixedString(msg.ai_flags().c_str());

	Requirements.Clear();
	for (auto const& reqmt : msg.requirements()) {
		Requirement requirement;
		requirement.FromProtobuf(reqmt);
		Requirements.Add(requirement);
	}

	MemorizationRequirements.Clear();
	for (auto const& reqmt : msg.memorization_requirements()) {
		Requirement requirement;
		requirement.FromProtobuf(reqmt);
		MemorizationRequirements.Add(requirement);
	}

	ComboCategories.Clear();
	for (auto const& category : msg.combo_categories()) {
		ComboCategories.Add(MakeFixedString(category.c_str()));
	}

	PropertyList.Clear();
	for (auto const& props : msg.property_lists()) {
		auto name = MakeFixedString(props.name().c_str());
		auto propertyList = stats->ConstructPropertyList(name);
		propertyList->FromProtobuf(props);
		PropertyList.Insert(name, propertyList);
	}*/
}

void Object::BroadcastSyncMessage(bool syncDuringLoading) const
{
	/*auto msg = gExtender->GetNetworkManager().GetFreeServerMessage(ReservedUserId);
	if (!msg) {
		OsiErrorS("Failed to get free message");
		return;
	}

	auto& wrap = msg->GetMessage();
	ToProtobuf(wrap.mutable_s2c_sync_stat());
	if (syncDuringLoading) {
		gExtender->GetNetworkManager().ServerBroadcastToConnectedPeers(msg, ReservedUserId, true);
	} else {
		gExtender->GetNetworkManager().ServerBroadcast(msg, ReservedUserId, true);
	}*/
}
	

bool RPGEnumeration::IsFlagType(FixedString const& typeName)
{
	return
		typeName == GFS.strAttributeFlags
		|| typeName == GFS.strSpellFlagList
		|| typeName == GFS.strWeaponFlags
		|| typeName == GFS.strResistanceFlags
		|| typeName == GFS.strPassiveFlags
		|| typeName == GFS.strProficiencyGroupFlags
		|| typeName == GFS.strStatsFunctorContext
		|| typeName == GFS.strStatusEvent
		|| typeName == GFS.strStatusPropertyFlags
		|| typeName == GFS.strStatusGroupFlags
		|| typeName == GFS.strLineOfSightFlags
		|| typeName == GFS.strSpellCategoryFlags;
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
	auto index = Attributes.FindIndex(name);
	if (!index) {
		return nullptr;
	} else {
		*attributeIndex = *index;
		return Attributes.Find(*index);
	}
}

bool RPGStats::ObjectExists(FixedString const& statsId, FixedString const& type)
{
	auto object = Objects.Find(statsId);
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
	auto modifierIdx = ModifierLists.FindIndex(type);
	if (!modifierIdx) {
		OsiError("Unknown modifier list type: " << type);
		return {};
	}

	return CreateObject(name, *modifierIdx);
}

std::optional<Object*> RPGStats::CreateObject(FixedString const& name, int32_t modifierListIndex)
{
	throw std::runtime_error("FIXME!");
	/*auto modifier = modifierList.Find(modifierListIndex);
	if (!modifier) {
		OsiError("Modifier list doesn't exist: " << name);
		return {};
	}

	auto object = objects.Find(name);
	if (object) {
		OsiError("A stats object already exists with this name: " << name);
		return {};
	}

	object = GameAlloc<Object>();
	object->VMT = gCRPGStatsVMTMappings.ObjectVMT;
	object->ModifierListIndex = modifierListIndex;
	object->IndexedProperties.resize(modifier->Attributes.Primitives.Size, 0);
	object->DivStats = DivinityStats;
	object->Name = name;
	object->PropertyList.Init(3);
	object->ConditionList.Init(3);

	object->Handle = objects.Primitives.Size;
	objects.Add(name, object);

	return object;*/
}

/*void RPGStats::SyncObjectFromServer(MsgS2CSyncStat const& msg)
{
	auto object = objects.Find(ToFixedString(msg.name().c_str()));
	if (object) {
		object->FromProtobuf(msg);
		SyncWithPrototypeManager(object);
	} else {
		auto newObject = CreateObject(MakeFixedString(msg.name().c_str()), msg.modifier_list());
		if (!newObject) {
			OsiError("Could not construct stats object from server: " << msg.name());
			return;
		}

		(*newObject)->FromProtobuf(msg);
		SyncWithPrototypeManager(*newObject);
	}
}*/

void RPGStats::SyncWithPrototypeManager(Object* object)
{
	auto modifier = ModifierLists.Find(object->ModifierListIndex);
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
		auto passiveMgr = GetStaticSymbols().eoc__PassiveManager;
		if (passiveMgr && *passiveMgr) {
			(*passiveMgr)->SyncStat(object);
		}
	}
}

/*void RPGStats::BroadcastSyncAll()
{
	for (auto const& statsId : gExtender->GetServer().GetExtensionState().GetDynamicStats()) {
		auto object = objects.Find(statsId);
		if (!object) {
			OsiError("Stat entry '" << statsId << "' is marked as dynamic but cannot be found! It will not be synced to the client!");
		} else {
			object->BroadcastSyncMessage(true);
		}
	}
}*/

std::optional<int> RPGStats::EnumLabelToIndex(FixedString const& enumName, char const* enumLabel)
{
	auto rpgEnum = ModifierValueLists.Find(enumName);
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
	auto rpgEnum = ModifierValueLists.Find(enumName);
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
		stringId = (int)FixedStrings.Size();
		FixedStrings.Add(FixedString{});
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
		Int64s.Add(val);
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
		Floats.Add(.0f);
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
		GUIDs.Add(Guid{});
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
		TranslatedStrings.Add(TranslatedString{});
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

	Conditions.Add(conditions);
	return (int)Conditions.Size() - 1;
}

Modifier * RPGStats::GetModifierInfo(FixedString const& modifierListName, FixedString const& modifierName)
{
	auto modifiers = ModifierLists.Find(modifierListName);
	if (modifiers != nullptr) {
		return modifiers->Attributes.Find(modifierName);
	} else {
		return nullptr;
	}
}

ModifierList * RPGStats::GetTypeInfo(Object * object)
{
	return ModifierLists.Find(object->ModifierListIndex);
}

RPGStats::VMTMappings RPGStats::sVMTMappings;

void RPGStats::VMTMappings::Update()
{
	if (VMTsMapped) return;
	auto stats = GetStaticSymbols().GetStats();

	if (stats->Objects.Primitives.Size() > 0) {
		ObjectVMT = stats->Objects.Primitives[0]->VMT;
	}

	for (auto const& kv : stats->StatsFunctors) {
		if (StatsFunctorSetVMT == nullptr) {
			StatsFunctorSetVMT = kv.Value->VMT;
		}

		for (auto const& prop : kv.Value->Functors) {
			auto it = FunctorVMTs.find(prop->TypeId);
			if (it == FunctorVMTs.end()) {
				FunctorVMTs.insert(std::make_pair(prop->TypeId, prop->VMT));
			}
		}
	}

	VMTsMapped = true;
}

Functors* RPGStats::ConstructFunctorSet(FixedString const& propertyName)
{
	sVMTMappings.Update();

	if (!sVMTMappings.StatsFunctorSetVMT) {
		OsiError("Cannot construct functor set - VMT not mapped!");
		return nullptr;
	}

	auto functorSet = GameAlloc<Functors>();
	functorSet->VMT = sVMTMappings.StatsFunctorSetVMT;
	// functorSet->FunctorsByName.ResizeHashtable(31);
	functorSet->UniqueName = propertyName;
	return functorSet;
}

Functor* RPGStats::ConstructFunctor(FunctorId action)
{
	sVMTMappings.Update();
	auto stats = GetStaticSymbols().GetStats();

	auto vmtIt = sVMTMappings.FunctorVMTs.find(action);
	if (vmtIt == sVMTMappings.FunctorVMTs.end()) {
		LuaError("Unable to construct functors of this type: " << (unsigned)action << " - VMT not mapped!");
		return nullptr;
	}

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
		V(EqualizeFunctor)
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

	functor->VMT = vmtIt->second;
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

	auto object = stats->Objects.Find(name);
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

	auto object = stats->Objects.Find(index);
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
