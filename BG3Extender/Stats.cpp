#include "stdafx.h"

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Symbols.h>
#include <GameDefinitions/Enumerations.h>
#include <ScriptHelpers.h>
#include "OsirisProxy.h"
#include <PropertyMaps.h>

namespace bg3se
{
	/*void SkillPrototypeManager::SyncSkillStat(CRPGStats_Object* object, SkillPrototype* proto)
	{
		auto stats = GetStaticSymbols().GetStats();
		proto->Ability = *stats->GetAttributeInt(object, GFS.strAbility);
		proto->Tier = *stats->GetAttributeInt(object, GFS.strTier);
		proto->Requirement = *stats->GetAttributeInt(object, GFS.strRequirement);
		proto->Level = *stats->GetAttributeInt(object, GFS.strLevel);
		proto->Icon = ToFixedString(*stats->GetAttributeString(object, GFS.strIcon));
		proto->MagicCost = *stats->GetAttributeInt(object, GFS.strMagicCost);
		proto->MemoryCost = *stats->GetAttributeInt(object, GFS.strMemoryCost);
		proto->ActionPoints = *stats->GetAttributeInt(object, GFS.strActionPoints);
		proto->Cooldown = *stats->GetAttributeInt(object, GFS.strCooldown) * 6.0f;
		proto->CooldownReduction = *stats->GetAttributeInt(object, GFS.strCooldownReduction) / 100.0f;
		proto->ChargeDuration = *stats->GetAttributeInt(object, GFS.strChargeDuration) * 6.0f;

		auto displayNameKey = ToFixedString(*stats->GetAttributeString(object, GFS.strDisplayName));
		TranslatedString displayName;
		if (script::GetTranslatedStringFromKey(displayNameKey, displayName)) {
			proto->DisplayName = displayName.Handle.ReferenceString;
		}

		STDString aiFlags = object->AIFlags.Str;
		proto->AiFlags = (AIFlags)0;
		if (aiFlags.find("CanNotUse") != STDString::npos) proto->AiFlags |= AIFlags::CanNotUse;
		if (aiFlags.find("IgnoreSelf") != STDString::npos) proto->AiFlags |= AIFlags::IgnoreSelf;
		if (aiFlags.find("IgnoreDebuff") != STDString::npos) proto->AiFlags |= AIFlags::IgnoreDebuff;
		if (aiFlags.find("IgnoreBuff") != STDString::npos) proto->AiFlags |= AIFlags::IgnoreBuff;
		if (aiFlags.find("StatusIsSecondary") != STDString::npos) proto->AiFlags |= AIFlags::StatusIsSecondary;
		if (aiFlags.find("IgnoreControl") != STDString::npos) proto->AiFlags |= AIFlags::IgnoreControl;
		if (aiFlags.find("CanNotTargetFrozen") != STDString::npos) proto->AiFlags |= AIFlags::CanNotTargetFrozen;
	}

	void SkillPrototypeManager::SyncSkillStat(CRPGStats_Object* object)
	{
		auto stats = GetStaticSymbols().GetStats();
		auto skillTypeFs = stats->GetAttributeString(object, GFS.strSkillType);
		if (!skillTypeFs || !*skillTypeFs) {
			OsiError("Skill stats object has no SkillType?");
			return;
		}

		// Cone is an alias of Zone, but is not part of the SkillType enum
		std::optional<SkillType> skillType;
		if (strcmp(*skillTypeFs, "Cone") == 0) {
			skillType = SkillType::Zone;
		} else {
			skillType = EnumInfo<SkillType>::Find(*skillTypeFs);
		}

		if (!skillType) {
			OsiError("Unsupported SkillType: " << *skillTypeFs);
			return;
		}

		auto pProto = Prototypes.Find(object->Name);
		if (pProto == nullptr) {
			auto proto = GameAlloc<SkillPrototype>();
			proto->RPGStatsObjectIndex = object->Handle;
			proto->SkillTypeId = *skillType;
			proto->SkillId = object->Name;
			proto->RootSkillPrototype = nullptr;
			SyncSkillStat(object, proto);

			Prototypes.Insert(proto->SkillId, proto);
			PrototypeNames.Add(proto->SkillId);

			auto lv1Proto = GameAlloc<SkillPrototype>();
			*lv1Proto = *proto;

			STDString lv1Name = proto->SkillId.Str;
			lv1Name += "_-1";
			lv1Proto->SkillId = MakeFixedString(lv1Name.c_str());
			lv1Proto->Level = -1;

			proto->ChildPrototypes.Add(lv1Proto);
			lv1Proto->RootSkillPrototype = proto;

			Prototypes.Insert(lv1Proto->SkillId, lv1Proto);
			PrototypeNames.Add(lv1Proto->SkillId);
		} else {
			SyncSkillStat(object, *pProto);

			STDString lv1Name = (*pProto)->SkillId.Str;
			lv1Name += "_-1";
			auto lv1Proto = Prototypes.Find(MakeFixedString(lv1Name.c_str()));
			if (lv1Proto) {
				SyncSkillStat(object, *lv1Proto);
			}
		}
	}

	void StatusPrototypeManager::SyncStatusStat(CRPGStats_Object* object)
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
		/*auto msg = gOsirisProxy->GetNetworkManager().GetFreeServerMessage(ReservedUserId);
		if (!msg) {
			OsiErrorS("Failed to get free message");
			return;
		}

		auto& wrap = msg->GetMessage();
		ToProtobuf(wrap.mutable_s2c_sync_stat());
		if (syncDuringLoading) {
			gOsirisProxy->GetNetworkManager().ServerBroadcastToConnectedPeers(msg, ReservedUserId, true);
		} else {
			gOsirisProxy->GetNetworkManager().ServerBroadcast(msg, ReservedUserId, true);
		}*/
	}

	bool RPGEnumeration::IsIndexedProperty() const
	{
		return Name != GFS.strProperties
			&& Name != GFS.strConditions
			&& Name != GFS.strAIFlags
			&& Name != GFS.strRequirements
			&& Name != GFS.strMemorizationRequirements;
	}

	bool RPGEnumeration::IsStringIndexedProperty() const
	{
		return Name == GFS.strFixedString;
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
	/*
	CRPGStatsVMTMappings gCRPGStatsVMTMappings;

	CRPGStatsVMTMappings::CRPGStatsVMTMappings()
	{
		PropertyTypes = {
			{CRPGStats_Object_Property_Type::Custom, nullptr},
			{CRPGStats_Object_Property_Type::Status, nullptr},
			{CRPGStats_Object_Property_Type::SurfaceChange, nullptr},
			{CRPGStats_Object_Property_Type::GameAction, nullptr},
			{CRPGStats_Object_Property_Type::OsirisTask, nullptr},
			{CRPGStats_Object_Property_Type::Sabotage, nullptr},
			{CRPGStats_Object_Property_Type::Summon, nullptr},
			{CRPGStats_Object_Property_Type::Force, nullptr}
		};
	}
	
	void CRPGStatsVMTMappings::MapVMTs()
	{
		if (VMTsMapped) return;
		auto stats = GetStaticSymbols().GetStats();

		if (stats->objects.Primitives.Size > 0) {
			ObjectVMT = stats->objects.Primitives[0]->VMT;
		}

		stats->PropertyLists.Iterate([this](auto const& k, auto const& propList) {
			SkillPropertiesVMT = *(void**)propList;
			for (auto prop : propList->Properties.Primitives) {
				PropertyTypes[prop->TypeId] = *(void**)prop;
			}
		});

		VMTsMapped = true;
	}*/

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
	}

	void RPGStats::SyncWithPrototypeManager(CRPGStats_Object* object)
	{
		auto modifier = modifierList.Find(object->ModifierListIndex);
		if (modifier->Name == GFS.strSkillData) {
			auto skillProtoMgr = GetStaticSymbols().eoc__SkillPrototypeManager;
			if (skillProtoMgr && *skillProtoMgr) {
				(*skillProtoMgr)->SyncSkillStat(object);
			}
		} else if (modifier->Name == GFS.strStatusData) {
			auto statusProtoMgr = GetStaticSymbols().eoc__StatusPrototypeManager;
			if (statusProtoMgr && *statusProtoMgr) {
				(*statusProtoMgr)->SyncStatusStat(object);
			}
		}
	}

	void RPGStats::BroadcastSyncAll()
	{
		for (auto const& statsId : gOsirisProxy->GetServerExtensionState().GetDynamicStats()) {
			auto object = objects.Find(statsId);
			if (!object) {
				OsiError("Stat entry '" << statsId << "' is marked as dynamic but cannot be found! It will not be synced to the client!");
			} else {
				object->BroadcastSyncMessage(true);
			}
		}
	}*/

	std::optional<int64_t*> RPGStats::GetInt64(int attributeFlagsId)
	{
		if (attributeFlagsId > 0) {
			return Int64s[attributeFlagsId];
		} else {
			return {};
		}
	}

	int64_t* RPGStats::GetOrCreateInt64(int& attributeId)
	{
		if (attributeId <= 0) {
			attributeId = (int)Int64s.Size;
			auto val = GameAlloc<int64_t>();
			*val = (int64_t)0;
			Int64s.Add(val);
		}

		return Int64s[attributeId];
	}

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

	RPGEnumeration * RPGStats::GetAttributeInfo(CRPGStats_Object * object, FixedString const& attributeName, int & attributeIndex)
	{
		auto objModifiers = ModifierLists.Find(object->ModifierListIndex);
		if (objModifiers == nullptr) {
			return nullptr;
		}

		auto modifierInfo = objModifiers->GetAttributeInfo(attributeName, &attributeIndex);
		if (modifierInfo == nullptr) {
			return nullptr;
		}

		auto typeInfo = ModifierValueLists.Find(modifierInfo->RPGEnumerationIndex);
		return typeInfo;
	}

	std::optional<char const *> RPGStats::GetAttributeString(CRPGStats_Object * object, FixedString const& attributeName)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(object, attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			return {};
		}

		auto index = object->IndexedProperties[attributeIndex];
		if (typeInfo->Name == GFS.strFixedString) {
			return FixedStrings[index].GetString();
		} else if (typeInfo->Name == GFS.strAttributeFlags) {
			auto attrFlags = GetInt64(index);
			if (attrFlags) {
				STDString flagsStr;

				for (auto i = 0; i < 64; i++) {
					if ((uint64_t)*(StatAttributeFlags*)*attrFlags & (1ull << i)) {
						auto label = EnumInfo<StatAttributeFlags>::Find((StatAttributeFlags)(1ull << i));
						if (label) {
							if (!flagsStr.empty()) {
								flagsStr += ';';
							}

							flagsStr += label.GetString();
						}
					}
				}

				return gTempStrings.Make(flagsStr);
			} else {
				return "";
			}
		} else if (typeInfo->Values.ItemCount > 0) {
			auto enumLabel = typeInfo->Values.FindByValue(index);
			if (enumLabel) {
				return enumLabel->GetString();
			} else {
				return {};
			}
		}
		else {
			return {};
		}
	}

	std::optional<int> RPGStats::GetAttributeInt(CRPGStats_Object * object, FixedString const& attributeName)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(object, attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			return {};
		}

		auto index = object->IndexedProperties[attributeIndex];
		if (typeInfo->Name == GFS.strConstantInt
			|| typeInfo->Values.ItemCount > 0) {
			return index;
		}
		else {
			return {};
		}
	}

	std::optional<int> RPGStats::GetAttributeIntScaled(CRPGStats_Object * object, FixedString const& attributeName, int level)
	{
		auto objModifiers = ModifierLists.Find(object->ModifierListIndex);
		if (objModifiers == nullptr) {
			return {};
		}

		int attributeIndex;
		auto modifierInfo = objModifiers->GetAttributeInfo(attributeName, &attributeIndex);
		if (modifierInfo == nullptr) {
			return {};
		}

		auto levelMap = LevelMaps.Find(modifierInfo->LevelMapIndex);
		auto value = object->IndexedProperties[attributeIndex];
		if (levelMap) {
			return (int32_t)levelMap->GetScaledValue(value, level);
		} else {
			return value;
		}
	}

	int RPGStats::GetOrCreateFixedString(const char * value)
	{
		FixedString fs(value);
		if (!fs) return -1;

		for (uint32_t i = 0; i < FixedStrings.Size; i++) {
			if (FixedStrings[i] == fs) {
				return i;
			}
		}

		FixedStrings.Add(fs);
		return FixedStrings.Size - 1;
	}

	std::optional<StatAttributeFlags> RPGStats::StringToAttributeFlags(const char * value)
	{
		StatAttributeFlags flags{ 0 };
		STDString token;
		std::istringstream tokenStream(value);
		while (std::getline(tokenStream, token, ';')) {
			auto label = EnumInfo<StatAttributeFlags>::Find(token.c_str());
			if (label) {
				flags |= *label;
			} else {
				OsiError("Invalid AttributeFlag: " << token);
			}
		}

		return flags;
	}

	void* RPGStats::BuildScriptCheckBlock(STDString const& source)
	{
		throw std::runtime_error("FIXME!");
		/*auto build = GetStaticSymbols().ScriptCheckBlock__Build;
		if (!build) {
			OsiError("ScriptCheckBlock::Build not available!");
			return nullptr;
		}

		return build(source, ConditionsManager.Variables, 0, (int)source.size());*/
	}

	void* RPGStats::BuildScriptCheckBlockFromProperties(STDString const& source)
	{
		throw std::runtime_error("FIXME!");
		/*STDString updated = source;
		for (size_t i = 0; i < updated.size(); i++) {
			if (updated[i] == ';') {
				updated[i] = '&';
			}
		}
		
		return BuildScriptCheckBlock(updated);*/
	}

	bool RPGStats::SetAttributeString(CRPGStats_Object * object, FixedString const& attributeName, const char * value)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(object, attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			OsiError("Couldn't fetch type info for " << object->Name << "." << attributeName);
			return false;
		}

		if (typeInfo->Name == GFS.strFixedString) {
			auto fs = GetOrCreateFixedString(value);
			if (fs != -1) {
				object->IndexedProperties[attributeIndex] = fs;
			} else {
				OsiError("Couldn't set " << object->Name << "." << attributeName << ": Unable to allocate pooled string");
			}
		} else if (typeInfo->Name == GFS.strAttributeFlags) {
			auto attrFlags = (StatAttributeFlags*)GetOrCreateInt64(object->IndexedProperties[attributeIndex]);
			auto flags = StringToAttributeFlags(value);
			if (flags) {
				*attrFlags = *flags;
			}
		} else if (typeInfo->Values.ItemCount > 0) {
			auto enumIndex = typeInfo->Values.Find(FixedString(value));
			if (enumIndex != nullptr) {
				object->IndexedProperties[attributeIndex] = *enumIndex;
			} else {
				OsiError("Couldn't set " << object->Name << "." << attributeName << ": Value (\"" << value << "\") is not a valid enum label");
				return false;
			}
		} else {
			OsiError("Couldn't set " << object->Name << "." << attributeName << ": Inappropriate type: " << typeInfo->Name);
			return false;
		}

		return true;
	}

	bool RPGStats::SetAttributeInt(CRPGStats_Object * object, FixedString const& attributeName, int32_t value)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(object, attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			OsiError("Couldn't fetch type info for " << object->Name << "." << attributeName);
			return false;
		}

		if (typeInfo->Name == GFS.strConstantInt) {
			object->IndexedProperties[attributeIndex] = value;
		} else if (typeInfo->Values.ItemCount > 0) {
			if (value >= 0 && value < (int)typeInfo->Values.ItemCount) {
				object->IndexedProperties[attributeIndex] = value;
			} else {
				OsiError("Couldn't set " << object->Name << "." << attributeName << ": Enum index (\"" << value << "\") out of range");
				return false;
			}
		} else {
			OsiError("Couldn't set " << object->Name << "." << attributeName << ": Inappropriate type: " << typeInfo->Name);
			return false;
		}

		return true;
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