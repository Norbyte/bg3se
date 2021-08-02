#include "stdafx.h"

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Symbols.h>
#include <GameDefinitions/Enumerations.h>
#include <Extender/Shared/ScriptHelpers.h>
#include <Extender/ScriptExtender.h>

namespace bg3se
{
	bool SpellPrototypeManager::SyncStat(CRPGStats_Object* object, SpellPrototype* proto)
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


	void SpellPrototypeManager::SyncStat(CRPGStats_Object* object)
	{
		auto stats = GetStaticSymbols().GetStats();
		auto pProto = Spells.Find(object->Name);
		if (pProto == nullptr) {
			auto proto = GameAlloc<SpellPrototype>();
			if (SyncStat(object, proto)) {
				Spells.Set(proto->SpellId, proto);
				SpellNames.Add(proto->SpellId);
			}
		} else {
			SyncStat(object, **pProto);
		}
	}

	/*void StatusPrototypeManager::SyncStatusStat(CRPGStats_Object* object)
	{
		auto stats = GetStaticSymbols().GetStats();
		auto statusTypeFs = stats->GetAttributeString(object, GFS.strStatusType);
		if (!statusTypeFs || !*statusTypeFs) {
			OsiError("Status stats object has no StatusType?");
			return;
		}

		auto statusType = EnumInfo<StatusType>::Find(*statusTypeFs);
		if (!statusType) {
			OsiError("Unsupported StatusType: " << *statusTypeFs);
			return;
		}

		StatusPrototype* proto;
		auto pProto = Prototypes.Find(object->Name);
		if (pProto == nullptr) {
			auto hitProto = Prototypes.Find(GFS.strHIT);
			if (!hitProto) {
				OsiError("Couldn't sync new status entry - missing HIT status!");
				return;
			}

			proto = GameAlloc<StatusPrototype>();
			proto->VMT = (*hitProto)->VMT;
			proto->RPGStatsObjectIndex = object->Handle;
			proto->StatusId = *statusType;
			proto->StatusName = object->Name;
			proto->HasStats = false;

			Prototypes.Insert(proto->StatusName, proto);
			PrototypeNames.Add(proto->StatusName);
		} else {
			proto = *pProto;
		}

		if (proto->HasStats) {
			OsiError("Cannot sync stats of builtin status " << proto->StatusName);
			return;
		}

		auto displayNameKey = ToFixedString(*stats->GetAttributeString(object, GFS.strDisplayName));
		TranslatedString displayName;
		if (script::GetTranslatedStringFromKey(displayNameKey, displayName)) {
			proto->DisplayName = displayName;
		}

		proto->Icon = ToFixedString(*stats->GetAttributeString(object, GFS.strIcon));
		// FIXME - AbsorbSurfaceType
	}

	void CRPGStats_Requirement::ToProtobuf(StatRequirement* msg) const
	{
		msg->set_requirement((int32_t)RequirementId);
		msg->set_int_param(IntParam);
		if (StringParam) {
			msg->set_string_param(StringParam.Str);
		}
		msg->set_negate(Negate);
	}

	void CRPGStats_Requirement::FromProtobuf(StatRequirement const& msg)
	{
		RequirementId = (RequirementType)msg.requirement();
		IntParam = msg.int_param();
		if (!msg.string_param().empty()) {
			StringParam = MakeFixedString(msg.string_param().c_str());
		}

		Negate = msg.negate();
	}*/


	void CRPGStats_Object::ToProtobuf(MsgS2CSyncStat* msg) const
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

	void CRPGStats_Object::FromProtobuf(MsgS2CSyncStat const& msg)
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
			CRPGStats_Requirement requirement;
			requirement.FromProtobuf(reqmt);
			Requirements.Add(requirement);
		}

		MemorizationRequirements.Clear();
		for (auto const& reqmt : msg.memorization_requirements()) {
			CRPGStats_Requirement requirement;
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

	void CRPGStats_Object::BroadcastSyncMessage(bool syncDuringLoading) const
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

		if (Values.Count() > 0) {
			if (IsFlagType(Name)) {
				return RPGEnumerationType::Flags;
			} else {
				return RPGEnumerationType::Enumeration;
			}
		}
		
		return RPGEnumerationType::Unknown;
	}

	CRPGStats_Modifier * ModifierList::GetAttributeInfo(FixedString const& name, int * attributeIndex) const
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


	std::optional<CRPGStats_Object*> RPGStats::CreateObject(FixedString const& name, FixedString const& type)
	{
		auto modifierIdx = ModifierLists.FindIndex(type);
		if (!modifierIdx) {
			OsiError("Unknown modifier list type: " << type);
			return {};
		}

		return CreateObject(name, *modifierIdx);
	}

	std::optional<CRPGStats_Object*> RPGStats::CreateObject(FixedString const& name, int32_t modifierListIndex)
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

		object = GameAlloc<CRPGStats_Object>();
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

	void RPGStats::SyncWithPrototypeManager(CRPGStats_Object* object)
	{
		auto modifier = ModifierLists.Find(object->ModifierListIndex);
		if (modifier->Name == GFS.strSpellData) {
			auto spellProtoMgr = GetStaticSymbols().eoc__SpellPrototypeManager;
			if (spellProtoMgr && *spellProtoMgr) {
				(*spellProtoMgr)->SyncStat(object);
			}
		} else if (modifier->Name == GFS.strStatusData) {
			ERR("FIXME - RPGStats::SyncWithPrototypeManager() not yet implemented for statuses!");
			/*auto statusProtoMgr = GetStaticSymbols().eoc__StatusPrototypeManager;
			if (statusProtoMgr && *statusProtoMgr) {
				(*statusProtoMgr)->SyncStatusStat(object);
			}*/
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

		auto index = rpgEnum->Values.Find(FixedString(enumLabel));
		if (index == nullptr) {
			return {};
		} else {
			return *index;
		}
	}

	FixedString RPGStats::EnumIndexToLabel(FixedString const& enumName, int index)
	{
		auto rpgEnum = ModifierValueLists.Find(enumName);
		if (rpgEnum == nullptr) {
			OsiError("No enum named '" << enumName << "' exists");
			return FixedString{};
		}

		auto value = rpgEnum->Values.FindByValue(index);
		if (value) {
			return *value;
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
			stringId = (int)FixedStrings.Size;
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
			attributeId = (int)Int64s.Size;
			auto val = GameAlloc<int64_t>();
			*val = (int64_t)0;
			Int64s.Add(val);
		}

		return Int64s[attributeId];
	}

	std::optional<float*> RPGStats::GetFloat(int attributeId)
	{
		if (attributeId > 0) {
			return &ConstantFloats[attributeId];
		} else {
			return {};
		}
	}

	float* RPGStats::GetOrCreateFloat(int& attributeId)
	{
		if (attributeId < 0) {
			attributeId = (int)ConstantFloats.Size();
			ConstantFloats.Add(.0f);
		}

		return &ConstantFloats[attributeId];
	}

	std::optional<UUID*> RPGStats::GetGuid(int attributeId)
	{
		if (attributeId > 0) {
			return &GUIDs[attributeId];
		} else {
			return {};
		}
	}

	UUID* RPGStats::GetOrCreateGuid(int& attributeId)
	{
		if (attributeId < 0) {
			attributeId = (int)GUIDs.Size();
			GUIDs.Add(UUID{});
		}

		return &GUIDs[attributeId];
	}

	std::optional<STDString*> RPGStats::GetConditions(int conditionsId)
	{
		if (conditionsId > 0) {
			return &ConditionList[conditionsId];
		} else {
			return {};
		}
	}

	int RPGStats::GetOrCreateConditions(STDString const& conditions)
	{
		if (conditions.empty()) {
			return -1;
		}

		for (unsigned i = 0; i < ConditionList.Size(); i++) {
			if (ConditionList[i] == conditions) {
				return (int)i;
			}
		}

		ConditionList.Add(conditions);
		return (int)ConditionList.Size() - 1;
	}

	CRPGStats_Modifier * RPGStats::GetModifierInfo(FixedString const& modifierListName, FixedString const& modifierName)
	{
		auto modifiers = ModifierLists.Find(modifierListName);
		if (modifiers != nullptr) {
			return modifiers->Attributes.Find(modifierName);
		} else {
			return nullptr;
		}
	}

	ModifierList * RPGStats::GetTypeInfo(CRPGStats_Object * object)
	{
		return ModifierLists.Find(object->ModifierListIndex);
	}

	RPGStats::VMTMappings RPGStats::sVMTMappings;

	void RPGStats::VMTMappings::Update()
	{
		if (VMTsMapped) return;
		auto stats = GetStaticSymbols().GetStats();

		if (stats->Objects.Primitives.Size > 0) {
			ObjectVMT = stats->Objects.Primitives[0]->VMT;
		}

		for (auto const& kv : stats->StatsFunctors) {
			if (StatsFunctorSetVMT == nullptr) {
				StatsFunctorSetVMT = kv.Value->VMT;
			}

			for (auto const& prop : kv.Value->FunctorList) {
				auto it = FunctorVMTs.find(prop->TypeId);
				if (it == FunctorVMTs.end()) {
					FunctorVMTs.insert(std::make_pair(prop->TypeId, prop->VMT));
				}
			}
		}

		VMTsMapped = true;
	}

	StatsFunctorSet* RPGStats::ConstructFunctorSet(FixedString const& propertyName)
	{
		sVMTMappings.Update();

		if (!sVMTMappings.StatsFunctorSetVMT) {
			OsiError("Cannot construct functor set - VMT not mapped!");
			return nullptr;
		}

		auto functorSet = GameAlloc<StatsFunctorSet>();
		functorSet->VMT = sVMTMappings.StatsFunctorSetVMT;
		functorSet->FunctorsByName.Init(31);
		functorSet->UniqueName = propertyName;
		return functorSet;
	}

	StatsFunctorBase* RPGStats::ConstructFunctor(StatsFunctorActionId action)
	{
		sVMTMappings.Update();
		auto stats = GetStaticSymbols().GetStats();

		auto vmtIt = sVMTMappings.FunctorVMTs.find(action);
		if (vmtIt == sVMTMappings.FunctorVMTs.end()) {
			LuaError("Unable to construct functors of this type: " << (unsigned)action << " - VMT not mapped!");
			return nullptr;
		}

#define V(type) case type::FunctorId: \
		functor = GameAlloc<type>(); \
		break;

		StatsFunctorBase* functor{ nullptr };
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

	CRPGStats_Object * StatFindObject(char const * name)
	{
		auto stats = GetStaticSymbols().GetStats();
		if (stats == nullptr) {
			OsiError("RPGStats not available");
			return nullptr;
		}

		auto object = stats->Objects.Find(name);
		if (object == nullptr) {
			OsiError("Stat object '" << name << "' does not exist");
			return nullptr;
		}

		return object;
	}

	CRPGStats_Object * StatFindObject(int index)
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

	CRPGStats_Object * SpellPrototype::GetStats() const
	{
		return StatFindObject(StatsObjectIndex);
	}

	CRPGStats_Object * StatusPrototype::GetStats() const
	{
		return StatFindObject(StatsObjectIndex);
	}

	/*
#define DEFN_GETTER(type, name) decltype(CharacterStatsGetters::Wrapper##name) * decltype(CharacterStatsGetters::Wrapper##name)::gHook;
#include <GameDefinitions/CharacterGetters.inl>
#undef DEFN_GETTER

	decltype(CharacterStatsGetters::WrapperHitChance) * decltype(CharacterStatsGetters::WrapperHitChance)::gHook;

	void CharacterStatsGetters::WrapAll()
	{
		if (Wrapped) return;

#define DEFN_GETTER(type, name) if (Get##name != nullptr) { \
	Wrapper##name.Wrap(Get##name); \
}

#include <GameDefinitions/CharacterGetters.inl>
#undef DEFN_GETTER

		if (GetHitChance != nullptr) {
			WrapperHitChance.Wrap(GetHitChance);
		}

		Wrapped = true;
	}

	void CharacterStatsGetters::UnwrapAll()
	{
#define DEFN_GETTER(type, name) Wrapper##name.ClearHook();
#include <GameDefinitions/CharacterGetters.inl>
#undef DEFN_GETTER
		WrapperHitChance.ClearHook();
	}

	std::optional<int32_t> CharacterStatsGetters::GetStat(CDivinityStats_Character * character, 
		FixedString const& name, bool original, bool excludeBoosts)
	{
		auto statType = EnumInfo<StatGetterType>::Find(name);
		if (!statType) {
			return {};
		}

		switch (*statType) {
#define DEFN_GETTER(type, n) case StatGetterType::n: \
	return CharacterStatGetter<n##Tag>(Get##n, Wrapper##n, character, original, excludeBoosts);

#include <GameDefinitions/CharacterGetters.inl>
#undef DEFN_GETTER

		case StatGetterType::PhysicalResistance:
			return character->GetPhysicalResistance(excludeBoosts);

		case StatGetterType::PiercingResistance:
			return character->GetPiercingResistance(excludeBoosts);

		case StatGetterType::CorrosiveResistance:
			return character->GetCorrosiveResistance(excludeBoosts);

		case StatGetterType::MagicResistance:
			return character->GetMagicResistance(excludeBoosts);

		case StatGetterType::DamageBoost:
			return character->GetDamageBoost();

		default:
			OsiError("No stat fetcher defined for stat: '" << name << "'");
			return {};
		}
	}*/

	/*
	std::optional<int32_t> CDivinityStats_Character::GetHitChance(CDivinityStats_Character * target)
	{
		auto getter = GetStaticSymbols().CharStatsGetters.GetHitChance;
		if (getter == nullptr) {
			return {};
		}

		return getter(this, target);
	}


	std::optional<int32_t> CDivinityStats_Character::GetStat(FixedString const& name, bool excludeBoosts)
	{
		return GetStaticSymbols().CharStatsGetters.GetStat(this, name, false, excludeBoosts);
	}


	bool CDivinityStats_Character::HasTalent(TalentType talent, bool excludeBoosts)
	{
		auto getter = GetStaticSymbols().CharStatsGetters.GetTalent;
		if (getter) {
			return getter(this, talent, excludeBoosts);
		} else {
			return false;
		}
	}


	int32_t CDivinityStats_Character::GetAbility(AbilityType ability, bool excludeBoosts)
	{
		auto getter = GetStaticSymbols().CharStatsGetters.GetAbility;
		if (getter) {
			return getter(this, ability, excludeBoosts, false);
		} else {
			return 0;
		}
	}

	CDivinityStats_Item * CDivinityStats_Character::GetItemBySlot(ItemSlot slot, bool mustBeEquipped)
	{
		for (auto stat : EquippedItems) {
			if (stat->ItemSlot == slot && (!mustBeEquipped || stat->IsEquipped)) {
				auto index = stat->ItemStatsHandle;
				if (index >= 0 
					&& index < DivStats->ItemList->Handles.Handles.size()) {
					auto item = DivStats->ItemList->Handles.Handles[index];
					if (item) {
						return item;
					}
				}
			}
		}

		return nullptr;
	}

	CDivinityStats_Item * CDivinityStats_Character::GetMainWeapon()
	{
		auto weapon = GetItemBySlot(ItemSlot::Weapon, true);
		if (weapon == nullptr || weapon->ItemType != EquipmentStatsType::Weapon) {
			weapon = GetItemBySlot(ItemSlot::Shield, true);
			if (weapon == nullptr || weapon->ItemType != EquipmentStatsType::Weapon) {
				weapon = DivStats->DefaultWeaponStats;
			}
		}

		return weapon;
	}

	CDivinityStats_Item * CDivinityStats_Character::GetOffHandWeapon()
	{
		auto mainWeapon = GetItemBySlot(ItemSlot::Weapon, true);
		if (mainWeapon == nullptr || mainWeapon->ItemType != EquipmentStatsType::Weapon) {
			return nullptr;
		}

		auto offHandWeapon = GetItemBySlot(ItemSlot::Shield, true);
		if (offHandWeapon == nullptr || offHandWeapon->ItemType != EquipmentStatsType::Weapon) {
			return nullptr;
		}

		return offHandWeapon;
	}

	bool CDivinityStats_Character::IsBoostActive(uint32_t conditionMask)
	{
		return conditionMask == 0
			|| (conditionMask & 1) && ActiveBoostConditions[1] > 0
			|| (conditionMask & 2) && ActiveBoostConditions[2] > 0
			|| (conditionMask & 4) && ActiveBoostConditions[3] > 0
			|| (conditionMask & 8) && ActiveBoostConditions[4] > 0
			|| (conditionMask & 0x10) && ActiveBoostConditions[5] > 0
			|| (conditionMask & 0x20) && ActiveBoostConditions[6] > 0
			|| (conditionMask & 0x40) && ActiveBoostConditions[7] > 0
			|| (conditionMask & 0x80) && ActiveBoostConditions[8] > 0
			|| (conditionMask & 0x100) && ActiveBoostConditions[9] > 0
			|| (conditionMask & 0x200) && ActiveBoostConditions[10] > 0
			|| (conditionMask & 0x300) && ActiveBoostConditions[11] > 0
			|| (conditionMask & 0x400) && ActiveBoostConditions[12] > 0
			|| (conditionMask & 0x500) && ActiveBoostConditions[13] > 0
			|| (conditionMask & 0x600) && ActiveBoostConditions[14] > 0
			|| (conditionMask & 0x700) && ActiveBoostConditions[15] > 0;
	}

	template <class IterFunc, class ItemFunc>
	int32_t ComputeCharacterStat(CDivinityStats_Character * character, int32_t initVal, IterFunc iter, ItemFunc iterItem, bool excludeBoosts)
	{
		int32_t value = initVal;

		auto lastStatIt = excludeBoosts ? (character->DynamicStats.begin() + 3) : character->DynamicStats.end();

		unsigned i = 0;
		for (auto it = character->DynamicStats.begin(); it != lastStatIt; it++, i++) {
			auto & stat = *it;
			if (stat->BoostConditionsMask == 0 || character->IsBoostActive(stat->BoostConditionsMask)) {
				value = iter(character, value, i, stat);
			}
		}

		if (!excludeBoosts) {
			auto & itemHandles = character->DivStats->ItemList->Handles;
			for (auto stat : character->EquippedItems) {
				if (stat->IsEquipped && stat->ItemStatsHandle >= 0 && stat->ItemStatsHandle < itemHandles.Handles.size()) {
					auto item = itemHandles.Handles[stat->ItemStatsHandle];
					if (item != nullptr) {
						value = iterItem(character, value, item);
					}
				}
			}
		}

		return value;
	}
	
	int32_t CDivinityStats_Character::GetPhysicalResistance(bool excludeBoosts)
	{
		auto physResist = std::min(DynamicStats[0]->PhysicalResistance, DynamicStats[0]->MaxResistance);
		auto addDynamic = [](CDivinityStats_Character * self, int32_t val, unsigned index, CharacterDynamicStat * stat) -> int32_t {
			return val + stat->PhysicalResistance;
		};
		auto addItem = [](CDivinityStats_Character * self, int32_t val, CDivinityStats_Item * item) -> int32_t {
			return val + item->GetPhysicalResistance();
		};

		return ComputeCharacterStat(this, physResist, addDynamic, addItem, excludeBoosts);
	}

	int32_t CDivinityStats_Character::GetPiercingResistance(bool excludeBoosts)
	{
		auto physResist = std::min(DynamicStats[0]->PiercingResistance, DynamicStats[0]->MaxResistance);
		auto addDynamic = [](CDivinityStats_Character * self, int32_t val, unsigned index, CharacterDynamicStat * stat) -> int32_t {
			return val + stat->PiercingResistance;
		};
		auto addItem = [](CDivinityStats_Character * self, int32_t val, CDivinityStats_Item * item) -> int32_t {
			return val + item->GetPiercingResistance();
		};

		return ComputeCharacterStat(this, physResist, addDynamic, addItem, excludeBoosts);
	}

	int32_t CDivinityStats_Character::GetCorrosiveResistance(bool excludeBoosts)
	{
		auto physResist = std::min(DynamicStats[0]->CorrosiveResistance, DynamicStats[0]->MaxResistance);
		auto addDynamic = [](CDivinityStats_Character * self, int32_t val, unsigned index, CharacterDynamicStat * stat) -> int32_t {
			return val + stat->CorrosiveResistance;
		};
		auto addItem = [](CDivinityStats_Character * self, int32_t val, CDivinityStats_Item * item) -> int32_t {
			return val + item->GetCorrosiveResistance();
		};

		return ComputeCharacterStat(this, physResist, addDynamic, addItem, excludeBoosts);
	}

	int32_t CDivinityStats_Character::GetMagicResistance(bool excludeBoosts)
	{
		auto physResist = std::min(DynamicStats[0]->MagicResistance, DynamicStats[0]->MaxResistance);
		auto addDynamic = [](CDivinityStats_Character * self, int32_t val, unsigned index, CharacterDynamicStat * stat) -> int32_t {
			return val + stat->MagicResistance;
		};
		auto addItem = [](CDivinityStats_Character * self, int32_t val, CDivinityStats_Item * item) -> int32_t {
			return val + item->GetMagicResistance();
		};

		return ComputeCharacterStat(this, physResist, addDynamic, addItem, excludeBoosts);
	}

	int32_t CDivinityStats_Character::GetDamageBoost()
	{
		int32_t damageBoost = 0;

		for (auto dynamicStat : DynamicStats) {
			if (dynamicStat->BoostConditionsMask == 0 || IsBoostActive(dynamicStat->BoostConditionsMask)) {
				damageBoost += dynamicStat->DamageBoost;
				if (Level > 1) {
					damageBoost += (int)round(dynamicStat->DamageBoostGrowthPerLevel * (Level - 1) * 0.1f);
				}
			}
		}

		return damageBoost;
	}

	int32_t CDivinityStats_Item::GetPhysicalResistance()
	{
		int32_t resistance = 0;
		for (auto dynamicStat : DynamicAttributes) {
			resistance += dynamicStat->PhysicalResistance;
		}

		return resistance;
	}

	int32_t CDivinityStats_Item::GetPiercingResistance()
	{
		int32_t resistance = 0;
		for (auto dynamicStat : DynamicAttributes) {
			resistance += dynamicStat->PiercingResistance;
		}

		return resistance;
	}

	int32_t CDivinityStats_Item::GetMagicResistance()
	{
		int32_t resistance = 0;
		for (auto dynamicStat : DynamicAttributes) {
			resistance += dynamicStat->MagicResistance;
		}

		return resistance;
	}

	int32_t CDivinityStats_Item::GetCorrosiveResistance()
	{
		int32_t resistance = 0;
		for (auto dynamicStat : DynamicAttributes) {
			resistance += dynamicStat->CorrosiveResistance;
		}

		return resistance;
	}


	bool CDivinityStats_Item::HasTalent(TalentType talent)
	{
		for (auto dynamicStat : DynamicAttributes) {
			if (dynamicStat->Talents.HasTalent(talent)) {
				return true;
			}
		}

		return false;
	}


	int32_t CDivinityStats_Item::GetAbility(AbilityType ability)
	{
		int32_t points = 0;
		for (auto dynamicStat : DynamicAttributes) {
			points += dynamicStat->AbilityModifiers[(unsigned)ability];
		}

		return points;
	}


	bool CRPGStats_Object_Property_Extender::GetDescription(STDWString* Line1)
	{
		ecl::LuaClientPin lua(ecl::ExtensionState::Get());
		if (lua) {
			auto desc = lua->GetSkillPropertyDescription(this);
			if (desc) {
				*Line1 = FromUTF8(*desc);
				return true;
			}
		}

		*Line1 = L"";
		return true;
	}


	PropertyMapBase & CDivinityStats_Equipment_Attributes::GetPropertyMap() const
	{
		switch (StatsType) {
		case EquipmentStatsType::Weapon:
			return gEquipmentAttributesWeaponPropertyMap;

		case EquipmentStatsType::Armor:
			return gEquipmentAttributesArmorPropertyMap;

		case EquipmentStatsType::Shield:
			return gEquipmentAttributesShieldPropertyMap;

		default:
			OsiError("Unknown equipment stats type: " << (unsigned)StatsType);
			return gEquipmentAttributesPropertyMap;
		}
	}*/
}